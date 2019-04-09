#include "pch.h"
#include "LanguageSerialization.h"
#include "../Grammar/GrammarBuilder.h"
#include "../Parser/ParserTable.h"
#include "../Parser/ParserState.h"
#include "../Lexer/Lexer.h"
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
}

void SerializeLanguage(const std::string& filepath, const Language& language)
{
	if (!language.IsInitialized())
	{
		throw std::runtime_error("can't serialize uninitialized language");
	}

	// Создаем корневой узел
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* root = doc.NewElement("Language");
	doc.InsertFirstChild(root);

	// Сериализуем информацию о лексере
	tinyxml2::XMLNode* patternsElement = doc.NewElement("Patterns");
	root->InsertFirstChild(patternsElement);

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
		actionsElement->InsertEndChild(actionElement);
	}

	// Сериализуем информацию о состояниях парсера
	/*tinyxml2::XMLNode* statesElement = doc.NewElement("States");
	root->InsertAfterChild(actionsElement, statesElement);

	for (std::size_t row = 0; row < parser.GetTable().GetStatesCount(); ++row)
	{
		const IParserState& state = parser.GetTable().GetState(row);
		tinyxml2::XMLElement* stateElement = doc.NewElement("State");
		stateElement->SetAttribute("name", state.GetName().c_str());

		for (StateFlag flag : GetAllStateFlags())
		{
			stateElement->SetAttribute(ToElementAttribute(flag).c_str(), state.GetFlag(flag));
		}
		if (auto next = state.GetNextAddress())
		{
			stateElement->SetAttribute("next", int64_t(*next));
		}
		for (const auto& terminal : state.GetAcceptableTerminals())
		{
			tinyxml2::XMLElement* acceptableElement = doc.NewElement("Acceptable");
			acceptableElement->SetText(terminal.c_str());
			stateElement->InsertEndChild(acceptableElement);
		}

		statesElement->InsertEndChild(stateElement);
	}*/

	// Сериализуем информацию о грамматике
	tinyxml2::XMLElement* grammarElement = doc.NewElement("Grammar");
	root->InsertAfterChild(actionsElement, grammarElement);

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

	// Сохраняем созданный DOM в файл
	const tinyxml2::XMLError status = doc.SaveFile(filepath.c_str());
	EnsureNoErrors(status, "can't' serialize language to file '" + filepath + "'");
}

std::unique_ptr<Language> UnserializeLanguage(const std::string& filepath)
{
	// Создаем DOM объект
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError status = doc.LoadFile(filepath.c_str());
	EnsureNoErrors(status, "can't open file '" + filepath + "' to read language");

	tinyxml2::XMLNode* root = doc.FirstChild();
	tinyxml2::XMLElement* grammarElement = root->FirstChildElement("Grammar");
	if (!grammarElement)
	{
		throw std::runtime_error("can't unserialize language: no grammar section");
	}

	const char* text = grammarElement->GetText();
	if (!text)
	{
		throw std::runtime_error("can't unserialize language: grammar section is corrupted");
	}

	auto language = std::make_unique<Language>();
	auto builder = std::make_unique<grammarlib::GrammarBuilder>();
	language->SetGrammar(builder->CreateGrammar(text));

	tinyxml2::XMLElement* parserElement = root->FirstChildElement("Parser");
	if (!parserElement)
	{
		throw std::runtime_error("can't unserialize language: no parser section");
	}

	tinyxml2::XMLElement* statesElement = parserElement->FirstChildElement("States");
	if (!statesElement)
	{
		throw std::runtime_error("can't unserialize language: no states section");
	}

	auto table = std::make_unique<ParserTable>();

	tinyxml2::XMLElement* stateElement = statesElement->FirstChildElement("State");
	while (stateElement)
	{
		auto state = std::make_unique<ParserState>();
		for (StateFlag flag : GetAllStateFlags())
		{
			bool value;
			tinyxml2::XMLError status = stateElement->QueryBoolAttribute(ToElementAttribute(flag).c_str(), &value);
			EnsureNoErrors(status, "can't unserialize language: corruped state");
			state->SetFlag(flag, value);
		}

		const char* name;
		tinyxml2::XMLError status = stateElement->QueryStringAttribute("name", &name);
		EnsureNoErrors(status, "can't unserialize language: corruped state");
		state->SetName(name);

		if (const tinyxml2::XMLAttribute* attribute = stateElement->FindAttribute("next"))
		{
			int64_t value;
			status = attribute->QueryInt64Value(&value);
			EnsureNoErrors(status, "can't unserialize language: corruped state");
			state->SetNextAddress(size_t(value));
		}

		std::set<std::string> acceptables;
		tinyxml2::XMLElement* acceptableElement = stateElement->FirstChildElement("Acceptable");

		while (acceptableElement)
		{
			const char* acceptable = acceptableElement->GetText();
			if (!acceptable)
			{
				throw std::runtime_error("can't unserialize language: corrupted acceptable");
			}
			acceptables.insert(acceptable);
			acceptableElement = acceptableElement->NextSiblingElement("Acceptable");
		}

		state->SetAcceptableTerminals(acceptables);
		table->AddState(std::move(state));
		stateElement = stateElement->NextSiblingElement("State");
	}

	auto lexer = std::make_unique<Lexer>();

	return language;
}
