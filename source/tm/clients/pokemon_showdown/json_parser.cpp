// Parse json messages from Pokemon Showdown
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/json_parser.hpp>

#include <boost/property_tree/json_parser.hpp>

namespace technicalmachine {
namespace ps {

boost::property_tree::ptree JSONParser::operator()(std::string_view const str) {
	m_json_buffer.str(std::string(str));
	// Hopefully, boost::property_tree's JSON parser is secure...
	boost::property_tree::ptree pt;
	read_json(m_json_buffer, pt);
	return pt;
}

}	// namespace ps
}	// namespace technicalmachine
