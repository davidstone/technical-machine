// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>
#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

using namespace bounded::literal;

template<typename T> requires std::is_enum_v<T>
inline constexpr auto number_of = bounded::constant<numeric_traits::max_value<T>> - bounded::constant<numeric_traits::min_value<T>> + 1_bi;

} // namespace technicalmachine
