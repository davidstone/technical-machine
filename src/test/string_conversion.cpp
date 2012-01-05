// Test string conversions
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

#include <iostream>
#include <string>

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../move.hpp"
#include "../nature.hpp"
#include "../pokemon.hpp"

namespace technicalmachine {
namespace {

template <typename Original, typename Result>
bool verify (Original original, Result result, std::string const & str) {
	bool passed = true;
	if (original != result) {
		std::cerr << "\t\t" << original << " is seen as " << result << ".\n";
		std::cerr << "\t\tIntermediate string: " + str + "\n";
		passed = false;
	}
	return passed;
}

template <class Class, typename Enum>
bool test_generic (std::string const & thing) {
	std::cerr << "\tVerifying correct " + thing + "\n";
	bool passed = true;
	for (Enum original = static_cast <Enum> (0); original != Enum::END; original = static_cast <Enum> (original + 1)) {
		std::string const str = Class::to_string (original);
		Enum const result = Class::from_string (str);
		passed &= verify (original, result, str);
	}
	return passed;
}

bool test_gender () {
	std::cerr << "\tVerifying correct gender.\n";
	bool passed = true;
	for (Gender::Genders gender = Gender::FEMALE; gender <= Gender::MALE; gender = static_cast <Gender::Genders> (gender + 1)) {
		std::string const name = Gender::to_string (gender);
		Gender::Genders const result = Gender::from_string (name);
		passed &= verify (gender, result, name);
	}
	return passed;
}

}	// anonymous namespace

bool string_conversion_tests () {
	std::cerr << "Running string conversion tests.\n";
	bool result = true;
	result &= test_generic <Ability, Ability::Abilities> ("ability");
	result &= test_gender ();
	result &= test_generic <Item, Item::Items> ("item");
	result &= test_generic <Move, Move::Moves> ("move");
	result &= test_generic <Nature, Nature::Natures> ("nature");
	result &= test_generic <Pokemon, Species> ("species");
	result &= test_generic <Status, Status::Statuses> ("status");
	return result;
}

}	// namespace technicalmachine
