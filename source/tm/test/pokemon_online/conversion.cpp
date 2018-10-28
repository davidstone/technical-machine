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

#include <tm/test/pokemon_online/conversion.hpp>

#include <iostream>

#include <tm/test/pokemon_online/../invalid_simulator_conversion.hpp>

#include <tm/ability.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/nature.hpp>

#include <tm/clients/pokemon_online/conversion.hpp>

namespace technicalmachine {
namespace po {
namespace {

template<typename Enum, typename ToID, typename FromID>
void test_enum(std::string const & name, ToID && to_id, FromID && from_id) {
	std::cout << "\t\tVerifying correct " << name << ".\n";
	for (auto const original : containers::enum_range<Enum>()) {
		auto const id = to_id(original);
		auto const result = from_id(id);
		if (original != result) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_item() {
	std::cout << "\t\tVerifying correct item.\n";
	for (auto const original : containers::enum_range<Item>()) {
		auto const id = item_to_id(original);
		auto const result = id_to_item(id);
		if (original != result and id != 0) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_move() {
	std::cout << "\t\tVerifying correct move.\n";
	for(auto const original : containers::enum_range<Moves>()) {
		if (is_regular(original)) {
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
	for (auto const original : containers::enum_range<Species>()) {
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
	test_enum<Ability>("Ability", ability_to_id, id_to_ability);
	test_enum<Gender>("Gender", gender_to_id, id_to_gender);
	test_item();
	test_move();
	test_enum<Nature>("Nature", nature_to_id, id_to_nature);
	test_species();
}

}	// namespace po
}	// namespace technicalmachine
