// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <doctest/doctest.h>

import tm.get_directory;

namespace technicalmachine {
namespace {

TEST_CASE("get_environment_variable: get_environment_variable succeed on present") {
	CHECK_NOTHROW(get_environment_variable("TM_TEST_DIRECTORY"));
}

TEST_CASE("get_environment_variable: get_environment_variable fail on missing") {
	CHECK_THROWS(get_environment_variable("TM_TEST_NOT_FOUND"));
}

} // namespace
} // namespace technicalmachine