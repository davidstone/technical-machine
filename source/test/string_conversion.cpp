// Test string conversions
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

#include "string_conversion.hpp"

#include <iostream>
#include <string>

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../status.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

#include "../string_conversions/conversion.hpp"
#include "../string_conversions/invalid_string_conversion.hpp"

namespace technicalmachine {
namespace {

template <class Class, typename Enum>
void test_generic (std::string const & thing) {
	std::cout << "\tVerifying correct " + thing + ".\n";
	for (auto original = static_cast<Enum>(0); original != Enum::END; original = static_cast<Enum>(static_cast<unsigned>(original) + 1)) {
		std::string const str = to_string(original);
		auto const result = from_string<Enum>(str);
		if (original != result)
			throw InvalidToStringConversion (original, result, str);
	}
}

template <>
void test_generic<Item, Item::Items> (std::string const & thing) {
	typedef Item Class;
	typedef Item::Items Enum;
	std::cout << "\tVerifying correct " + thing + ".\n";
	for (auto original = static_cast<Enum>(0); original != Enum::END; original = static_cast<Enum>(static_cast<unsigned>(original) + 1)) {
		std::string const str = Class::to_string (original);
		Enum const result = Class::from_string (str);
		if (original != result)
			throw InvalidToStringConversion (original, result, str);
	}
}

template <>
void test_generic<Move, Moves> (std::string const & thing) {
	typedef Move Class;
	typedef Moves Enum;
	std::cout << "\tVerifying correct " + thing + ".\n";
	for (auto original = static_cast<Enum>(0); original != Enum::END; original = static_cast<Enum>(static_cast<unsigned>(original) + 1)) {
		std::string const str = Class::to_string (original);
		Enum const result = Class::from_string (str);
		if (original != result)
			throw InvalidToStringConversion (original, result, str);
	}
}


}	// anonymous namespace

void string_conversion_tests () {
	std::cout << "Running string conversion tests.\n";
	test_generic <Ability, Ability::Abilities> ("ability");
	test_generic <Gender, Gender::Genders> ("gender");
	test_generic <Item, Item::Items> ("item");
	test_generic <Move, Moves> ("move");
	test_generic <Nature, Nature::Natures> ("nature");
	test_generic <Pokemon, Species> ("species");
	test_generic <Status, Status::Statuses> ("status");
	std::cout << "String conversion tests passed.\n\n";
}

}	// namespace technicalmachine
