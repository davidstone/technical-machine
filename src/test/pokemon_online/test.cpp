// Test PO stuff
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

#include "../../pokemon_online/conversion.hpp"
#include "../../ability.hpp"
#include "../../move.hpp"
#include "../../nature.hpp"
#include "../../pokemon.hpp"
#include "../../species.hpp"

namespace technicalmachine {
namespace po {
namespace {

bool test_ability () {
	std::cerr << "\tVerifying correct ability.\n";
	bool passed = true;
	for (Ability::Abilities ability = static_cast <Ability::Abilities> (0); ability != Ability::END; ability = static_cast <Ability::Abilities> (ability + 1)) {
		int const id = ability_to_id (ability);
		Ability::Abilities const result = id_to_ability (id);
		if (ability != result) {
			passed = false;
			std::cerr << "\t\t" + Ability::to_string (ability) + " is seen as " + Ability::to_string (result) + ".\n";
		}
	}
	return passed;
}

bool test_item () {
	std::cerr << "\tVerifying correct item.\n";
	bool passed = true;
	for (Item::Items item = static_cast <Item::Items> (0); item != Item::END; item = static_cast <Item::Items> (item + 1)) {
		int const id = item_to_id (item);
		Item::Items const result = id_to_item (id);
		if (item != result and result != Item::END and id != 0) {
			passed = false;
			std::cerr << "\t\t" + Item::to_string (item) + " is seen as " + Item::to_string (result) + ".\n";
		}
	}
	return passed;
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
	std::cerr << "\tVerifying correct nature.\n";
	bool passed = true;
	for (Nature::Natures nature = static_cast <Nature::Natures> (0); nature != Nature::END; nature = static_cast <Nature::Natures> (nature + 1)) {
		int const id = nature_to_id (nature);
		Nature::Natures const result = id_to_nature (id);
		if (nature != result) {
			passed = false;
			std::cerr << "\t\t" + Nature::to_string (nature) + " is seen as " + Nature::to_string (result) + ".\n";
		}
	}
	return passed;
}

bool test_species () {
	std::cerr << "\tVerifying correct species.\n";
	bool passed = true;
	for (Species species = static_cast <Species> (0); species != Species::END; species = static_cast <Species> (species + 1)) {
		std::pair <uint16_t, uint8_t> const ids = species_to_id (species);
		Species const result = id_to_species (ids.first, ids.second);
		if (species != result) {
			passed = false;
			std::cerr << "\t\t" + Pokemon::to_string (species) + " is seen as " + Pokemon::to_string (result) + ".\n";
		}
	}
	return passed;
}

}	// anonymous namespace

bool test () {
	std::cerr << "Testing Pokemon Online.\n";
	bool passed = true;
	passed &= test_species ();
	passed &= test_ability ();
	passed &= test_item ();
	passed &= test_move ();
	passed &= test_nature ();
	return passed;
}

}	// namespace po
}	// namespace technicalmachine
