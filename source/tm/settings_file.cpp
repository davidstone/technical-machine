// Read and write settings files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/settings_file.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <containers/string.hpp>

namespace technicalmachine {

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile {
	boost::property_tree::ptree pt;
	read_xml(path.string(), pt);
	auto const & server = pt.get_child("settings");
	auto username = containers::string(server.get<std::string>("username"));
	if (containers::is_empty(username)) {
		throw std::runtime_error("Missing username and password in settings file");
	}
	auto team_file = server.get_optional<std::filesystem::path>("team");
	return SettingsFile{
		BOUNDED_CONDITIONAL(team_file, std::move(*team_file), bounded::none),
		containers::string(server.get<std::string>("host")),
		containers::string(server.get<std::string>("port")),
		std::move(username),
		containers::string(server.get<std::string>("password")),
		containers::string(server.get<std::string>("resource"))
	};
}

}	// namespace technicalmachine
