// Read and write settings files
// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/settings_file.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>

namespace technicalmachine {

auto load_settings_file(std::filesystem::path const & path) -> SettingsFile {
	boost::property_tree::ptree pt;
	read_xml(path.string(), pt);
	auto const & server = pt.get_child("settings");
	auto username = server.get<std::string>("username");
	if (username.empty()) {
		throw std::runtime_error("Missing username and password in settings file");
	}
	return SettingsFile{
		server.get<std::filesystem::path>("team"),
		server.get<std::string>("host"),
		server.get<std::string>("port"),
		std::move(username),
		server.get<std::string>("password"),
		server.get<std::string>("resource")
	};
}

}	// namespace technicalmachine
