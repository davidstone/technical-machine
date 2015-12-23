// Random effects of moves
// Copyright (C) 2015 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "move/moves.hpp"
#include "pokemon/collection.hpp"
#include "pokemon/level.hpp"
#include "pokemon/species_forward.hpp"

#include <bounded/integer.hpp>

#include <containers/static_vector/static_vector.hpp>

#include <cstdint>

namespace technicalmachine {
struct ActivePokemon;
struct Team;

// Used for moves with a variable power / length / other integer range. Moves of
// variable power: Magnitude = 4-10, Psywave = 5-15, Present = 0-4 (0 = heal).
// It is also used to determine whether random effects activate.
struct Variable {
	using value_type = bounded::integer<0, 150>;
	using Probability = double;
	using Magnitude = bounded::checked_integer<4, 10>;

	value_type value;
	Probability probability;
};

using Probabilities = containers::static_vector<Variable, 101>;
auto all_probabilities(ActivePokemon pokemon, TeamSize foe_size) -> Probabilities const &;

// Team is the Team that was phazed, not the team that used the phazing move
auto set_phaze_index(Variable & variable, Team const & team, Species species) -> void;
auto set_flinch(Variable & variable, bool set = true) -> void;
auto effect_activates(Variable variable) -> bool;
constexpr auto phaze_index(Variable const variable, containers::index_type<PokemonCollection> const foe_index) {
	return (variable.value < foe_index) ?
		containers::index_type<PokemonCollection>(variable.value) :
		containers::index_type<PokemonCollection>(variable.value + 1_bi);
}
auto present_heals(Variable variable) -> bool;

inline auto psywave_damage(Variable const variable, Level const level) {
	return bounded::max(1_bi, level() * static_cast<bounded::integer<50, 150>>(variable.value) / 100_bi);
}
auto set_magnitude(Variable & variable, Variable::Magnitude magnitude) -> void;

}	// namespace technicalmachine
