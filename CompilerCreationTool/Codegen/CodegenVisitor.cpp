#include "stdafx.h"
#include "CodegenVisitor.h"
#include <boost/format.hpp>
#include <unordered_set>

namespace
{
bool Convertible(const ExpressionType& from, const ExpressionType& to)
{
	if (from == to)
	{
		throw std::runtime_error("trying to convert from '" + ToString(from) + "' to itself");
	}

	static const std::unordered_map<ExpressionType::Value, std::unordered_set<ExpressionType::Value>> scAvailableCasts = {
		{ ExpressionType::Int, { ExpressionType::Float, ExpressionType::Bool } },
		{ ExpressionType::Float, { ExpressionType::Int, ExpressionType::Bool } },
		{ ExpressionType::Bool, { ExpressionType::Int, ExpressionType::Float } }
	};

	auto found = scAvailableCasts.find(from.value);
	if (found == scAvailableCasts.end())
	{
		return false;
	}

	const std::unordered_set<ExpressionType::Value>& availableCasts = found->second;
	return availableCasts.find(to.value) != availableCasts.end();
}

bool ConvertibleToBool(const ExpressionType& type)
{
	return Convertible(type, ExpressionType::Bool);
}

// Возвращает тип, к которому должна быть приведена левая и правая часть
//  бинарного выражения для выполнения оператора
boost::optional<ExpressionType> GetPreferredType(const ExpressionType& left, const ExpressionType& right)
{
	if (left == right)
	{
		return left;
	}

	static const std::map<std::pair<ExpressionType::Value, ExpressionType::Value>, ExpressionType::Value> scBinaryCasts = {
		{ { ExpressionType::Int, ExpressionType::Float }, ExpressionType::Float },
		{ { ExpressionType::Int, ExpressionType::Bool }, ExpressionType::Int },
		{ { ExpressionType::Float, ExpressionType::Int }, ExpressionType::Float },
		{ { ExpressionType::Float, ExpressionType::Bool }, ExpressionType::Float },
		{ { ExpressionType::Bool, ExpressionType::Int }, ExpressionType::Int },
		{ { ExpressionType::Bool, ExpressionType::Float }, ExpressionType::Float }
	};

	auto found = scBinaryCasts.find(std::make_pair(left.value, right.value));
	if (found == scBinaryCasts.end())
	{
		return boost::none;
	}
	return ExpressionType(found->second);
}

// Function forward declaration
ExpressionType ToExpressionType(llvm::Type* type);

// This code generator supports only 32 bit integers and booleans
ExpressionType ToExpressionTypeFromInt(llvm::Type* type)
{
	if (type->getIntegerBitWidth() == 32)
	{
		return ExpressionType::Int;
	}
	if (type->getIntegerBitWidth() == 1)
	{
		return ExpressionType::Bool;
	}
	throw std::logic_error("llvm::Type have unsupported integer bit width");
}

// Expression type can be string or array, depending on passed llvm::Type
ExpressionType ToExpressionTypeFromPtr(llvm::Type* type)
{
	assert(type->isPointerTy());
	llvm::Type* ptrElementType = type->getPointerElementType();

	if (ptrElementType->getTypeID() == llvm::Type::IntegerTyID && ptrElementType->getIntegerBitWidth() == 8)
	{
		return ExpressionType::String;
	}

	assert(false);
	throw std::logic_error("can't find proper array ExpressionType for llvm::Type* pointer type");
}

// Function performs cast from llvm::Type to ExpressionType
ExpressionType ToExpressionType(llvm::Type* type)
{
	if (type->isIntegerTy())
	{
		return ToExpressionTypeFromInt(type);
	}
	if (type->isDoubleTy())
	{
		return ExpressionType::Float;
	}
	if (type->isPointerTy())
	{
		return ToExpressionTypeFromPtr(type);
	}
	throw std::logic_error("can't convert undefined llvm::Type to ExpressionType");
}

// Function performs cast from ExpressionType to llvm::Type
llvm::Type* ToLLVMType(ExpressionType type, llvm::LLVMContext& context)
{
	switch (type.value)
	{
	case ExpressionType::Int:
		return llvm::Type::getInt32Ty(context);
	case ExpressionType::Float:
		return llvm::Type::getDoubleTy(context);
	case ExpressionType::Bool:
		return llvm::Type::getInt1Ty(context);
	case ExpressionType::String:
		return llvm::Type::getInt8PtrTy(context);
	}

	throw std::logic_error("can't convert undefined ExpressionType to llvm::Type");
}

llvm::Value* ConvertToIntegerValue(
	llvm::Value* value,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	const ExpressionType type = ToExpressionType(value->getType());

	// Strings and arrays can't be casted to integer
	switch (type.value)
	{
	case ExpressionType::Int:
		return value;
	case ExpressionType::Float:
		return builder.CreateFPToUI(value, llvm::Type::getInt32Ty(llvmContext), "float_to_int_cast_tmp");
	case ExpressionType::Bool:
		return builder.CreateIntCast(value, llvm::Type::getInt32Ty(llvmContext), false, "bool_to_int_cast_tmp");
	default:
		throw std::invalid_argument("can't cast string to integer");
	}
}

llvm::Value* ConvertToFloatValue(
	llvm::Value* value,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	const ExpressionType type = ToExpressionType(value->getType());

	// Strings and arrays can't be casted to float
	switch (type.value)
	{
	case ExpressionType::Int:
	case ExpressionType::Bool:
		return builder.CreateUIToFP(value, llvm::Type::getDoubleTy(llvmContext), "float_cast_tmp");
	case ExpressionType::Float:
		return value;
	default:
		throw std::invalid_argument("can't cast array to integer");
	}
}

llvm::Value* ConvertToBooleanValue(
	llvm::Value* value,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	const ExpressionType type = ToExpressionType(value->getType());

	// String can't be casted to bool
	switch (type.value)
	{
	case ExpressionType::Int:
		return builder.CreateNot(builder.CreateICmpEQ(
			value, llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), uint64_t(0)), "int_to_bool_cast_tmp"));
	case ExpressionType::Float:
		return builder.CreateNot(builder.CreateFCmpOEQ(
			value, llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvmContext), 0.0), "fcmptmp"), "float_to_bool_cast_tmp");
	case ExpressionType::Bool:
		return value;
	default:
		throw std::invalid_argument("can't cast string to bool");
	}
}

