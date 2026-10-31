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