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
bool verify (Original original, Result result, std::string str) {
	bool passed = true;
	if (original != result) {
		std::cerr << "\t\t" << original << " is seen as " << result << ".\n";
		std::cerr << "\t\tIntermediate string: " + str + "\n";
		passed = false;
	}
	return passed;
}

bool test_ability () {
	std::cerr << "\tVerifying correct ability.\n";
	bool passed = true;
	for (Ability::Abilities ability = static_cast <Ability::Abilities> (0); ability != Ability::END_ABILITY; ability = static_cast <Ability::Abilities> (ability + 1)) {
		std::string const name = Ability::to_string (ability);
		Ability::Abilities const result = Ability::from_string (name);
		passed &= verify (ability, result, name);
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

bool test_item () {
	std::cerr << "\tVerifying correct item.\n";
	bool passed = true;
	for (Item::Items item = static_cast <Item::Items> (0); item != Item::END_ITEM; item = static_cast <Item::Items> (item + 1)) {
		std::string const name = Item::to_string (item);
		Item::Items const result = Item::from_string (name);
		passed &= verify (item, result, name);
	}
	return passed;
}

bool test_move () {
	std::cerr << "\tVerifying correct move.\n";
	bool passed = true;
	for (Move::Moves move = static_cast <Move::Moves> (0); move != Move::END_MOVE; move = static_cast <Move::Moves> (move + 1)) {
		std::string const name = Move::to_string (move);
		Move::Moves const result = Move::from_string (name);
		passed &= verify (move, result, name);
	}
	return passed;
}

bool test_nature () {
	std::cerr << "\tVerifying correct nature.\n";
	bool passed = true;
	for (Nature::Natures nature = static_cast <Nature::Natures> (0); nature != Nature::END_NATURE; nature = static_cast <Nature::Natures> (nature + 1)) {
		std::string const name = Nature::to_string (nature);
		Nature::Natures const result = Nature::from_string (name);
		passed &= verify (nature, result, name);
	}
	return passed;
}

bool test_species () {
	std::cerr << "\tVerifying correct species.\n";
	bool passed = true;
	for (Species species = static_cast <Species> (0); species != END_SPECIES; species = static_cast <Species> (species + 1)) {
		std::string const name = Pokemon::to_string (species);
		Species const result = Pokemon::from_string (name);
		passed &= verify (species, result, name);
	}
	return passed;
}

bool test_status () {
	std::cerr << "\tVerifying correct status.\n";
	bool passed = true;
	for (Status::Statuses status = static_cast <Status::Statuses> (0); status != Status::END_STATUS; status = static_cast <Status::Statuses> (status + 1)) {
		std::string const name = Status::to_string (status);
		Status::Statuses const result = Status::from_string (name);
		passed &= verify (status, result, name);
	}
	return passed;
}

}	// anonymous namespace

bool string_conversion_tests () {
	std::cerr << "Running string conversion tests.\n";
	bool result = true;
	result &= test_ability ();
	result &= test_gender ();
	result &= test_item ();
	result &= test_move ();
	result &= test_nature ();
	result &= test_species ();
	result &= test_status ();
	return result;
}

}	// namespace technicalmachine
