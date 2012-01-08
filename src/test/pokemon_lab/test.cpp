// Test PL stuff
// Copyright (C) 2011 David Stone
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

#include "test.hpp"

#include <iostream>

#include "../invalid_simulator_conversion.hpp"

#include "../../pokemon_lab/conversion.hpp"
#include "../../pokemon.hpp"
#include "../../species.hpp"

namespace technicalmachine {
namespace pl {
namespace {

void test_ability () {
	// Abilities are sent as strings, so no ID conversion required.
}

void test_gender () {
	std::cout << "\tVerifying correct gender.\n";
	for (Gender::Genders original = Gender::FEMALE; original <= Gender::MALE; original = static_cast <Gender::Genders> (original + 1)) {
		int const id = gender_to_id (original);
		Gender::Genders const result = id_to_gender (id);
		if (original != result)
			throw InvalidSimulatorConversion <Gender> (original, result);
	}
}

void test_item () {
	// Items are sent as strings, so no ID conversion required.
}

void test_move () {
	std::cout << "\tVerifying correct move.\n";
	for (Move::Moves original = static_cast <Move::Moves> (0); original != Move::END; original = static_cast <Move::Moves> (original + 1)) {
		if (!Move::is_switch (original)) {
			int const id = move_to_id (original);
			Move::Moves const result = id_to_move (id);
			if (original != result)
				throw InvalidSimulatorConversion <Move> (original, result);
		}
	}
}

void test_nature () {
	// Natures are sent the same as TM, so no conversion required.
}

void test_species () {
	std::cout << "\tVerifying correct species.\n";
	for (Species original = static_cast <Species> (0); original != Species::END; original = static_cast <Species> (original + 1)) {
		int const id = species_to_id (original);
		Species const result = id_to_species (id);
		if (original != result)
			throw InvalidSimulatorConversion <Pokemon> (original, result);
	}
}

void test_file () {
}

}	// anonymous namespace

void test () {
	std::cout << "Running Pokemon Lab tests.\n";
	test_ability ();
	test_gender ();
	test_item ();
	test_move ();
	test_nature ();
	test_species ();

	test_file ();
}

}	// namespace pl
}	// namespace technicalmachine
