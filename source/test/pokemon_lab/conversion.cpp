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
#include "../../clients/pokemon_lab/id.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/pokemon.hpp"
#include "../../pokemon/species.hpp"

namespace technicalmachine {
namespace pl {
namespace {

void test_ability () {
	// Abilities are sent as strings, so no ID conversion required.
}

template<typename Enum>
void test_generic(std::string const & type, Enum const last) {
	std::cout << "\t\tVerifying correct " << type << ".\n";
	for (Enum original = static_cast<Enum>(0); original != last; original = static_cast<Enum>(static_cast<unsigned>(original) + 1)) {
		auto const id = simulator_cast<ID<Enum>>(original);
		auto const result = simulator_cast<Enum>(id);
		if (original != result) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_item () {
	// Items are sent as strings, so no ID conversion required.
}

void test_nature () {
	// Natures are sent the same as TM, so no conversion required.
}

}	// anonymous namespace

void test_conversions () {
	std::cout << "\tRunning Pokemon Lab conversion tests.\n";
	test_ability ();
	test_generic("Gender", Gender::Genders::END);
	test_item ();
	test_generic("Move", Moves::END);
	test_nature ();
	test_generic("Species", Species::Generation_4_End);
}

}	// namespace pl
}	// namespace technicalmachine
