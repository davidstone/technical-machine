// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <containers/begin_end.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

BOUNDED_COMMON_ARITHMETIC
using containers::begin;
using containers::end;

namespace detail {

using namespace bounded::literal;

BOUNDED_COMMON_ARITHMETIC
using containers::begin;
using containers::end;

} // namespace detail
} // namespace technicalmachine
