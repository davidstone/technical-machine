// Test string conversions
// Copyright (C) 2014 David Stone
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

#include "../move/moves.hpp"

#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

#include "../string_conversions/ability.hpp"
#include "../string_conversions/gender.hpp"
#include "../string_conversions/item.hpp"
#include "../string_conversions/move.hpp"
#include "../string_conversions/nature.hpp"
#include "../string_conversions/pokemon.hpp"
#include "../string_conversions/status.hpp"
#include "../string_conversions/invalid_string_conversion.hpp"

namespace technicalmachine {
namespace {

template <typename Enum>
void test_generic (std::string const & thing) {
	std::cout << "\tVerifying correct " + thing + ".\n";
	for (auto original = static_cast<Enum>(0); original != Enum::END; original = static_cast<Enum>(static_cast<unsigned>(original) + 1)) {
		std::string const str = to_string(original);
		auto const result = from_string<Enum>(str);
		if (original != result)
			throw InvalidToStringConversion (original, result, str);
	}
}

}	// anonymous namespace

void string_conversion_tests () {
	std::cout << "Running string conversion tests.\n";
	test_generic<Ability::Abilities> ("ability");
	test_generic<Gender::Genders> ("gender");
	test_generic<Item> ("item");
	test_generic<Moves> ("move");
	test_generic<Nature> ("nature");
	test_generic<Species> ("species");
	test_generic<Status::Statuses> ("status");
	std::cout << "String conversion tests passed.\n\n";
}

}	// namespace technicalmachine
