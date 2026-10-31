module;

#include <doctest/doctest.h>

export module tm.string_conversions.test.item;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.item;

import tm.item;

namespace technicalmachine {

TEST_CASE("string_conversion: item") {
	test_enum_string<Item>();
}

} // namespace technicalmachine