// TODO: убрать выбросы исключений из вызываемых функций, чтобы
//  функция возвращала только nullptr при неудаче
// Возвращает nullptr, либо бросает исключение, если нельзя
//  сгенерировать код преобразования значения в другой тип
llvm::Value* CastValue(
	llvm::Value* value,
	ExpressionType type,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	switch (type.value)
	{
	case ExpressionType::Int:
		return ConvertToIntegerValue(value, llvmContext, builder);
	case ExpressionType::Float:
		return ConvertToFloatValue(value, llvmContext, builder);
	case ExpressionType::Bool:
		return ConvertToBooleanValue(value, llvmContext, builder);
	}
	return nullptr;
}

llvm::Value* CreateIntegerBinaryExpression(
	llvm::Value* left,
	llvm::Value* right,
	BinaryExpressionAST::Operator operation,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
#ifdef _DEBUG
	const ExpressionType ltype = ToExpressionType(left->getType());
	const ExpressionType rtype = ToExpressionType(right->getType());
	assert(ltype.value == ExpressionType::Int && rtype.value == ExpressionType::Int);
#endif

	switch (operation)
	{
	case BinaryExpressionAST::Or:
		return builder.CreateOr(
			ConvertToBooleanValue(left, llvmContext, builder),
			ConvertToBooleanValue(right, llvmContext, builder), "int_binary_or_tmp");
	case BinaryExpressionAST::And:
		return builder.CreateAnd(
			ConvertToBooleanValue(left, llvmContext, builder),
			ConvertToBooleanValue(right, llvmContext, builder), "int_binary_and_tmp");
	case BinaryExpressionAST::Equals:
		return builder.CreateICmpEQ(left, right, "int_binary_eq_tmp");
	case BinaryExpressionAST::Less:
		return builder.CreateICmpSLT(left, right, "int_binary_lt_tmp");
	case BinaryExpressionAST::Plus:
		return builder.CreateAdd(left, right, "int_binary_add_tmp");
	case BinaryExpressionAST::Minus:
		return builder.CreateSub(left, right, "int_binary_sub_tmp");
	case BinaryExpressionAST::Mul:
		return builder.CreateMul(left, right, "int_binary_mul_tmp");
	case BinaryExpressionAST::Div:
		return builder.CreateSDiv(left, right, "int_binary_div_tmp");
	}

	assert(false);
	throw std::logic_error("undefined BinaryExpressionAST::Operator in CreateIntBinaryExpression");
}

