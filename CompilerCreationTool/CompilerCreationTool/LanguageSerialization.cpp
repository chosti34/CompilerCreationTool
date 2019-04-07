#include "pch.h"
#include "LanguageSerialization.h"
#include "../Grammar/GrammarBuilder.h"
#include "../Parser/ParserTable.h"
#include "../Parser/ParserState.h"
#include "../Lexer/Lexer.h"
#include <tinyxml2.h>

using namespace std::literals::string_literals;

namespace
{
std::string ToElementAttribute(StateFlag flag)
{
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
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* root = doc.NewElement("Language");
	doc.InsertFirstChild(root);

	// Serializing lexer
	tinyxml2::XMLNode* lexerElement = doc.NewElement("Lexer");
	root->InsertFirstChild(lexerElement);

	tinyxml2::XMLNode* patternsElement = doc.NewElement("Patterns");
	lexerElement->InsertFirstChild(patternsElement);

	const ILexer& lexer = language.GetLexer();
	for (std::size_t i = 0; i < lexer.GetPatternsCount(); ++i)
	{
		const TokenPattern& pattern = lexer.GetPattern(i);
		tinyxml2::XMLElement* pElement = doc.NewElement("Pattern");
		pElement->SetAttribute("name", pattern.GetName().c_str());
		pElement->SetAttribute("origin", pattern.GetOrigin().c_str());
		pElement->SetAttribute("isEnding", pattern.IsEnding());
		patternsElement->InsertEndChild(pElement);
	}

	// Serializing parser
	tinyxml2::XMLNode* parserElement = doc.NewElement("Parser");
	root->InsertAfterChild(lexerElement, parserElement);

	tinyxml2::XMLNode* actionsElement = doc.NewElement("Actions");
	parserElement->InsertFirstChild(actionsElement);

	const IParser<ParseResults>& parser = language.GetParser();
	for (std::size_t i = 0; i < parser.GetActionsCount(); ++i)
	{
		const IAction& action = parser.GetAction(i);
		tinyxml2::XMLElement* pElement = doc.NewElement("Action");
		pElement->SetAttribute("name", action.GetName().c_str());
		pElement->SetAttribute("type", ToString(action.GetType()).c_str());
		actionsElement->InsertEndChild(pElement);
	}

	tinyxml2::XMLNode* statesElement = doc.NewElement("States");
	parserElement->InsertAfterChild(actionsElement, statesElement);

	const IParserTable& table = parser.GetTable();
	for (std::size_t i = 0; i < table.GetStatesCount(); ++i)
	{
		const IParserState& state = table.GetState(i);
		tinyxml2::XMLElement* pElement = doc.NewElement("State");

		pElement->SetAttribute("name", state.GetName().c_str());
		for (StateFlag flag : GetAllStateFlags())
		{
			pElement->SetAttribute(ToElementAttribute(flag).c_str(), state.GetFlag(flag));
		}

		if (auto next = state.GetNextAddress())
		{
			pElement->SetAttribute("next", int64_t(*next));
		}

		const auto& terminals = state.GetAcceptableTerminals();
		for (const auto& terminal : terminals)
		{
			tinyxml2::XMLElement* pAcceptableElement = doc.NewElement("Acceptable");
			pAcceptableElement->SetText(terminal.c_str());
			pElement->InsertEndChild(pAcceptableElement);
		}

		statesElement->InsertEndChild(pElement);
	}

	// Serializing grammar
	tinyxml2::XMLElement* grammarElement = doc.NewElement("Grammar");
	root->InsertAfterChild(parserElement, grammarElement);

	grammarElement->SetText(language.GetGrammar().GetText().c_str());

	if (doc.SaveFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		throw std::runtime_error("can't serialize language to file '" + filepath + "'");
	}
}

std::unique_ptr<Language> UnserializeLanguage(const std::string& filepath)
{
	using namespace grammarlib;

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
	{
		throw std::runtime_error("can't open file '" + filepath + "' to read language");
	}

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
	auto builder = std::make_unique<GrammarBuilder>();
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
