// Exception class for settings files that are incorrect
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/invalid_settings_file.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <string>
#include <string_view>

namespace technicalmachine {
namespace {

constexpr auto to_string(InvalidSettingsFile::Problem const problem) -> std::string_view {
	switch (problem) {
		case InvalidSettingsFile::Problem::does_not_exist: return " does not exist.";
		case InvalidSettingsFile::Problem::too_long: return " is too long.";
		case InvalidSettingsFile::Problem::too_short: return " is too short.";
		case InvalidSettingsFile::Problem::invalid_data: return " contains invalid data.";
	}
}
}	// namespace

InvalidSettingsFile::InvalidSettingsFile(std::filesystem::path const & file_name, Problem const problem):
	std::runtime_error(containers::concatenate<std::string>(file_name.string(), to_string(problem))) {
}

}	// namespace technicalmachine
