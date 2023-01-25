// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.get_directory;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export auto get_environment_variable(char const * const environment_variable) -> std::string_view {
	auto const dir = std::getenv(environment_variable);
	if (!dir) {
		throw std::runtime_error(containers::concatenate<std::string>("Please create an environment variable "sv, std::string_view(environment_variable)));
	}
	return dir;
}

export auto get_settings_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_SETTINGS_DIRECTORY"));
}

export auto get_test_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_TEST_DIRECTORY"));
}

export auto get_usage_stats_directory() -> std::filesystem::path {
	return std::filesystem::path(get_environment_variable("TM_USAGE_STATS_DIRECTORY"));
}

} // namespace technicalmachine
