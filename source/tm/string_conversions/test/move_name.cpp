module;

#include <doctest/doctest.h>

export module tm.string_conversions.test.move_name;

import tm.move.move_name;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.move_name;

namespace technicalmachine {

TEST_CASE("string_conversion: move") {
	test_enum_string<MoveName>();
}

} // namespace technicalmachine
