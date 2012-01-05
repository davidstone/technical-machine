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

#include "../../pokemon_lab/conversion.hpp"
#include "../../ability.hpp"
#include "../../pokemon.hpp"
#include "../../species.hpp"

namespace technicalmachine {
namespace pl {
namespace {

bool test_ability () {
	// Abilities are sent as strings, so no ID conversion required.
	return true;
}

bool test_item () {
	// Items are sent as strings, so no ID conversion required.
	return true;
}

bool test_move () {
	std::cerr << "\tVerifying correct move.\n";
	bool passed = true;
	for (Move::Moves move = static_cast <Move::Moves> (0); move != Move::END; move = static_cast <Move::Moves> (move + 1)) {
		int const id = move_to_id (move);
		Move::Moves const result = id_to_move (id);
		if (move != result and !Move::is_switch (move)) {
			passed = false;
			std::cerr << "\t\t" + Move::to_string (move) + " is seen as " + Move::to_string (result) + ".\n";
		}
	}
	return passed;
}

bool test_nature () {
	// Natures are sent the same as TM, so no conversion required.
	return true;
}

bool test_species () {
	std::cerr << "\tVerifying correct species.\n";
	bool passed = true;
	for (Species species = static_cast <Species> (0); species != Species::END; species = static_cast <Species> (species + 1)) {
		int const id = species_to_id (species);
		Species const result = id_to_species (id);
		if (species != result) {
			passed = false;
			std::cerr << "\t\t" + Pokemon::to_string (species) + " is seen as " + Pokemon::to_string (result) + ".\n";
		}
	}
	return passed;
}

bool test_file () {
	bool passed = true;
	return passed;
}

}	// anonymous namespace

bool test () {
	std::cerr << "Testing Pokemon Lab.\n";
	bool passed = true;
	passed &= test_species ();
	passed &= test_ability ();
	passed &= test_item ();
	passed &= test_move ();
	passed &= test_nature ();
	passed &= test_file ();
	return passed;
}

}	// namespace pl
}	// namespace technicalmachine
