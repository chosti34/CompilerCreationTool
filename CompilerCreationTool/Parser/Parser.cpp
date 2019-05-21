#include "stdafx.h"
#include "Parser.h"
#include "Action.h"
#include "ParserState.h"
#include "../Utils/string_utils.h"

#include <map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <boost/format.hpp>

using namespace std::literals::string_literals;

namespace
{
template <typename T>
T Pop(std::vector<T>& vect)
{
	auto value = std::move(vect.back());
	vect.pop_back();
	return std::move(value);
}

template <typename Derived, typename Base>
std::unique_ptr<Derived> DowncastUniquePtr(std::unique_ptr<Base> && base)
{
	std::unique_ptr<Derived> derived = nullptr;
	if (Derived* ptr = dynamic_cast<Derived*>(base.get()))
	{
		base.release();
		derived.reset(ptr);
		return std::move(derived);
	}
	return nullptr;
}

class ActionExecutor
{
public:
	ActionExecutor(const Token& token, IParserLogger* logger)
		: mCurrentToken(token)
		, mLogger(logger)
		, mHasErrors(false)
	{
	}

	std::unique_ptr<IExpressionAST> GetExpression()
	{
		if (!mHasErrors && mExpressionsStack.size() == 1 && mStatementsStack.empty())
		{
			return Pop(mExpressionsStack);
		}
		return nullptr;
	}

	std::unique_ptr<IStatementAST> GetStatement()
	{
		if (!mHasErrors && mStatementsStack.size() == 1 && mExpressionsStack.empty())
		{
			return Pop(mStatementsStack);
		}
		return nullptr;
	}

	void DoNoAction()
	{
	}

	void DoCreateIntegerNode()
	{
		try
		{
			int value = std::stoi(mCurrentToken.value);
			mExpressionsStack.push_back(std::make_unique<LiteralExpressionAST>(value));
		}
		catch (...)
		{
			mHasErrors = true;
			throw std::runtime_error("failed to create integer node from: '" + mCurrentToken.name + "'");
		}
	}

	void DoCreateFloatNode()
	{
		try
		{
			double value = std::stod(mCurrentToken.value);
			mExpressionsStack.push_back(std::make_unique<LiteralExpressionAST>(value));
		}
		catch (...)
		{
			mHasErrors = true;
			throw std::runtime_error("failed to create float node from: '" + mCurrentToken.name + "'");
		}
	}

	void DoCreateBinaryNode(BinaryExpressionAST::Operator op)
	{
		if (mExpressionsStack.size() < 2)
		{
			mHasErrors = true;
			throw std::runtime_error("expressions stack must contain atleast 2 elements");
		}
		auto right = Pop(mExpressionsStack);
		auto left = Pop(mExpressionsStack);
		mExpressionsStack.push_back(std::make_unique<BinaryExpressionAST>(std::move(left), std::move(right), op));
	}

