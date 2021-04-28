// Read and write settings files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/settings_file.hpp>

#include <containers/string.hpp>

#include <nlohmann/json.hpp>

#include <fstream>

namespace technicalmachine {

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile {
	auto file = std::ifstream(path);
	auto json = nlohmann::json();
	file >> json;
	auto const & server = json.at("settings");
	auto username = containers::string(server.at("username").get<std::string_view>());
	if (containers::is_empty(username)) {
		throw std::runtime_error("Missing username and password in settings file");
	}
	auto team_file = server.at("team").get<std::string_view>();

	return SettingsFile{
		BOUNDED_CONDITIONAL(!containers::is_empty(team_file), std::filesystem::path(team_file), bounded::none),
		containers::string(server.at("host").get<std::string_view>()),
		containers::string(server.at("port").get<std::string_view>()),
		std::move(username),
		containers::string(server.at("password").get<std::string_view>()),
		containers::string(server.at("resource").get<std::string_view>())
	};
}

}	// namespace technicalmachine
