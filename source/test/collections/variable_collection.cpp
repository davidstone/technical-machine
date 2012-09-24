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

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace {
void test_zero_size_team();
void test_small_size_team(Team & team);
void test_other_combinations(Team & team, unsigned max_foe_size);
void phaze_in_same_pokemon(VariableCollection & collection, Team const & team);
void phaze_in_different_pokemon(VariableCollection & collection, Team const & team, unsigned new_index, unsigned current_index, unsigned foe_size);
unsigned expected_index(unsigned current_index, unsigned new_index);

}	// unnamed namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	std::cout << "\t\tRunning variable collection tests on size zero containers.\n";
	test_zero_size_team();
	std::cout << "\t\tRunning variable collection tests on size one / two containers.\n";
	Team team;
	test_small_size_team(team);
	std::cout << "\t\tRunning variable collection tests on arbitrary size containers.\n";
	constexpr unsigned max_foe_size = 10;
	test_other_combinations(team, max_foe_size);
}

namespace {

void test_zero_size_team() {
}

void test_small_size_team(Team & team) {
	for (unsigned const foe_size : { 1u, 2u }) {
		constexpr unsigned level = 100;
		Gender const gender(Gender::MALE);
		Species const species = static_cast<Species>(foe_size);
		team.add_pokemon(species, level, gender);
		VariableCollection collection (Moves::WHIRLWIND, foe_size);
		collection.set_phaze_index(team, species);
		if (collection.index() != 0)
			throw InvalidCollection("Phazing index is not always 0 when the team size is " + std::to_string(foe_size) + ".");
	}
}

void test_other_combinations(Team & team, unsigned const max_foe_size) {
	constexpr unsigned level = 100;
	Gender const gender(Gender::MALE);
	for (unsigned foe_size = 3; foe_size <= max_foe_size; ++foe_size) {
		Species const species = static_cast<Species>(foe_size);
		team.add_pokemon(species, level, gender);
		VariableCollection collection (Moves::WHIRLWIND, foe_size);
		for (unsigned new_index = 0; new_index <= foe_size; ++new_index) {
			for (unsigned current_index = 0; current_index != foe_size; ++current_index) {
				team.all_pokemon().set_index(current_index);
				if (current_index == new_index)
					phaze_in_same_pokemon(collection, team);
				else
					phaze_in_different_pokemon(collection, team, new_index, current_index, foe_size);
			}
		}
	}
}

void phaze_in_same_pokemon(VariableCollection & collection, Team const & team) {
	try {
		collection.set_phaze_index(team, team.pokemon().name());
		throw InvalidCollection("Can phaze in the same Pokemon.");
	}
	catch (PhazingInSamePokemon const &) {
		// Do nothing; the above operation should throw.
	}
}

void phaze_in_different_pokemon(VariableCollection & collection, Team const & team, unsigned new_index, unsigned current_index, unsigned foe_size) {
	try {
		collection.set_phaze_index(team, team.pokemon(new_index).name());
		unsigned const expected = expected_index(current_index, new_index);
		if (collection().value() != expected)
			throw InvalidCollection("Offsets for phazing are incorrect. Expected " + std::to_string(expected) + " but got a result of " + std::to_string(static_cast<unsigned>(collection().value())) + ".");
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