llvm::Value* CreateFloatBinaryExpression(
	llvm::Value* left,
	llvm::Value* right,
	BinaryExpressionAST::Operator operation,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
#ifdef _DEBUG
	const ExpressionType ltype = ToExpressionType(left->getType());
	const ExpressionType rtype = ToExpressionType(right->getType());
	assert(ltype.value == ExpressionType::Float && rtype.value == ExpressionType::Float);
#endif

	switch (operation)
	{
	case BinaryExpressionAST::Or:
		return builder.CreateOr(
			ConvertToBooleanValue(left, llvmContext, builder),
			ConvertToBooleanValue(right, llvmContext, builder), "flt_binary_or_tmp");
	case BinaryExpressionAST::And:
		return builder.CreateAnd(
			ConvertToBooleanValue(left, llvmContext, builder),
			ConvertToBooleanValue(right, llvmContext, builder), "flt_binary_and_tmp");
	case BinaryExpressionAST::Equals:
		return builder.CreateFCmpOEQ(left, right, "flt_binary_eq_tmp");
	case BinaryExpressionAST::Less:
		return builder.CreateFCmpOLT(left, right, "flt_binary_lt_tmp");
	case BinaryExpressionAST::Plus:
		return builder.CreateFAdd(left, right, "flt_binary_add_tmp");
	case BinaryExpressionAST::Minus:
		return builder.CreateFSub(left, right, "flt_binary_sub_tmp");
	case BinaryExpressionAST::Mul:
		return builder.CreateFMul(left, right, "flt_binary_mul_tmp");
	case BinaryExpressionAST::Div:
		return builder.CreateFDiv(left, right, "flt_binary_div_tmp");
	}

	assert(false);
	throw std::logic_error("undefined BinaryExpressionAST::Operator in CreateFloatBinaryExpression");
}

llvm::Value* CreateBooleanBinaryExpression(
	llvm::Value* left,
	llvm::Value* right,
	BinaryExpressionAST::Operator operation,
	llvm::LLVMContext&,
	llvm::IRBuilder<> & builder)
{
#ifdef _DEBUG
	const ExpressionType ltype = ToExpressionType(left->getType());
	const ExpressionType rtype = ToExpressionType(right->getType());
	assert(ltype.value == ExpressionType::Bool && rtype.value == ExpressionType::Bool);
#endif

	switch (operation)
	{
	case BinaryExpressionAST::Or:
		return builder.CreateOr(left, right, "bool_or_tmp");
	case BinaryExpressionAST::And:
		return builder.CreateAnd(left, right, "bool_and_tmp");
	case BinaryExpressionAST::Equals:
		return builder.CreateICmpEQ(left, right, "bool_eq_tmp");
	case BinaryExpressionAST::Plus:
	case BinaryExpressionAST::Minus:
	case BinaryExpressionAST::Mul:
	case BinaryExpressionAST::Div:
		throw std::runtime_error("can't perform codegen for operator '" + ToString(operation) + "' on two booleans");
	}

	assert(false);
	throw std::logic_error("undefined BinaryExpressionAST::Operator in CreateBooleanBinaryExpression");
}

// Codegen arithmetic value negation
llvm::Value* CodegenNegativeValue(llvm::Value* value, llvm::IRBuilder<>& builder)
{
	const ExpressionType type = ToExpressionType(value->getType());
	switch (type.value)
	{
	case ExpressionType::Int:
		return builder.CreateNeg(value, "negtmp");
	case ExpressionType::Float:
		return builder.CreateFNeg(value, "fnegtmp");
	case ExpressionType::Bool:
		return builder.CreateNeg(value, "bnegtmp");
	default:
		throw std::runtime_error("can't codegen negative value of " + ToString(type) + " type");
	}
}

// Codegen boolean value negation
llvm::Value* CreateValueNegation(
	llvm::Value* value,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	return builder.CreateNot(ConvertToBooleanValue(value, llvmContext, builder));
}

