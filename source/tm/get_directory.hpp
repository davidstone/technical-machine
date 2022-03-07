// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <containers/algorithms/concatenate.hpp>

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

inline auto get_environment_variable(char const * const environment_variable) -> std::string_view {
	auto const dir = ::getenv(environment_variable);
	if (!dir) {
		throw std::runtime_error(containers::concatenate<std::string>("Please create an environment variable "sv, std::string_view(environment_variable)));
	}
	return dir;
}

inline auto get_settings_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_SETTINGS_DIRECTORY"));
}

inline auto get_test_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_TEST_DIRECTORY"));
}

inline auto get_usage_stats_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_USAGE_STATS_DIRECTORY"));
}

} // namespace technicalmachine
