// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.pokemon.potentially_selectable_moves;

import tm.pokemon.max_pokemon_per_team;

import tm.move.move;
import tm.move.move_name;
import tm.move.is_switch;
import tm.move.regular_moves;

import tm.pokemon.max_pokemon_per_team;

import tm.any_team;
import tm.generation;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto weird_moves(Generation const generation) {
	return containers::array({
		Move(generation, MoveName::Pass),
		Move(generation, MoveName::Struggle)
	});
}

constexpr auto switches(Generation const generation, TeamSize const team_size) {
	auto const switches_to_add = BOUNDED_CONDITIONAL(team_size != 1_bi, team_size, 0_bi);
	return containers::indexed_generate_n(switches_to_add, [=](auto const n) { return Move(generation, to_switch(n)); });
}

constexpr auto potentially_selectable_moves(Generation const generation, TeamSize const team_size, RegularMoves regular_moves) {
	return containers::concatenate_view(
		std::move(regular_moves),
		weird_moves(generation),
		switches(generation, team_size)
	);
}

export template<any_team TeamType>
constexpr auto potentially_selectable_moves(TeamType const & team) {
	return potentially_selectable_moves(generation_from<TeamType>, team.size(), team.pokemon().regular_moves());
}


constexpr auto make_all(TeamSize const team_size, auto... move_names) {
	constexpr auto generation = Generation::one;
	return potentially_selectable_moves(
		generation,
		team_size,
		RegularMoves({Move(generation, move_names)...})
	);
}

static_assert(containers::equal(
	make_all(1_bi, MoveName::Tackle),
	containers::array({
		MoveName::Tackle,
		MoveName::Pass,
		MoveName::Struggle
	})
));

static_assert(containers::equal(
	make_all(2_bi, MoveName::Tackle),
	containers::array({
		MoveName::Tackle,
		MoveName::Pass,
		MoveName::Struggle,
		MoveName::Switch0,
		MoveName::Switch1
	})
));

static_assert(containers::equal(
	make_all(6_bi, MoveName::Tackle, MoveName::Thunder, MoveName::Psychic, MoveName::Body_Slam),
	containers::array({
		MoveName::Tackle,
		MoveName::Thunder,
		MoveName::Psychic,
		MoveName::Body_Slam,
		MoveName::Pass,
		MoveName::Struggle,
		MoveName::Switch0,
		MoveName::Switch1,
		MoveName::Switch2,
		MoveName::Switch3,
		MoveName::Switch4,
		MoveName::Switch5
	})
));

} // namespace technicalmachine
