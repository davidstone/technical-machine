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

#include "../invalid_simulator_conversion.hpp"

#include "../../pokemon_online/conversion.hpp"
#include "../../ability.hpp"
#include "../../move.hpp"
#include "../../nature.hpp"
#include "../../pokemon.hpp"
#include "../../species.hpp"

namespace technicalmachine {
namespace po {
namespace {

void test_ability () {
	std::cerr << "\tVerifying correct ability.\n";
	for (Ability::Abilities original = static_cast <Ability::Abilities> (0); original != Ability::END; original = static_cast <Ability::Abilities> (original + 1)) {
		int const id = ability_to_id (original);
		Ability::Abilities const result = id_to_ability (id);
		if (original != result)
			throw InvalidSimulatorConversion <Ability> (original, result);
	}
}

void test_item () {
	std::cerr << "\tVerifying correct item.\n";
	for (Item::Items original = static_cast <Item::Items> (0); original != Item::END; original = static_cast <Item::Items> (original + 1)) {
		int const id = item_to_id (original);
		Item::Items const result = id_to_item (id);
		if (original != result and id != 0)
			throw InvalidSimulatorConversion <Item> (original, result);
	}
}

void test_move () {
	std::cerr << "\tVerifying correct move.\n";
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
	std::cerr << "\tVerifying correct nature.\n";
	for (Nature::Natures original = static_cast <Nature::Natures> (0); original != Nature::END; original = static_cast <Nature::Natures> (original + 1)) {
		int const id = nature_to_id (original);
		Nature::Natures const result = id_to_nature (id);
		if (original != result)
			throw InvalidSimulatorConversion <Nature> (original, result);
	}
}

void test_species () {
	std::cerr << "\tVerifying correct species.\n";
	for (Species original = static_cast <Species> (0); original != Species::END; original = static_cast <Species> (original + 1)) {
		std::pair <uint16_t, uint8_t> const ids = species_to_id (original);
		Species const result = id_to_species (ids.first, ids.second);
		if (original != result)
			throw InvalidSimulatorConversion <Pokemon> (original, result);
	}
}

}	// anonymous namespace

void test () {
	std::cerr << "Running Pokemon Online tests.\n";
	test_species ();
	test_ability ();
	test_item ();
	test_move ();
	test_nature ();
}

}	// namespace po
}	// namespace technicalmachine