llvm::Value* CreateDefaultValue(
	ExpressionType type,
	llvm::LLVMContext& llvmContext,
	llvm::IRBuilder<>& builder)
{
	switch (type.value)
	{
	case ExpressionType::Int:
		return llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), llvm::APInt(32, uint64_t(0), true));
	case ExpressionType::Float:
		return llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvmContext), 0.0);
	case ExpressionType::Bool:
		return llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvmContext), uint64_t(0));
	case ExpressionType::String:
	{
		llvm::Type* i8 = llvm::Type::getInt8Ty(llvmContext);
		llvm::ArrayType* arrayType = llvm::ArrayType::get(i8, 1);
		llvm::AllocaInst* allocaInst = builder.CreateAlloca(arrayType, nullptr, "str_alloc");

		// Создаем константый массив на стеке
		std::vector<llvm::Constant*> constants = { llvm::ConstantInt::get(llvm::Type::getInt8Ty(llvmContext), uint64_t(0)) };
		llvm::Constant* arr = llvm::ConstantArray::get(arrayType, constants);

		llvm::StoreInst* storeInst = builder.CreateStore(arr, allocaInst);
		(void)storeInst;

		return builder.CreateBitCast(allocaInst, llvm::Type::getInt8PtrTy(llvmContext), "str_to_i8_ptr");
	}
	default:
		assert(false);
		throw std::logic_error("can't codegen default value for undefined ExpressionType");
	}
}

class ContextScopeHelper
{
public:
	explicit ContextScopeHelper(CodegenContext& context)
		: m_context(context)
	{
		m_context.PushScope();
	}

	~ContextScopeHelper()
	{
		m_context.PopScope();
	}

private:
	CodegenContext& m_context;
};

llvm::Value* EmitPrintfStringLiteral(llvm::Value* value, llvm::LLVMContext& llvmContext, llvm::IRBuilder<>& builder)
{
	const auto getStringTypeSpecifier = [](const ExpressionType& type) {
		switch (type.value)
		{
		case ExpressionType::Int:
		case ExpressionType::Bool:
			return "%d";
		case ExpressionType::Float:
			return "%f";
		case ExpressionType::String:
			return "%s";
		}
		throw std::runtime_error("can't emit printf string argument for undefined expression type");
	};

	const ExpressionType type = ToExpressionType(value->getType());
	const std::string specifier = getStringTypeSpecifier(type);

	llvm::Type* i8 = llvm::Type::getInt8Ty(llvmContext);
	llvm::Constant* constantString = llvm::ConstantDataArray::getString(llvmContext, specifier, true);
	llvm::ArrayType* arrayType = llvm::ArrayType::get(i8, specifier.length() + 1);

	llvm::AllocaInst* allocaInst = builder.CreateAlloca(arrayType,
		llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), uint64_t(specifier.length() + 1)), "str_alloc");
	llvm::StoreInst* storeInst = builder.CreateStore(constantString, allocaInst);

	return builder.CreateBitCast(allocaInst, llvm::Type::getInt8PtrTy(llvmContext), "str_to_i8_ptr");
}
}


// Expression codegen visitor
ExpressionCodegen::ExpressionCodegen(CodegenContext& context)
	: m_context(context)
	, m_stack()
{
}

llvm::Value* ExpressionCodegen::Visit(const IExpressionAST& node)
{
	node.Accept(*this);
	if (!m_stack.empty())
	{
		llvm::Value* value = m_stack.back();
		m_stack.pop_back();
		return value;
	}
	throw std::logic_error("internal error while generating code for expression");
}

