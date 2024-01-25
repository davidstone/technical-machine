// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.legal_selections;

import tm.move.action;
import tm.move.max_moves_per_pokemon;

import tm.pokemon.max_pokemon_per_team;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

// The possible selection sets are:
// Pass
// Team size (6) - 1 switches
// Struggle, team size (6) - 1 switches
// Normal moves (4), team size (6) - 1 switches
export constexpr auto maximum_possible_selections = max_moves_per_pokemon + max_pokemon_per_team - 1_bi;

export using LegalSelections = containers::static_vector<
	Action,
	maximum_possible_selections
>;

} // namespace technicalmachine
