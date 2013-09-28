// Test PO conversions
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

#include "conversion.hpp"

#include <iostream>

#include "../invalid_simulator_conversion.hpp"

#include "../../ability.hpp"

#include "../../move/move.hpp"
#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

#include "../../stat/nature.hpp"

#include "../../clients/pokemon_online/conversion.hpp"

namespace technicalmachine {
namespace po {
namespace {
template<typename Enum, typename Function>
void for_each(Function && f, Enum const last = Enum::END) {
	for (Enum original = static_cast<Enum>(0); original != last; original = static_cast<Enum>(static_cast<unsigned>(original) + 1))
		f(original);
}

void test_ability () {
	std::cout << "\t\tVerifying correct ability.\n";
	for_each<Ability::Abilities>([](Ability::Abilities const original) {
		unsigned const id = ability_to_id (original);
		Ability::Abilities const result = id_to_ability (id);
		if (original != result)
			throw InvalidSimulatorConversion(original, result);
	});
}

void test_gender () {
	std::cout << "\t\tVerifying correct gender.\n";
	for_each<Gender::Genders>([](Gender::Genders const original) {
		unsigned const id = gender_to_id (original);
		Gender::Genders const result = id_to_gender (id);
		if (original != result)
			throw InvalidSimulatorConversion(original, result);
	});
}

void test_item () {
	std::cout << "\t\tVerifying correct item.\n";
	for_each<Item::Items>([](Item::Items const original) {
		unsigned const id = item_to_id (original);
		Item::Items const result = id_to_item (id);
		if (original != result and id != 0)
			throw InvalidSimulatorConversion(original, result);
	});
}

void test_move () {
	std::cout << "\t\tVerifying correct move.\n";
	for_each<Moves>([](Moves const original) {
		if (!is_switch(original) and original != Moves::Hit_Self) {
			unsigned const id = move_to_id (original);
			Moves const result = id_to_move (id);
			if (original != result)
				throw InvalidSimulatorConversion(original, result);
		}
	});
}

void test_nature () {
	std::cout << "\t\tVerifying correct nature.\n";
	for_each<Nature::Natures>([](Nature::Natures const original) {
		unsigned const id = nature_to_id (original);
		Nature::Natures const result = id_to_nature (id);
		if (original != result)
			throw InvalidSimulatorConversion(original, result);
	});
}

void test_species () {
	std::cout << "\t\tVerifying correct species.\n";
	auto const f = [](Species const original) {
		std::pair <uint16_t, uint8_t> const ids = species_to_id (original);
		Species const result = id_to_species (ids.first, ids.second);
		if (original != result)
			throw InvalidSimulatorConversion(original, result);
	};
	for_each<Species>(f, Species::Generation_4_End);
}

}	// anonymous namespace

void test_conversions () {
	std::cout << "\tRunning Pokemon Online conversion tests.\n";
	test_ability ();
	test_gender ();
	test_item ();
	test_move ();
	test_nature ();
	test_species ();
}

}	// namespace po
}	// namespace technicalmachine