void ExpressionCodegen::Visit(const BinaryExpressionAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();

	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();

	llvm::Value* left = Visit(node.GetLeft());
	llvm::Value* right = Visit(node.GetRight());

	// Есть два варианта действий при посещении узла бинарного оператора:
	//  1. Посмотреть тип левого операнда, и привести правый операнд к этому же типу,
	//     далее смотреть на оператор и генерироровать код в зависимости от оператора и от типа обеих частей выражения
	//  2. Посмотреть на тип правого и левого операнда. По определенному приоритету, выполнить преобразование
	//     обеих частей выражения в один тип. Далее генерировать код, в зависимости от оператора и от типа (используется этот вариант)

	if (ToExpressionType(left->getType()) != ToExpressionType(right->getType()))
	{
		const auto castType = GetPreferredType(
			ToExpressionType(left->getType()),
			ToExpressionType(right->getType()));

		if (!castType)
		{
			const auto fmt = boost::format("can't codegen operator '%1%' on operands with types '%2%' and '%3%'")
				% ToString(node.GetOperator())
				% ToString(ToExpressionType(left->getType()))
				% ToString(ToExpressionType(right->getType()));
			throw std::runtime_error(fmt.str());
		}

		switch (castType->value)
		{
		case ExpressionType::Int:
			left = ConvertToIntegerValue(left, llvmContext, builder);
			right = ConvertToIntegerValue(right, llvmContext, builder);
			break;
		case ExpressionType::Float:
			left = ConvertToFloatValue(left, llvmContext, builder);
			right = ConvertToFloatValue(right, llvmContext, builder);
			break;
		case ExpressionType::Bool:
			left = ConvertToBooleanValue(left, llvmContext, builder);
			right = ConvertToBooleanValue(right, llvmContext, builder);
			break;
		case ExpressionType::String:
			throw std::runtime_error("can't codegen binary operator for string");
		default:
			throw std::logic_error("can't codegen binary operator for undefined expression type");
		}

		// TODO: produce warning here
	}

	assert(ToExpressionType(left->getType()) == ToExpressionType(right->getType()));

	switch (ToExpressionType(left->getType()).value)
	{
	case ExpressionType::Int:
		m_stack.push_back(CreateIntegerBinaryExpression(left, right, node.GetOperator(), llvmContext, builder));
		break;
	case ExpressionType::Float:
		m_stack.push_back(CreateFloatBinaryExpression(left, right, node.GetOperator(), llvmContext, builder));
		break;
	case ExpressionType::Bool:
		m_stack.push_back(CreateBooleanBinaryExpression(left, right, node.GetOperator(), llvmContext, builder));
		break;
	default:
		throw std::runtime_error("can't codegen binary operator '" +
			ToString(node.GetOperator()) + "' for " + ToString(ToExpressionType(left->getType())));
	}
}

void ExpressionCodegen::Visit(const LiteralExpressionAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();

	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	const LiteralExpressionAST::Value& constant = node.GetValue();

	// TODO: use boost::static_visitor
	if (constant.type() == typeid(int))
	{
		const int number = boost::get<int>(constant);
		llvm::Value* value = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), number);
		m_stack.push_back(value);
	}
	else if (constant.type() == typeid(double))
	{
		const double number = boost::get<double>(constant);
		llvm::Value* value = llvm::ConstantFP::get(llvm::Type::getDoubleTy(llvmContext), number);
		m_stack.push_back(value);
	}
	else if (constant.type() == typeid(bool))
	{
		const bool boolean = boost::get<bool>(constant);
		llvm::Value* value = llvm::ConstantInt::get(llvm::Type::getInt1Ty(llvmContext), uint64_t(boolean));
		m_stack.push_back(value);
	}
	else if (constant.type() == typeid(std::string))
	{
		const std::string& str = boost::get<std::string>(constant);
		llvm::Type* i8 = llvm::Type::getInt8Ty(llvmContext);
		llvm::Constant* constantString = llvm::ConstantDataArray::getString(llvmContext, str, true);
		llvm::ArrayType* arrayType = llvm::ArrayType::get(i8, str.length() + 1);

		llvm::AllocaInst* allocaInst = builder.CreateAlloca(arrayType,
			llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvmContext), uint64_t(str.length() + 1)), "str_alloc");
		llvm::StoreInst* storeInst = builder.CreateStore(constantString, allocaInst);
		(void)storeInst;

		m_stack.push_back(builder.CreateBitCast(allocaInst, llvm::Type::getInt8PtrTy(llvmContext), "str_to_i8_ptr"));
	}
	else
	{
		assert(false);
		throw std::logic_error("Visiting LiteralConstantAST - can't codegen for undefined literal expression ast");
	}
}

void ExpressionCodegen::Visit(const UnaryExpressionAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();
	llvm::Value* value = Visit(node.GetExpression());

	switch (node.GetOperator())
	{
	case UnaryExpressionAST::Plus:
		m_stack.push_back(value);
		break;
	case UnaryExpressionAST::Minus:
		m_stack.push_back(CodegenNegativeValue(value, builder));
		break;
	case UnaryExpressionAST::Negation:
		m_stack.push_back(CreateValueNegation(value, llvmContext, builder));
		break;
	default:
		assert(false);
		throw std::logic_error("Visit(UnaryAST): undefined unary operator");
	}
}

