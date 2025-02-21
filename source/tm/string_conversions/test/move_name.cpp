// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.string_conversions.test.move_name;

import tm.move.move_name;

import tm.string_conversions.test.test_enum_string;

import tm.string_conversions.move_name;

namespace technicalmachine {

TEST_CASE("move", "[string_conversion]") {
	test_enum_string<MoveName>();
}

} // namespace technicalmachine
