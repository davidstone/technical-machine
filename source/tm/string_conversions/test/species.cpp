module;

#include <doctest/doctest.h>

export module tm.string_conversions.test.species;

import tm.pokemon.species;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.species;

namespace technicalmachine {

TEST_CASE("string_conversion: species") {
	test_enum_string<Species>();
}

} // namespace technicalmachine
