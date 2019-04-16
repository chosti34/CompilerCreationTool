#include "pch.h"
#include "LanguageSerialization.h"

#include "../Grammar/Grammar.h"
#include "../Grammar/GrammarSymbol.h"
#include "../Grammar/GrammarProduction.h"

#include "../Lexer/Lexer.h"
#include "../Parser/Action.h"

#include <tinyxml2.h>


namespace
{
std::string ToElementAttribute(StateFlag flag)
{
	using namespace std::literals::string_literals;
	return "is"s + ToString(flag);
}

void EnsureNoErrors(tinyxml2::XMLError status, const std::string& message)
{
	if (status != tinyxml2::XML_SUCCESS)
	{
		throw std::runtime_error(message);
	}
}

void EnsureElementPtr(tinyxml2::XMLElement* elementPtr, const std::string& message)
{
	if (!elementPtr)
	{
		throw std::runtime_error(message);
	}
}
}

void SerializeLanguage(const std::string& filepath, const Language& language)
{
	if (!language.IsInitialized())
	{
		throw std::runtime_error("language is not initialized");
	}

	// Создаем корневой узел
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* root = doc.NewElement("Language");
	doc.InsertEndChild(root);

	// Сериализуем информацию о грамматике
	tinyxml2::XMLElement* grammarElement = doc.NewElement("Grammar");
	root->InsertEndChild(grammarElement);

	for (std::size_t row = 0; row < language.GetGrammar().GetProductionsCount(); ++row)
	{
		const grammarlib::IGrammarProduction& production = language.GetGrammar().GetProduction(row);
		tinyxml2::XMLElement* productionElement = doc.NewElement("Production");
		productionElement->SetAttribute("left", production.GetLeftPart().c_str());
		grammarElement->InsertEndChild(productionElement);

		for (std::size_t col = 0; col < production.GetSymbolsCount(); ++col)
		{
			const grammarlib::IGrammarSymbol& symbol = production.GetSymbol(col);
			tinyxml2::XMLElement* symbolElement = doc.NewElement("Symbol");
			symbolElement->SetAttribute("name", symbol.GetName().c_str());
			symbolElement->SetAttribute("type", ToString(symbol.GetType()).c_str());
			if (symbol.HasAttribute())
			{
				symbolElement->SetAttribute("attribute", symbol.GetAttribute()->c_str());
			}
			productionElement->InsertEndChild(symbolElement);
		}
	}

	// Сериализуем информацию о лексере
	tinyxml2::XMLNode* patternsElement = doc.NewElement("Patterns");
	root->InsertEndChild(patternsElement);

	const ILexer& lexer = language.GetLexer();
	for (std::size_t row = 0; row < lexer.GetPatternsCount(); ++row)
	{
		const TokenPattern& pattern = lexer.GetPattern(row);
		tinyxml2::XMLElement* patternElement = doc.NewElement("Pattern");
		patternElement->SetAttribute("name", pattern.GetName().c_str());
		patternElement->SetAttribute("origin", pattern.GetOrigin().c_str());
		patternElement->SetAttribute("isEnding", pattern.IsEnding());
		patternsElement->InsertEndChild(patternElement);
	}

	// Сериализуем информацию о действиях парсера
	tinyxml2::XMLNode* actionsElement = doc.NewElement("Actions");
	root->InsertAfterChild(patternsElement, actionsElement);

	const IParser<ParseResults>& parser = language.GetParser();
	for (std::size_t row = 0; row < parser.GetActionsCount(); ++row)
	{
		const IAction& action = parser.GetAction(row);
		tinyxml2::XMLElement* actionElement = doc.NewElement("Action");
		actionElement->SetAttribute("name", action.GetName().c_str());
		actionElement->SetAttribute("type", ToString(action.GetType()).c_str());
		if (!action.GetMessage().empty())
		{
			actionElement->SetAttribute("message", action.GetMessage().c_str());
		}
		actionsElement->InsertEndChild(actionElement);
	}

	// Сохраняем созданный DOM в файл
	const tinyxml2::XMLError status = doc.SaveFile(filepath.c_str());
	EnsureNoErrors(status, "can't' serialize language to file '" + filepath + "'");
}

