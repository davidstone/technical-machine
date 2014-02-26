// Test checked collections of random move effects
// Copyright (C) 2014 David Stone
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

#include "variable_collection.hpp"

#include <iostream>
#include <string>

#include "invalid_collection.hpp"

#include "../../empty_team.hpp"
#include "../../phazing_in_same_pokemon.hpp"
#include "../../team.hpp"
#include "../../variable.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/collection.hpp"
#include "../../pokemon/max_pokemon_per_team.hpp"
#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace {

void add_pokemon(Team & team, Species species);
void test_combinations(Team & team);
void phaze_in_same_pokemon(Variable & variable, Team const & team);
void phaze_in_different_pokemon(Variable & variable, Team const & team, PokemonCollection::index_type new_index, PokemonCollection::index_type current_index, TeamSize foe_size);

}	// namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	Team team;
	add_pokemon(team, static_cast<Species>(1));
	test_combinations(team);
}

namespace {

using bounded_integer::to_string;

void add_pokemon(Team & team, Species const species) {
	Level const level(100_bi);
	Gender const gender(Gender::MALE);
	team.add_pokemon(species, level, gender);
	team.pokemon().all_moves().add(Moves::Whirlwind);
}

void test_combinations(Team & team) {
	for (auto const foe_size : bounded_integer::range(2_bi, max_pokemon_per_team + 1_bi)) {
		add_pokemon(team, static_cast<Species>(foe_size));
		auto collection = all_probabilities(team.pokemon(), static_cast<unsigned>(foe_size));
		if (collection.size() != foe_size - 1_bi) {
			throw InvalidCollection("Phazing size is incorrect. Expected: " + to_string(foe_size - 1_bi) + " but got " + to_string(collection.size()));
		}
		for (auto const new_index : bounded_integer::range(foe_size)) {
			for (auto const current_index : bounded_integer::range(foe_size)) {
				team.all_pokemon().set_index(current_index);
				if (current_index == new_index) {
					phaze_in_same_pokemon(collection.front(), team);
				}
				else {
					phaze_in_different_pokemon(collection.front(), team, new_index, current_index, foe_size);
				}
			}
		}
	}
}

void phaze_in_same_pokemon(Variable & variable, Team const & team) {
	try {
		variable.set_phaze_index(team, team.pokemon());
		throw InvalidCollection("Can phaze in the same Pokemon.");
	}
	catch (PhazingInSamePokemon const &) {
		// Do nothing; the above operation should throw.
	}
}

void phaze_in_different_pokemon(Variable & variable, Team const & team, PokemonCollection::index_type const new_index, PokemonCollection::index_type const current_index, TeamSize foe_size) {
	static constexpr auto expected_index = bounded_integer::make_explicit_optional_array<6, 6>(
		bounded_integer::none, 0_bi, 1_bi, 2_bi, 3_bi, 4_bi,
		0_bi, bounded_integer::none, 1_bi, 2_bi, 3_bi, 4_bi,
		0_bi, 1_bi, bounded_integer::none, 2_bi, 3_bi, 4_bi,
		0_bi, 1_bi, 2_bi, bounded_integer::none, 3_bi, 4_bi,
		0_bi, 1_bi, 2_bi, 3_bi, bounded_integer::none, 4_bi,
		0_bi, 1_bi, 2_bi, 3_bi, 4_bi, bounded_integer::none
	);
	try {
		variable.set_phaze_index(team, team.pokemon(new_index));
		auto const expected = expected_index[current_index][new_index];
		if (variable.value() != expected)
			throw InvalidCollection("Offsets for phazing are incorrect. Expected " + to_string(expected.value()) + " but got a result of " + to_string(variable.value()) + ".");
		if (new_index == foe_size)
			throw InvalidCollection("Phazing supports too many elements when the foe's size is " + to_string(foe_size) + ".");
	}
	catch(InvalidCollectionIndex const &) {
		if (new_index != foe_size)
			throw InvalidCollection("Phazing does not support " + to_string(new_index) + " elements when the foe's size is " + to_string(foe_size) + ".");
		// otherwise, everything is good
	}
}

}	// namespace
}	// namespace technicalmachine
