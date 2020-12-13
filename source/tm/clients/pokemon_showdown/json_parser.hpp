// Parse json messages from Pokemon Showdown
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <boost/property_tree/ptree.hpp>

#include <sstream>
#include <string_view>

namespace technicalmachine {
namespace ps {

struct JSONParser {
	boost::property_tree::ptree operator()(std::string_view str);
	
private:
	std::stringstream m_json_buffer;
};

}	// namespace ps
}	// namespace technicalmachine