	void DoCreateUnaryNode(UnaryExpressionAST::Operator op)
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("expressions stack must contain atleast 1 element");
		}
		auto expression = Pop(mExpressionsStack);
		mExpressionsStack.push_back(std::make_unique<UnaryExpressionAST>(std::move(expression), op));
	}

	void DoCreateIdentifierNode()
	{
		mExpressionsStack.push_back(std::make_unique<IdentifierExpressionAST>(mCurrentToken.value));
	}

	void DoCreateBooleanNode(bool value)
	{
		mExpressionsStack.push_back(std::make_unique<LiteralExpressionAST>(value));
	}

	void DoCreateStringLiteralNode()
	{
		mExpressionsStack.push_back(std::make_unique<LiteralExpressionAST>(mCurrentToken.value));
	}

	void DoCreateExpressionType(ExpressionType::Value value)
	{
		mExpressionTypesStack.push_back(ExpressionType(value));
	}

	void DoCreateVariableDeclarationNode()
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("expression stack must contain identifier");
		}

		if (mExpressionTypesStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("types stack must contain atleast one element");
		}

		auto identifier = DowncastUniquePtr<IdentifierExpressionAST>(Pop(mExpressionsStack));
		if (!identifier)
		{
			mHasErrors = true;
			throw std::runtime_error("expression stack must contain identifier");
		}

		auto type = Pop(mExpressionTypesStack);

		auto declaration = std::make_unique<VariableDeclarationAST>(std::move(identifier), type);
		if (mOptionalAssignExpression)
		{
			declaration->SetExpression(std::move(mOptionalAssignExpression));
			mOptionalAssignExpression = nullptr;
		}

		mStatementsStack.push_back(std::move(declaration));
	}

	void DoCreateOptionalAssignNode()
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("expression stack must contain atleast one element");
		}

		mOptionalAssignExpression = Pop(mExpressionsStack);
	}

	void DoCreateAssignNode()
	{
		if (mExpressionsStack.size() < 2)
		{
			mHasErrors = true;
			throw std::runtime_error("expression stack must contain atleast two elements");
		}

		auto expression = Pop(mExpressionsStack);
		auto identifier = DowncastUniquePtr<IdentifierExpressionAST>(Pop(mExpressionsStack));
		if (!identifier)
		{
			mHasErrors = true;
			throw std::runtime_error("assign statement requires identifier");
		}

		mStatementsStack.push_back(std::make_unique<AssignStatementAST>(std::move(identifier), std::move(expression)));
	}

	void DoCreateIfStatementNode()
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("if statement requires one expression in stack");
		}

		if (mStatementsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("if statement requires one statement in stack");
		}

		auto expression = Pop(mExpressionsStack);
		auto then = Pop(mStatementsStack);

		mStatementsStack.push_back(std::make_unique<IfStatementAST>(std::move(expression), std::move(then)));
	}

	void DoSaveOptionalElseClause()
	{
		if (mStatementsStack.size() < 2)
		{
			mHasErrors = true;
			throw std::runtime_error("optional else clause requires atleast two elements in statements stack");
		}

		auto elseClause = Pop(mStatementsStack);
		auto ifStatement = DowncastUniquePtr<IfStatementAST>(Pop(mStatementsStack));

		if (!ifStatement)
		{
			mHasErrors = true;
			throw std::runtime_error("optional else clause need place for insertion");
		}

		ifStatement->SetElseClause(std::move(elseClause));
		mStatementsStack.push_back(std::move(ifStatement));
	}

	void DoCreateWhileStatementNode()
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("while statement requires one statement in stack");
		}

		if (mStatementsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("while statement requires one expression in stack");
		}

		auto expression = Pop(mExpressionsStack);
		auto statement = Pop(mStatementsStack);

		mStatementsStack.push_back(std::make_unique<WhileStatementAST>(std::move(expression), std::move(statement)));
	}

	void DoCreatePrintStatementNode()
	{
		if (mExpressionsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("print statement requires one expression in stack");
		}

		auto expression = Pop(mExpressionsStack);
		mStatementsStack.push_back(std::make_unique<PrintStatementAST>(std::move(expression)));
	}

	void DoAllocateNewComposite()
	{
		mCompositeStatements.emplace_back();
	}

	void DoOnCompositePartParse()
	{
		if (mStatementsStack.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("composite part requires atleast one statement");
		}

		if (mCompositeStatements.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("composite part requires allocated space for composite statement");
		}

		auto statement = Pop(mStatementsStack);
		mCompositeStatements.back().push_back(std::move(statement));
	}

	void DoMoveCompositeToStatement()
	{
		if (mCompositeStatements.empty())
		{
			mHasErrors = true;
			throw std::runtime_error("can't move composite because no space allocated for it");
		}

		auto composite = std::make_unique<CompositeStatementAST>();
		for (auto& statement : mCompositeStatements.back())
		{
			composite->AddStatement(std::move(statement));
		}
		mCompositeStatements.pop_back();
		mStatementsStack.push_back(std::move(composite));
	}

