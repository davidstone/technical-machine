module;

#include <tm/for_each_generation.hpp>

export module tm.string_conversions.team;

import tm.team;

import containers;

namespace technicalmachine {

#define CREATE_OVERLOAD(TeamType) \
	export auto to_string( \
		TeamType const & team, \
		bool include_active_marker \
	) -> containers::string

#define CREATE_OVERLOADS(generation) \
	CREATE_OVERLOAD(Team<generation>); \
	CREATE_OVERLOAD(KnownTeam<generation>); \
	CREATE_OVERLOAD(SeenTeam<generation>)

TM_FOR_EACH_GENERATION(CREATE_OVERLOADS);

} // namespace technicalmachine