void ExpressionCodegen::Visit(const IdentifierExpressionAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();

	const std::string& name = node.GetName();
	llvm::AllocaInst* variable = m_context.GetVariable(name);

	if (!variable)
	{
		throw std::runtime_error("variable '" + name + "' is not defined");
	}

	llvm::Value* value = builder.CreateLoad(variable, name + "Value");
	m_stack.push_back(value);
}


// Statement codegen visitor
StatementCodegen::StatementCodegen(CodegenContext& context)
	: m_context(context)
	, m_expressionCodegen(context)
{
}

void StatementCodegen::Visit(const IStatementAST& node)
{
	node.Accept(*this);
}

void StatementCodegen::Visit(const VariableDeclarationAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();

	const std::string& name = node.GetIdentifier().GetName();
	if (m_context.GetVariable(name))
	{
		throw std::runtime_error("variable '" + name + "' is already defined");
	}

	// Создаем переменную на стеке и получаем указатель на нее, сохраненный в 'variable'
	llvm::Type* type = ToLLVMType(node.GetType(), llvmContext);
	llvm::AllocaInst* variable = builder.CreateAlloca(type, nullptr, name + "Ptr");

	// Устанавливаем переменной значение по умолчанию
	llvm::Value* defaultValue = CreateDefaultValue(node.GetType(), llvmContext, builder);
	builder.CreateStore(defaultValue, variable);

	// Сохраняем переменную в контекст (таблицу символов)
	m_context.Define(name, variable);

	// Обработка опционального блока присваивания
	if (const IExpressionAST* expression = node.GetExpression())
	{
		llvm::Value* value = m_expressionCodegen.Visit(*expression);

		if (ToExpressionType(value->getType()) != node.GetType())
		{
			llvm::Value* casted = CastValue(value, node.GetType(), llvmContext, builder);
			if (!casted)
			{
				auto fmt = boost::format("can't set expression of type '%1%' to variable '%2%' of type '%3%'")
					% ToString(ToExpressionType(value->getType()))
					% name
					% ToString(node.GetType());
				throw std::runtime_error(fmt.str());
			}

			// TODO: produce warning here
			assert(casted->getType()->getTypeID() == variable->getType()->getPointerElementType()->getTypeID());
			builder.CreateStore(casted, variable);
			return;
		}

		assert(value->getType()->getTypeID() == variable->getType()->getPointerElementType()->getTypeID());
		builder.CreateStore(value, variable);
	}
}

void StatementCodegen::Visit(const AssignStatementAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();

	const std::string& name = node.GetIdentifier().GetName();
	llvm::AllocaInst* variable = m_context.GetVariable(name);
	if (!variable)
	{
		throw std::runtime_error("can't assign because variable '" + name + "' is not defined");
	}

	llvm::Value* value = m_expressionCodegen.Visit(node.GetExpression());

	if (ToExpressionType(value->getType()) != ToExpressionType(variable->getType()->getPointerElementType()))
	{
		llvm::Value* casted = CastValue(value, ToExpressionType(variable->getType()->getPointerElementType()), llvmContext, builder);
		if (!casted)
		{
			auto fmt = boost::format("can't set expression of type '%1%' to variable '%2%' of type '%3%'")
				% ToString(ToExpressionType(value->getType()))
				% name
				% ToString(ToExpressionType(variable->getType()->getPointerElementType()));
			throw std::runtime_error(fmt.str());
		}

		// TODO: produce warning here
		assert(casted->getType()->getTypeID() == variable->getType()->getPointerElementType()->getTypeID());
		builder.CreateStore(casted, variable);
		return;
	}

	assert(value->getType()->getTypeID() == variable->getType()->getPointerElementType()->getTypeID());
	builder.CreateStore(value, variable);
}