private:
	const Token& mCurrentToken;
	std::vector<std::unique_ptr<IExpressionAST>> mExpressionsStack;
	std::vector<std::unique_ptr<IStatementAST>> mStatementsStack;
	std::vector<std::vector<std::unique_ptr<IStatementAST>>> mCompositeStatements;
	std::unique_ptr<IExpressionAST> mOptionalAssignExpression;
	std::vector<ExpressionType> mExpressionTypesStack;
	IParserLogger* mLogger;
	bool mHasErrors;
};

template <typename T>
class ScopeHelper
{
public:
	ScopeHelper(T& valueRef, const T& valueOnDestroy)
		: mValueRef(valueRef)
		, mValueOnDestroy(valueOnDestroy)
	{
	}

	~ScopeHelper()
	{
		mValueRef = mValueOnDestroy;
	}

private:
	T& mValueRef;
	T mValueOnDestroy;
};
}

Parser::Parser(std::unique_ptr<IParserTable> && table, ILexer& lexer)
	: mTable(std::move(table))
	, mLexer(lexer)
	, mActionList()
	, mLogger(nullptr)
	, mRunTask(false)
{
}

ParseResults Parser::Parse(const std::string& text)
{
	mRunTask = true;
	ScopeHelper<bool> scopeHelper(mRunTask, false);

	size_t index = 0;
	std::vector<size_t> addresses;

	mLexer.SetText(text);
	Token token;

	ActionExecutor executor(token, mLogger.get());
	const std::map<ActionType, std::function<void()>> cActionMap = {
		{ ActionType::None, std::bind(&ActionExecutor::DoNoAction, &executor) },
		{ ActionType::CreateIntegerNumberNode, std::bind(&ActionExecutor::DoCreateIntegerNode, &executor) },
		{ ActionType::CreateFloatNumberNode, std::bind(&ActionExecutor::DoCreateFloatNode, &executor) },
		{ ActionType::CreateBinaryPlusNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Plus) },
		{ ActionType::CreateBinaryMinusNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Minus) },
		{ ActionType::CreateBinaryMulNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Mul) },
		{ ActionType::CreateBinaryDivNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Div) },
		{ ActionType::CreateBinaryLessNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Less) },
		{ ActionType::CreateBinaryEqualsNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Equals) },
		{ ActionType::CreateBinaryOrNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::Or) },
		{ ActionType::CreateBinaryAndNode, std::bind(&ActionExecutor::DoCreateBinaryNode, &executor, BinaryExpressionAST::And) },
		{ ActionType::CreateUnaryMinusNode, std::bind(&ActionExecutor::DoCreateUnaryNode, &executor, UnaryExpressionAST::Minus) },
		{ ActionType::CreateUnaryPlusNode, std::bind(&ActionExecutor::DoCreateUnaryNode, &executor, UnaryExpressionAST::Plus) },
		{ ActionType::CreateUnaryNegationNode, std::bind(&ActionExecutor::DoCreateUnaryNode, &executor, UnaryExpressionAST::Negation) },
		{ ActionType::CreateIdentifier, std::bind(&ActionExecutor::DoCreateIdentifierNode, &executor) },
		{ ActionType::CreateTrueNode, std::bind(&ActionExecutor::DoCreateBooleanNode, &executor, true) },
		{ ActionType::CreateFalseNode, std::bind(&ActionExecutor::DoCreateBooleanNode, &executor, false) },
		{ ActionType::CreateStringLiteralNode, std::bind(&ActionExecutor::DoCreateStringLiteralNode, &executor) },

		{ ActionType::CreateIntTypeNode, std::bind(&ActionExecutor::DoCreateExpressionType, &executor, ExpressionType::Int) },
		{ ActionType::CreateFloatTypeNode, std::bind(&ActionExecutor::DoCreateExpressionType, &executor, ExpressionType::Float) },
		{ ActionType::CreateBoolTypeNode, std::bind(&ActionExecutor::DoCreateExpressionType, &executor, ExpressionType::Bool) },
		{ ActionType::CreateStringTypeNode, std::bind(&ActionExecutor::DoCreateExpressionType, &executor, ExpressionType::String) },

		{ ActionType::CreateVariableDeclarationNode, std::bind(&ActionExecutor::DoCreateVariableDeclarationNode, &executor) },
		{ ActionType::SaveOptionalAssignExpression, std::bind(&ActionExecutor::DoCreateOptionalAssignNode, &executor) },
		{ ActionType::CreateAssignNode, std::bind(&ActionExecutor::DoCreateAssignNode, &executor) },
		{ ActionType::CreateIfStatementNode, std::bind(&ActionExecutor::DoCreateIfStatementNode, &executor) },
		{ ActionType::SaveOptionalElseStatement, std::bind(&ActionExecutor::DoSaveOptionalElseClause, &executor) },
		{ ActionType::CreateWhileStatementNode, std::bind(&ActionExecutor::DoCreateWhileStatementNode, &executor) },
		{ ActionType::CreatePrintStatementNode, std::bind(&ActionExecutor::DoCreatePrintStatementNode, &executor) },
		{ ActionType::AllocateNewComposite, std::bind(&ActionExecutor::DoAllocateNewComposite, &executor) },
		{ ActionType::OnCompositePartParse, std::bind(&ActionExecutor::DoOnCompositePartParse, &executor) },
		{ ActionType::MoveCompositeToStatement, std::bind(&ActionExecutor::DoMoveCompositeToStatement, &executor) }
	};

	try
	{
		token = FetchNextToken(index);
	}
	catch (const std::exception& ex)
	{
		LogIfNotNull("Lexer error: "s + ex.what(), index);
		return { false, nullptr, nullptr, ex.what() };
	}

	while (true)
	{
		// some condition checking that we need to stop the task
		{
			if (!mRunTask)
			{
				break;
			}
		}

		const IParserState& state = mTable->GetState(index);

		if (state.GetFlag(StateFlag::Attribute))
		{
			auto found = FindActionIndexByName(state.GetName());
			assert(found.is_initialized());

			const IAction& action = *mActionList[*found];
			const ActionType actionType = action.GetType();

			auto it = cActionMap.find(actionType);
			assert(it != cActionMap.end());
			auto& func = it->second;

			try
			{
				LogIfNotNull("Performing action '" + state.GetName() + "'", index);
				func();
				if (!action.GetMessage().empty())
				{
					LogIfNotNull(action.GetMessage(), index, IParserLogger::Action);
				}
			}
			catch (const std::exception& ex)
			{
				LogIfNotNull("Action error: "s + ex.what());
			}
		}
		else if (!state.AcceptsTerminal(token.name))
		{
			if (state.GetFlag(StateFlag::Error))
			{
				LogIfNotNull("Parser doesn't accept token '" +
					token.name + "' at error state", index);
				const std::string acceptables = string_utils::JoinStrings(
					state.GetAcceptableTerminals(), ", ", "[", "]");
				LogIfNotNull("List of acceptable tokens: " + acceptables);
				return { false, nullptr, nullptr,
					"parser doesn't accept token '" + token.name +
					"' at state '" + state.GetName() + "' (#" + std::to_string(index) + ")" };
			}
			else
			{
				LogIfNotNull("Parser doesn't accept token '" +
					token.name + "', switching to alternative #" +
					std::to_string(index + 1) + "...", index);
				++index;
				continue;
			}
		}

		if (state.GetFlag(StateFlag::End))
		{
			assert(addresses.empty());
			return ParseResults{ true, executor.GetExpression(), executor.GetStatement() };
		}
		if (state.GetFlag(StateFlag::Push))
		{
			LogIfNotNull("Push index #" + std::to_string(index + 1) + " to the stack", index);
			addresses.push_back(index + 1);
		}
		if (state.GetFlag(StateFlag::Shift))
		{
			try
			{
				token = FetchNextToken(index);
			}
			catch (const std::exception& ex)
			{
				LogIfNotNull("Lexer error: "s + ex.what(), index);
				return { false, nullptr, nullptr, ex.what() };
			}
		}

		if (auto next = state.GetNextAddress())
		{
			LogIfNotNull("Changing state index to #" + std::to_string(*next), index);
			index = *next;
		}
		else
		{
			assert(!addresses.empty());
			index = addresses.back();
			addresses.pop_back();
			LogIfNotNull("Pop index #" + std::to_string(index) + " from the stack", index);
		}
	}

	std::cout << "Parse cancelled" << std::endl;
	return ParseResults{ false, nullptr, nullptr, "parse cancelled", true };
}

