// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.get_directory;

namespace technicalmachine {
namespace {

TEST_CASE("get_environment_variable succeed on present", "[get_environment_variable]") {
	constexpr auto key = "TM_TEST_FOUND";
	constexpr auto value = "obi";
	constexpr auto overwrite = false;
	CHECK(::setenv(key, value, overwrite) == 0);
	CHECK(get_environment_variable(key) == value);
	::unsetenv(key);
}

TEST_CASE("get_environment_variable fail on missing", "[get_environment_variable]") {
	CHECK_THROWS(get_environment_variable("TM_TEST_NOT_FOUND"));
}

} // namespace
} // namespace technicalmachine