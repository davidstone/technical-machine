// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.string_conversions.team;

import tm.team;

import containers;

namespace technicalmachine {

#define CREATE_OVERLOADS(generation) \
	export auto to_string(Team<generation> const & team) -> containers::string; \
	export auto to_string(KnownTeam<generation> const & team) -> containers::string; \
	export auto to_string(SeenTeam<generation> const & team) -> containers::string

TM_FOR_EACH_GENERATION(CREATE_OVERLOADS);

} // namespace technicalmachine
