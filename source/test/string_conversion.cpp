// Test string conversions
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

#include "string_conversion.hpp"

#include <iostream>
#include <string>

#include "../ability.hpp"
#include "../enum_range.hpp"
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

// I make the ToString conversion a logic error because I only convert to a
// string from an internal data structure, so I know the input is only invalid
// if there is a logic error in my program.

struct InvalidToStringConversion : std::logic_error {
	template<typename Test>
	InvalidToStringConversion(Test original, Test result, boost::string_ref const intermediate):
		std::logic_error(to_string(bounded::integer(original)) + " is seen as " + to_string(bounded::integer(result)) + " with an intermediate string of " + intermediate.to_string() + ".\n") {
	}
};

template <typename Enum>
void test_generic (std::string const & thing) {
	std::cout << "\tVerifying correct " + thing + ".\n";
	for (auto const original : enum_range<Enum>) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		if (original != result) {
			throw InvalidToStringConversion(original, result, str);
		}
	}
}

}	// namespace

void string_conversion_tests () {
	std::cout << "Running string conversion tests.\n";
	test_generic<Ability> ("ability");
	test_generic<Gender> ("gender");
	test_generic<Item> ("item");
	test_generic<Moves> ("move");
	test_generic<Nature> ("nature");
	test_generic<Species> ("species");
	test_generic<Statuses> ("status");
	std::cout << "String conversion tests passed.\n\n";
}

}	// namespace technicalmachine
