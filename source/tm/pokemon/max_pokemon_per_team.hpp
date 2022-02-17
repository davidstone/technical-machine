// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto max_pokemon_per_team = 6_bi;
using TeamSize = bounded::integer<
	0,
	bounded::normalize<max_pokemon_per_team>
>;
using TeamIndex = bounded::integer<
	0,
	bounded::normalize<max_pokemon_per_team - 1_bi>
>;

} // namespace technicalmachine