void UnserializeLanguage(const std::string& filepath, Language& language)
{
	// Создаем DOM объект
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError status = doc.LoadFile(filepath.c_str());
	EnsureNoErrors(status, "can't open file '" + filepath + "'");

	tinyxml2::XMLNode* root = doc.FirstChild();
	tinyxml2::XMLElement* grammarElement = root->FirstChildElement("Grammar");
	EnsureElementPtr(grammarElement, "no grammar section found");

	// Создаем грамматику
	auto grammar = std::make_unique<grammarlib::Grammar>();
	tinyxml2::XMLElement* productionElement = grammarElement->FirstChildElement("Production");
	EnsureElementPtr(productionElement, "grammar must have at least one production");

	while (productionElement)
	{
		const char* left;
		status = productionElement->QueryStringAttribute("left", &left);
		EnsureNoErrors(status, "production has no 'left' attribute");

		std::vector<std::unique_ptr<grammarlib::IGrammarSymbol>> right;
		tinyxml2::XMLElement* symbolElement = productionElement->FirstChildElement("Symbol");
		while (symbolElement)
		{
			const char* name;
			status = symbolElement->QueryStringAttribute("name", &name);
			EnsureNoErrors(status, "symbol has no 'name' attribute");

			const char* type;
			status = symbolElement->QueryStringAttribute("type", &type);
			EnsureNoErrors(status, "symbol has no 'type' attribute");

			boost::optional<std::string> attribute;
			const char* attributeValue;

			if (symbolElement->QueryStringAttribute("attribute", &attributeValue) != tinyxml2::XML_NO_ATTRIBUTE)
			{
				assert(attributeValue);
				attribute = std::string(attributeValue);
			}

			right.emplace_back(std::make_unique<grammarlib::GrammarSymbol>(name, ToGrammarSymbolType(type), attribute));
			symbolElement = symbolElement->NextSiblingElement("Symbol");
		}

		grammar->AddProduction(std::make_unique<grammarlib::GrammarProduction>(left, std::move(right)));
		productionElement = productionElement->NextSiblingElement("Production");
	}

	// Нормализуем индексы
	grammar->NormalizeIndices();

	// Считываем и сохраняем шаблоны разбора для лексера
	std::vector<TokenPattern> patterns;
	tinyxml2::XMLElement* patternsElement = root->FirstChildElement("Patterns");
	EnsureElementPtr(patternsElement, "no 'Patterns' element found");

	tinyxml2::XMLElement* patternElement = patternsElement->FirstChildElement("Pattern");
	EnsureElementPtr(patternElement, "lexer must have at least one pattern");

	while (patternElement)
	{
		const char* name;
		status = patternElement->QueryStringAttribute("name", &name);
		EnsureNoErrors(status, "pattern element must have 'name' attribute");

		const char* origin;
		status = patternElement->QueryStringAttribute("origin", &origin);
		EnsureNoErrors(status, "pattern element must have 'origin' attribute");

		bool isEnding;
		status = patternElement->QueryBoolAttribute("isEnding", &isEnding);
		EnsureNoErrors(status, "pattern element must have 'isEnding' attribute");

		patterns.emplace_back(name, origin, isEnding);
		patternElement = patternElement->NextSiblingElement("Pattern");
	}

	// Считываем и сохраняем действия парсера
	std::vector<std::unique_ptr<IAction>> actions;
	tinyxml2::XMLElement* actionsElement = root->FirstChildElement("Actions");
	EnsureElementPtr(actionsElement, "no 'Actions' element");

	tinyxml2::XMLElement* actionElement = actionsElement->FirstChildElement("Action");
	while (actionElement)
	{
		const char* name;
		status = actionElement->QueryStringAttribute("name", &name);
		EnsureNoErrors(status, "action element must have 'name' attribute");

		const char* type;
		status = actionElement->QueryStringAttribute("type", &type);
		EnsureNoErrors(status, "action element must have 'type' attribute");

		const char* message;
		status = actionElement->QueryStringAttribute("message", &message);
		const std::string msg = (status == tinyxml2::XML_NO_ATTRIBUTE) ? "" : message;

		actions.push_back(std::make_unique<Action>(name, ToActionType(type), msg));
		actionElement = actionElement->NextSiblingElement("Action");
	}

	language.SetGrammar(std::move(grammar));
	assert(language.IsInitialized());
	language.GetLexer().SetPatterns(std::move(patterns));
	language.GetParser().SetActions(std::move(actions));
}
