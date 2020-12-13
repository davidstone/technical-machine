// Ability string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/conversion.hpp>

#include <tm/ability.hpp>

#include <string_view>

namespace technicalmachine {

auto to_string(Ability const name) -> std::string_view;

template<>
auto from_string<Ability>(std::string_view const str) -> Ability;

}	// namespace technicalmachine
