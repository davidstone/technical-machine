// Read and write settings files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/optional.hpp>

#include <containers/string.hpp>

#include <filesystem>

namespace technicalmachine {

struct SettingsFile {
	bounded::optional<std::filesystem::path> team_file;
	containers::string host;
	containers::string port;
	containers::string username;
	containers::string password;
	containers::string resource;
};

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile;

}	// namespace technicalmachine
