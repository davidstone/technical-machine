// Test PO conversions
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

#include "conversion.hpp"

#include <iostream>

#include "../invalid_simulator_conversion.hpp"

#include "../../ability.hpp"
#include "../../enum_range.hpp"

#include "../../move/is_switch.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

#include "../../stat/nature.hpp"

#include "../../clients/pokemon_online/conversion.hpp"

namespace technicalmachine {
namespace po {
namespace {

template<typename Enum, typename ToID, typename FromID>
void test_enum(std::string const & name, ToID && to_id, FromID && from_id) {
	std::cout << "\t\tVerifying correct " << name << ".\n";
	for (auto const original : enum_range<Enum>) {
		auto const id = to_id(original);
		auto const result = from_id(id);
		if (original != result) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_item() {
	std::cout << "\t\tVerifying correct item.\n";
	for (auto const original : enum_range<Item>) {
		auto const id = item_to_id(original);
		auto const result = id_to_item(id);
		if (original != result and id != 0) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_move() {
	std::cout << "\t\tVerifying correct move.\n";
	for(auto const original : enum_range<Moves>) {
		if (!is_switch(original) and original != Moves::Hit_Self) {
			auto const id = move_to_id(original);
			auto const result = id_to_move(id);
			if (original != result) {
				throw InvalidSimulatorConversion(original, result);
			}
		}
	}
}

void test_species() {
	std::cout << "\t\tVerifying correct species.\n";
	for (auto const original : enum_range<Species>) {
		if (original == Species::Victini) {
			break;
		}
		auto const ids = species_to_id(original);
		auto const result = id_to_species(ids);
		if (original != result) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

}	// namespace

void test_conversions () {
	std::cout << "\tRunning Pokemon Online conversion tests.\n";
	test_enum<Ability::Abilities>("Ability", ability_to_id, id_to_ability);
	test_enum<Gender>("Gender", gender_to_id, id_to_gender);
	test_item();
	test_move();
	test_enum<Nature>("Nature", nature_to_id, id_to_nature);
	test_species();
}

}	// namespace po
}	// namespace technicalmachine
