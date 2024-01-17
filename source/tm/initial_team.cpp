// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.initial_team;

import tm.pokemon.initial_pokemon;
import tm.pokemon.max_pokemon_per_team;

import tm.stat.stat_style;

import containers;
import tv;

namespace technicalmachine {

export template<SpecialStyle style>
using InitialTeam = containers::static_vector<InitialPokemon<style>, max_pokemon_per_team>;

export using AnyInitialTeam = tv::variant<
	InitialTeam<SpecialStyle::combined>,
	InitialTeam<SpecialStyle::split>
>;

} // namespace technicalmachine
