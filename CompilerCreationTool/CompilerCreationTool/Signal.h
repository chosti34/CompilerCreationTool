#pragma once
#include <boost/signals2.hpp>

template <typename Signature>
using Signal = boost::signals2::signal<Signature>;

using SignalConnection = boost::signals2::connection;
using SignalScopedConnection = boost::signals2::scoped_connection;
