// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

export module tm.string_conversions.test.item;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.item;

import tm.item;

namespace technicalmachine {

TEST_CASE("item", "[string_conversion]") {
	test_enum_string<Item>();
}

} // namespace technicalmachine
