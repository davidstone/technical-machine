// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.string_conversions.team;

import tm.string_conversions.generation;
import tm.string_conversions.pokemon_to_string;

import tm.any_team;
import tm.generation;
import tm.team;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

template<any_team TeamType>
constexpr auto to_string_impl(TeamType const & team) -> containers::string {
	auto output = containers::concatenate<containers::string>(
		"Generation "sv,
		to_string(generation_from<TeamType>),
		" team:\n"sv
	);
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::array{'\n'}
		);
	}
	return output;
}

#define CREATE_OVERLOADS(generation) \
	auto to_string(Team<generation> const & team) -> containers::string { \
		return to_string_impl(team); \
	} \
	auto to_string(KnownTeam<generation> const & team) -> containers::string { \
		return to_string_impl(team); \
	} \
	auto to_string(SeenTeam<generation> const & team) -> containers::string { \
		return to_string_impl(team); \
	}

TM_FOR_EACH_GENERATION(CREATE_OVERLOADS);

} // namespace technicalmachine
