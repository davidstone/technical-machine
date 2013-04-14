// Exception class for illegal simulator message data
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

#ifndef NETWORK__INVALID_SIMULATOR_DATA_HPP_
#define NETWORK__INVALID_SIMULATOR_DATA_HPP_

#include <cstdint>
#include <string>
#include <boost/utility/enable_if.hpp>
#include "invalid_packet.hpp"

namespace technicalmachine {
namespace network {

class InvalidSimulatorData : public InvalidPacket {
public:
	template<typename Integer, typename boost::enable_if_c<sizeof(Integer) <= 4, int>::type = 0>
	InvalidSimulatorData (Integer const received, Integer const minimum, Integer const maximum, std::string const & variable):
		InvalidPacket (variable + " should be between " + std::to_string(static_cast<int64_t>(minimum)) + " and " + std::to_string(static_cast<int64_t>(maximum)) + ", but the server sent a value of " + std::to_string(static_cast<int64_t>(received)) + ".") {
	}
	template<typename Integer, typename boost::enable_if_c<sizeof(Integer) <= 4, int>::type = 0>
	InvalidSimulatorData(Integer const received, std::string const & variable):
		InvalidPacket ("Recieved an invalid value of " + std::to_string(static_cast<int64_t>(received)) + " for " + variable + ".") {
	}
};
}	// namespace network
}	// namespace technicalmachine
#endif	// NETWORK__INVALID_SIMULATOR_DATA_HPP_
