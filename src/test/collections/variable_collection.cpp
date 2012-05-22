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

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

namespace technicalmachine {
namespace {
void test_zero_size_team();
void test_small_size_team();
void test_other_combinations(unsigned max_foe_size);
void phaze_in_same_pokemon(VariableCollection & collection, unsigned index);
void test_invalid_combinations(unsigned max_foe_size);
unsigned expected_index(unsigned current_index, unsigned new_index);

}	// unnamed namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	std::cout << "\t\tRunning variable collection tests on size zero containers.\n";
	test_zero_size_team();
	std::cout << "\t\tRunning variable collection tests on size one / two containers.\n";
	test_small_size_team();
	std::cout << "\t\tRunning variable collection tests on arbitrary size containers.\n";
	constexpr unsigned max_foe_size = 10;
	test_other_combinations(max_foe_size);
}

namespace {

void test_zero_size_team() {
	try {
		VariableCollection collection(Moves::WHIRLWIND, 0);
		throw InvalidCollection("Can construct a variable collection from a team of size 0.");
	}
	catch (EmptyTeam const & ex) {
		// Do nothing; the above operation should throw.
	}
}

void test_small_size_team() {
	for (unsigned const foe_size : { 1u, 2u }) {
		unsigned const current_index = 0;
		unsigned const new_index = 0;
		VariableCollection collection (Moves::WHIRLWIND, foe_size);
		collection.set_phaze_index(current_index, new_index);
		if (collection.index() != 0)
			throw InvalidCollection("Phazing index is not always 0 when the team size is " + std::to_string(foe_size) + ".");
	}
}

void test_other_combinations(unsigned const max_foe_size) {
	for (unsigned foe_size = 3; foe_size <= max_foe_size; ++foe_size) {
		VariableCollection collection (Moves::WHIRLWIND, foe_size);
		for (unsigned new_index = 0; new_index <= foe_size; ++new_index) {
			for (unsigned current_index = 0; current_index != foe_size; ++current_index) {
				if (current_index == new_index)
					phaze_in_same_pokemon(collection, new_index);
				else {
					try {
						collection.set_phaze_index(current_index, new_index);
						unsigned const expected = expected_index(current_index, new_index);
						if (collection().value() != expected)
							throw InvalidCollection("Offsets for phazing are incorrect. Expected " + std::to_string(expected) + " but got a result of " + std::to_string(static_cast<unsigned>(collection().value())) + ".");
						if (new_index == foe_size)
							throw InvalidCollection("Phazing supports too many elements when the foe's size is " + std::to_string(foe_size) + ".");
					}
					catch(InvalidCollectionIndex const & ex) {
						if (new_index != foe_size)
							throw InvalidCollection("Phazing does not support " + std::to_string(new_index) + " elements when the foe's size is " + std::to_string(foe_size) + ".");
						// otherwise, everything is good
					}
				}
			}
		}
	}
}

void phaze_in_same_pokemon(VariableCollection & collection, unsigned const index) {
	try {
		collection.set_phaze_index(index, index);
		throw InvalidCollection("Can phaze in the same Pokemon.");
	}
	catch (PhazingInSamePokemon const & ex) {
		// Do nothing; the above operation should throw.
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
