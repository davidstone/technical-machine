// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.server.predictor_inputs;

import tm.move.move_names;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.species;

import tm.stat.evs;
import tm.stat.nature;

import tm.team_predictor.server.style;

import tm.ability;
import tm.generation;
import tm.item;

import containers;
import tv;

namespace technicalmachine {

export struct PredictorPokemon {
	Species species;
	Level level;
	tv::optional<Item> item;
	tv::optional<Ability> ability;
	tv::optional<Nature> nature;
	tv::optional<EVs> evs;
	MoveNames moves;
};

export using PredictorTeam = containers::static_vector<PredictorPokemon, max_pokemon_per_team>;

export struct PredictorInputs {
	Generation generation;
	Style style;
	PredictorTeam team;
};

} // namespace technicalmachine
