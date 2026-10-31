module;

#include <doctest/doctest.h>

export module tm.string_conversions.test.ability;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.ability;

import tm.ability;

namespace technicalmachine {

TEST_CASE("string_conversion: ability") {
	test_enum_string<Ability>();
}

} // namespace technicalmachine
