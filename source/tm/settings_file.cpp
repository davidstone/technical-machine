// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/settings_file.hpp>

#include <tm/load_json_from_file.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <nlohmann/json.hpp>

#include <fstream>

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

auto get_string(nlohmann::json const & json, std::string_view const key) {
	return json.at(key).get<std::string_view>();
};

auto parse_team(nlohmann::json const & json) {
	auto const it = json.find("team");
	if (it == json.end()) {
		return SettingsFile::Team(SettingsFile::NoTeam());
	}
	auto const team = it->get<std::string_view>();
	return containers::is_empty(team) ?
		SettingsFile::Team(SettingsFile::GenerateTeam()) :
		SettingsFile::Team(std::filesystem::path(team));
}

auto parse_style(nlohmann::json const & json) {
	auto get = [&](std::string_view const str) {
		return get_string(json, str);
	};
	auto const mode = get("mode");
	if (mode == "ladder") {
		return SettingsFile::Style(SettingsFile::Ladder{
			get("format")
		});
	} else if (mode == "challenge") {
		return SettingsFile::Style(SettingsFile::Challenge{
			get("user"),
			get("format")
		});
	} else if (mode == "accept") {
		return SettingsFile::Style(SettingsFile::Accept{
			containers::vector<containers::string>(json.at("users").get<std::vector<std::string_view>>())
		});
	} else {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid mode "sv, mode));
	}
}

} // namespace

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile {
	auto const json = load_json_from_file(path);
	auto const & settings = json.at("settings");
	auto get = [&](std::string_view const str) {
		return get_string(settings, str);
	};
	auto username = get("username");
	if (containers::is_empty(username)) {
		throw std::runtime_error("Missing username and password in settings file");
	}

	return SettingsFile{
		get("host"),
		get("port"),
		get("resource"),
		std::move(username),
		get("password"),
		parse_team(settings),
		parse_style(settings.at("style"))
	};
}

} // namespace technicalmachine
