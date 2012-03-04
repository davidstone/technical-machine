// Read and write settings files
// Copyright (C) 2012 David Stone
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

#include "settings_file.hpp"

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace technicalmachine {

std::string const Settings::file_name = "settings/settings.xml";

Settings::Settings () {
	boost::property_tree::ptree pt;
	read_xml (file_name, pt);

	boost::property_tree::ptree & root = pt.get_child ("settings");

	team_file = root.get <std::string> ("team");
	for (boost::property_tree::ptree::value_type const & server_tree : root.get_child ("servers"))
		servers.push_back (Server (server_tree.second));
	time_format = root.get <std::string> ("time");
	chattiness = root.get <unsigned> ("chattiness");
}

Server::Server (boost::property_tree::ptree const & server):
	host (server.get <std::string> ("host")),
	port (server.get <std::string> ("port")),
	username (server.get <std::string> ("username")),
	password (server.get <std::string> ("password"))
	{
}

void Settings::write () const {
	boost::property_tree::ptree pt;
	boost::property_tree::xml_writer_settings<char> format_settings ('\t', 1);

	boost::property_tree::ptree & root = pt.add ("settings", "");

	root.add ("team", "teams/");
	boost::property_tree::ptree & servers_tree = root.add ("servers", "");
	for (Server const & server : servers)
		server.add (servers_tree);
	root.add ("time", "%Y-%m-%d %H:%M:%S");
	root.add ("chattiness", 50);

	write_xml (file_name, pt, std::locale (), format_settings);
}

void Server::add (boost::property_tree::ptree & root) const {
	boost::property_tree::ptree & server_tree = root.add ("server", "");
	server_tree.add ("host", host);
	server_tree.add ("port", port);
	server_tree.add ("username", username);
	server_tree.add ("password", password);
}

}	// namespace technicalmachine
