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

#pragma once

#include <cstdint>
#include <string>
#include <boost/utility/enable_if.hpp>
#include "invalid_packet.hpp"

namespace technicalmachine {
namespace network {

using std::to_string;

struct InvalidSimulatorData : InvalidPacket {
	template<typename Received, typename Minimum, typename Maximum>
	InvalidSimulatorData(Received const received, Minimum const minimum, Maximum const maximum, std::string const & variable):
		InvalidPacket (variable + " should be between " + to_string(minimum) + " and " + to_string(maximum) + ", but the server sent a value of " + to_string(received) + ".") {
	}
	template<typename Received>
	InvalidSimulatorData(Received const received, std::string const & variable):
		InvalidPacket ("Recieved an invalid value of " + to_string(received) + " for " + variable + ".") {
	}
};
}	// namespace network
}	// namespace technicalmachine
