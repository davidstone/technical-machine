// Test PL conversions
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

#include "../../clients/pokemon_lab/conversion.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace pl {
namespace {

void test_ability () {
	// Abilities are sent as strings, so no ID conversion required.
}

void test_gender () {
	std::cout << "\t\tVerifying correct gender.\n";
	for (Gender::Genders original = static_cast <Gender::Genders> (0); original != Gender::END; original = static_cast <Gender::Genders> (original + 1)) {
		auto const id = simulator_cast<unsigned>(original);
		auto const result = simulator_cast<Gender::Genders>(id);
		if (original != result)
			throw InvalidSimulatorConversion <Gender> (original, result);
	}
}

void test_item () {
	// Items are sent as strings, so no ID conversion required.
}

void test_move () {
	std::cout << "\t\tVerifying correct move.\n";
	for (auto original = static_cast<Moves>(0); original != Moves::END; original = static_cast<Moves>(static_cast<unsigned>(original) + 1)) {
		if (!Move::is_switch (original)) {
			auto const id = simulator_cast<unsigned>(original);
			auto const result = simulator_cast<Moves>(id);
			if (original != result)
				throw InvalidSimulatorConversion <Move> (original, result);
		}
	}
}

void test_nature () {
	// Natures are sent the same as TM, so no conversion required.
}

void test_species () {
	std::cout << "\t\tVerifying correct species.\n";
	for (auto original = static_cast<Species>(0); original != Species::Generation_4_End; original = static_cast<Species>(static_cast<unsigned>(original) + 1)) {
		auto const id = simulator_cast<unsigned>(original);
		auto const result = simulator_cast<Species>(id);
		if (original != result)
			throw InvalidSimulatorConversion <Pokemon> (original, result);
	}
}

}	// anonymous namespace

void test_conversions () {
	std::cout << "\tRunning Pokemon Lab conversion tests.\n";
	test_ability ();
	test_gender ();
	test_item ();
	test_move ();
	test_nature ();
	test_species ();
}

}	// namespace pl
}	// namespace technicalmachine
