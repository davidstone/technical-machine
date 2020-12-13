// Read and write settings files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/optional.hpp>

#include <boost/property_tree/ptree.hpp>

#include <filesystem>
#include <string>

namespace technicalmachine {

struct SettingsFile {
	bounded::optional<std::filesystem::path> team_file;
	std::string host;
	std::string port;
	std::string username;
	std::string password;
	std::string resource;
};

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile;

}	// namespace technicalmachine