void Parser::CancelParseTask()
{
	mRunTask = false;
}

bool Parser::IsParseTaskRunning() const
{
	return mRunTask;
}

const IParserTable& Parser::GetTable() const
{
	return *mTable;
}

void Parser::SetActionNames(const std::vector<std::string>& actions)
{
	for (const std::string& name : actions)
	{
		mActionList.emplace_back(std::make_unique<Action>(name, ActionType::None));
	}
}

void Parser::SetAction(size_t index, std::unique_ptr<IAction> && action)
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	mActionList[index] = std::move(action);
}

void Parser::SetActions(std::vector<std::unique_ptr<IAction>> && actions)
{
	mActionList = std::move(actions);
}

void Parser::SwapActions(size_t oldPos, size_t newPos)
{
	std::iter_swap(mActionList.begin() + oldPos, mActionList.begin() + newPos);
}

boost::optional<size_t> Parser::GetActionPos(const std::string& name) const
{
	for (std::size_t index = 0; index < mActionList.size(); ++index)
	{
		const IAction& action = *mActionList[index];
		if (action.GetName() == name)
		{
			return boost::make_optional(index);
		}
	}
	return boost::none;
}

const IAction& Parser::GetAction(size_t index) const
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *mActionList[index];
}

IAction& Parser::GetAction(size_t index)
{
	if (index >= mActionList.size())
	{
		throw std::out_of_range("index must be less than actions count");
	}
	return *mActionList[index];
}