void StatementCodegen::Visit(const IfStatementAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();

	llvm::Function* func = builder.GetInsertBlock()->getParent();

	// Создаем три базовых блока, каждый из которых должен завершиться терминальной инструкцией
	llvm::BasicBlock* thenBlock = llvm::BasicBlock::Create(llvmContext, "then", func);
	llvm::BasicBlock* elseBlock = llvm::BasicBlock::Create(llvmContext, "else", func);
	llvm::BasicBlock* continueBlock = llvm::BasicBlock::Create(llvmContext, "continue_after_if", func);

	// Генерируем код условного выражения
	llvm::Value* value = m_expressionCodegen.Visit(node.GetExpression());
	value = ConvertToBooleanValue(value, llvmContext, builder);
	builder.CreateCondBr(value, thenBlock, elseBlock);

	auto putBrAfterBranchInsertionIfNecessary = [&](llvm::BasicBlock* branch) {
		// Если блок не имеет выхода, добавляем безусловный переход
		if (!branch->getTerminator())
		{
			builder.CreateBr(continueBlock);
			return;
		}
		// Если блок имеет вложенные if инструкции, и при этом блок continue не имеет выхода,
		//  добавляем безусловный переход
		if (!m_branchContinueStack.empty())
		{
			const bool hasTerminated = m_branchContinueStack.back()->getTerminator();
			m_branchContinueStack.pop_back();
			if (!hasTerminated)
			{
				builder.CreateBr(continueBlock);
			}
		}
	};

	// Генерируем код для блока then. В конце блока then добавляем безусловный переход на блок continue,
	//  если блок then не был прерван какой-либо инструкцией
	{
		ContextScopeHelper scope(m_context);
		builder.SetInsertPoint(thenBlock);
		Visit(node.GetThenStatement());
		putBrAfterBranchInsertionIfNecessary(thenBlock);
	}

	// Генерируем код для блока else. В конце блока else добавляем безусловный переход на блок continue,
	//  если блок else не был прерван какой-либо инструкцией
	{
		ContextScopeHelper scope(m_context);
		builder.SetInsertPoint(elseBlock);
		if (node.GetElseStatement())
		{
			Visit(*node.GetElseStatement());
		}
		putBrAfterBranchInsertionIfNecessary(elseBlock);
	}

	// Устанавливаем точку для вставки генерируемого кода в блок continue_after_if,
	//  сохраняем блок continue в стеке для корректной вставки переходов во вложенных инструкциях if, while
	builder.SetInsertPoint(continueBlock);
	m_branchContinueStack.push_back(continueBlock);
}

void StatementCodegen::Visit(const WhileStatementAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::IRBuilder<>& builder = utils.GetBuilder();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();

	llvm::Function* func = builder.GetInsertBlock()->getParent();

	// Создаем два базовых блока, каждый из которых должен завершиться терминальной инструкцией
	llvm::BasicBlock* body = llvm::BasicBlock::Create(llvmContext, "loop_body", func);
	llvm::BasicBlock* afterLoop = llvm::BasicBlock::Create(llvmContext, "after_loop_body", func);

	// Генерируем код условия цикла
	llvm::Value* value = ConvertToBooleanValue(m_expressionCodegen.Visit(node.GetExpression()), llvmContext, builder);
	builder.CreateCondBr(value, body, afterLoop);

	// Генерируем код для тела цикла
	{
		ContextScopeHelper scope(m_context);
		builder.SetInsertPoint(body);
		Visit(node.GetStatement());

		// Если последний блок из тела цикла не имеет терминальной инструкции,
		//  добаляем условный переход на начало тела цикла (таким образом реализуется цикличность)
		if (!body->getTerminator())
		{
			value = ConvertToBooleanValue(m_expressionCodegen.Visit(node.GetExpression()), llvmContext, builder);
			builder.CreateCondBr(value, body, afterLoop);
		}
		// Иначе, если стек меток if-while не пуст, и последний элемент стека не имеет терминальной инструкции,
		//  тогда добавляем условный переход на начало тела цикла (таким образом реализуется цикличность)
		else if (!m_branchContinueStack.empty())
		{
			if (!m_branchContinueStack.back()->getTerminator())
			{
				builder.SetInsertPoint(m_branchContinueStack.back());
				value = ConvertToBooleanValue(m_expressionCodegen.Visit(node.GetExpression()), llvmContext, builder);
				builder.CreateCondBr(value, body, afterLoop);
			}
			m_branchContinueStack.pop_back();
		}
	}

	// Устанавливаем точку для вставки генерируемого кода в блок after_loop_body,
	//  сохраняем блок continue в стеке для корректной вставки переходов во вложенных инструкциях if, while
	builder.SetInsertPoint(afterLoop);
	m_branchContinueStack.push_back(afterLoop);
}

