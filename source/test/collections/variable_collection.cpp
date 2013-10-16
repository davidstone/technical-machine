// Test checked collections of random move effects
// Copyright (C) 2012 David Stone
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

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace {
void add_pokemon(Team & team, Species species);
void test_combinations(Team & team, unsigned max_foe_size);
void phaze_in_same_pokemon(Variable & variable, Team const & team);
void phaze_in_different_pokemon(Variable & variable, Team const & team, unsigned new_index, unsigned current_index, unsigned foe_size);
unsigned expected_index(unsigned current_index, unsigned new_index);

}	// unnamed namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	Team team;
	add_pokemon(team, static_cast<Species>(1));
	constexpr unsigned max_foe_size = 10;
	test_combinations(team, max_foe_size);
}

namespace {

void add_pokemon(Team & team, Species const species) {
	constexpr unsigned level = 100;
	Gender const gender(Gender::MALE);
	team.add_pokemon(species, level, gender);
	team.pokemon().all_moves().add(Moves::Whirlwind);
}

void test_combinations(Team & team, unsigned const max_foe_size) {
	for (unsigned foe_size = 2; foe_size <= max_foe_size; ++foe_size) {
		auto const species = static_cast<Species>(foe_size);
		add_pokemon(team, species);
		auto collection = all_probabilities(team.pokemon(), foe_size);
		if (collection.size() != foe_size - 1) {
			throw InvalidCollection("Phazing size is incorrect. Expected: " + std::to_string(foe_size - 1) + " but got " + std::to_string(collection.size()));
		}
		for (unsigned new_index = 0; new_index <= foe_size; ++new_index) {
			for (unsigned current_index = 0; current_index != foe_size; ++current_index) {
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

void phaze_in_different_pokemon(Variable & variable, Team const & team, unsigned new_index, unsigned current_index, unsigned foe_size) {
	try {
		variable.set_phaze_index(team, team.pokemon(new_index));
		unsigned const expected = expected_index(current_index, new_index);
		if (variable.value() != expected)
			throw InvalidCollection("Offsets for phazing are incorrect. Expected " + std::to_string(expected) + " but got a result of " + std::to_string(variable.value()) + ".");
		if (new_index == foe_size)
			throw InvalidCollection("Phazing supports too many elements when the foe's size is " + std::to_string(foe_size) + ".");
	}
	catch(InvalidCollectionIndex const &) {
		if (new_index != foe_size)
			throw InvalidCollection("Phazing does not support " + std::to_string(new_index) + " elements when the foe's size is " + std::to_string(foe_size) + ".");
		// otherwise, everything is good
	}
}

unsigned expected_index(unsigned const current_index, unsigned const new_index) {
	constexpr unsigned X = 0xFF;
	constexpr unsigned lookup [10][10] = {
		{ X, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
		{ 0, X, 1, 2, 3, 4, 5, 6, 7, 8 },
		{ 0, 1, X, 2, 3, 4, 5, 6, 7, 8 },
		{ 0, 1, 2, X, 3, 4, 5, 6, 7, 8 },
		{ 0, 1, 2, 3, X, 4, 5, 6, 7, 8 },
		{ 0, 1, 2, 3, 4, X, 5, 6, 7, 8 },
		{ 0, 1, 2, 3, 4, 5, X, 6, 7, 8 },
		{ 0, 1, 2, 3, 4, 5, 6, X, 7, 8 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, X, 8 },
		{ 0, 1, 2, 3, 4, 5, 6, 7, 8, X },
	};
	return lookup[current_index][new_index];
}

}	// unnamed namespace
}	// namespace technicalmachine
