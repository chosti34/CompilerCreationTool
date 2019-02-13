#pragma once
#include "IParserState.h"
#include <boost/noncopyable.hpp>
#include <memory>

class IParserTable : private boost::noncopyable
{
public:
	virtual ~IParserTable() = default;

	virtual void AddState(std::unique_ptr<IParserState> && state) = 0;
	virtual size_t GetStatesCount() const = 0;

	virtual IParserState& GetState(size_t index) = 0;
	virtual const IParserState& GetState(size_t index) const = 0;
};