void StatementCodegen::Visit(const CompositeStatementAST& node)
{
	ContextScopeHelper scopedContext(m_context);
	llvm::IRBuilder<>& builder = m_context.GetUtils().GetBuilder();

	for (size_t i = 0; i < node.GetStatementsCount(); ++i)
	{
		node.GetStatement(i).Accept(*this);
		if (builder.GetInsertBlock()->getTerminator())
		{
			break;
		}
		// TODO: produce warning about unreachable code
	}
}

void StatementCodegen::Visit(const PrintStatementAST& node)
{
	CodegenUtils& utils = m_context.GetUtils();
	llvm::LLVMContext& llvmContext = utils.GetLLVMContext();
	llvm::IRBuilder<>& builder = utils.GetBuilder();

	llvm::Value* expressionValue = m_expressionCodegen.Visit(node.GetExpression());
	std::vector<llvm::Value*> printfArgs = {
		EmitPrintfStringLiteral(expressionValue, llvmContext, builder), expressionValue };
	builder.CreateCall(m_context.GetPrintf(), printfArgs, "print_tmp");
}

Codegen::Codegen(CodegenContext& context)
	: m_context(context)
{
}

void Codegen::Generate(const IStatementAST& statement)
{
	llvm::LLVMContext& llvmContext = m_context.GetUtils().GetLLVMContext();
	llvm::Module& llvmModule = m_context.GetUtils().GetModule();
	llvm::IRBuilder<>& builder = m_context.GetUtils().GetBuilder();

	// Генерируем код функции i32 main(void)
	llvm::FunctionType* mainFuncType = llvm::FunctionType::get(
		llvm::Type::getInt32Ty(llvmContext), llvm::ArrayRef<llvm::Type*>(), false);
	llvm::Function* mainFunc = llvm::Function::Create(
		mainFuncType, llvm::Function::ExternalLinkage, "main", &llvmModule);

	// Генерируем код для инструкции (или набора инструкций) внутри функции main
	llvm::BasicBlock* bb = llvm::BasicBlock::Create(llvmContext, "entry", mainFunc);
	builder.SetInsertPoint(bb);

	StatementCodegen statementCodegen(m_context);
	statement.Accept(statementCodegen);

	// Генерируем код инструкции возврата функции main
	llvm::Value* exitCode = llvm::ConstantInt::get(llvmContext, llvm::APInt(32, uint64_t(0), true));
	builder.CreateRet(exitCode);

	// Проверка на корректность сгенерированного кода
	if (llvm::verifyFunction(*mainFunc))
	{
		mainFunc->eraseFromParent();
		throw std::runtime_error("main function is not verified");
	}
}

void Codegen::Generate(const IExpressionAST& expression)
{
	llvm::LLVMContext& llvmContext = m_context.GetUtils().GetLLVMContext();
	llvm::Module& llvmModule = m_context.GetUtils().GetModule();
	llvm::IRBuilder<>& builder = m_context.GetUtils().GetBuilder();

	// Генерируем код функции i32 main(void)
	llvm::FunctionType* mainFuncType = llvm::FunctionType::get(
		llvm::Type::getInt32Ty(llvmContext), llvm::ArrayRef<llvm::Type*>(), false);
	llvm::Function* mainFunc = llvm::Function::Create(
		mainFuncType, llvm::Function::ExternalLinkage, "main", &llvmModule);

	// Создаем базовый блок, внутри которого произойдет вызов функции printf
	llvm::BasicBlock* bb = llvm::BasicBlock::Create(llvmContext, "entry", mainFunc);
	builder.SetInsertPoint(bb);

	// Генерируем код выражения
	ExpressionCodegen expressionCodegen(m_context);
	llvm::Value* value = expressionCodegen.Visit(expression);

	// Генерируем код вызова функции printf
	std::vector<llvm::Value*> printfArgs = { EmitPrintfStringLiteral(value, llvmContext, builder), value };
	builder.CreateCall(m_context.GetPrintf(), printfArgs);

	// Генерируем код инструкции возврата функции main
	llvm::Value* exitCode = llvm::ConstantInt::get(llvmContext, llvm::APInt(32, uint64_t(0), true));
	builder.CreateRet(exitCode);

	// Проверка на корректность сгенерированного кода
	if (llvm::verifyFunction(*mainFunc))
	{
		mainFunc->eraseFromParent();
		throw std::runtime_error("main function is not verified");
	}
}
