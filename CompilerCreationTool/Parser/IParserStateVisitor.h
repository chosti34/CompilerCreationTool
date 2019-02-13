#pragma once

class ParserState;
class AttributeState;

class IParserStateVisitor
{
public:
	virtual ~IParserStateVisitor() = default;
	virtual void Visit(const ParserState& state) = 0;
	virtual void Visit(const AttributeState& state) = 0;
};
