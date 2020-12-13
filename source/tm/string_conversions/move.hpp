// Move string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/conversion.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;

auto to_string(Moves name) -> std::string_view;

template<>
auto from_string<Moves>(std::string_view const str) -> Moves;

}	// namespace technicalmachine