size_t Parser::GetActionsCount() const
{
	return mActionList.size();
}

void Parser::SetLogger(std::unique_ptr<IParserLogger> && logger)
{
	mLogger = std::move(logger);
}

const IParserLogger* Parser::GetLogger() const
{
	return mLogger.get();
}

IParserLogger* Parser::GetLogger()
{
	return mLogger.get();
}

Token Parser::FetchNextToken(size_t currentStateIndex)
{
	const Token token = mLexer.GetNextToken();
	const auto fmt = boost::format("Token (%1%%2%) read by lexer")
		% token.name
		% (token.value != token.name && !token.value.empty() ? (", \"" + token.value + "\"") : "");
	LogIfNotNull(fmt.str(), currentStateIndex);
	return token;
}

boost::optional<size_t> Parser::FindActionIndexByName(const std::string& name)
{
	for (size_t i = 0; i < mActionList.size(); ++i)
	{
		const auto& pAction = mActionList[i];
		if (pAction->GetName() == name)
		{
			return boost::make_optional(i);
		}
	}
	return boost::none;
}

void Parser::LogIfNotNull(
	const std::string& message,
	boost::optional<size_t> state,
	IParserLogger::MessageCategory category,
	bool newline
)
{
	if (!mLogger)
	{
		return;
	}
	mLogger->Log((state ? "[#" + std::to_string(*state) + "] " : "") + message + (newline ? "\n" : ""), category);
}
