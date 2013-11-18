// Generic outgoing messages
// Copyright (C) 2013 David Stone
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

#ifndef NETWORK__OUTMESSAGE_HPP_
#define NETWORK__OUTMESSAGE_HPP_

#include <cstdint>
#include <limits>
#include <string>
#include <vector>
#include <boost/asio/ip/tcp.hpp>
#include <ranged_integer/ranged_integer.hpp>

namespace technicalmachine {
class Team;
namespace network {

class OutMessage {
public:
	explicit OutMessage (uint8_t code);
	virtual ~OutMessage () {}
	void write_byte (uint8_t byte);
	template<intmax_t minimum, intmax_t maximum, typename policy>
	void write_byte(ranged_integer<minimum, maximum, policy> const integer) {
		static_assert(minimum >= 0 and maximum <= std::numeric_limits<uint8_t>::max(), "Value out of range of uint8_t.");
		write_byte(static_cast<uint8_t>(integer));
	}
	void write_short (uint16_t bytes);
	template<intmax_t minimum, intmax_t maximum, typename policy>
	void write_short(ranged_integer<minimum, maximum, policy> const integer) {
		static_assert(minimum >= 0 and maximum <= std::numeric_limits<uint16_t>::max(), "Value out of range of uint16_t.");
		write_short(static_cast<uint16_t>(integer));
	}
	void write_int (uint32_t bytes);
	template<intmax_t minimum, intmax_t maximum, typename policy>
	void write_int(ranged_integer<minimum, maximum, policy> const integer) {
		static_assert(minimum >= 0 and maximum <= std::numeric_limits<uint32_t>::max(), "Value out of range of uint32_t.");
		write_int(static_cast<uint32_t>(integer));
	}
	virtual void write_string (std::string const & str) = 0;
	virtual void write_move (uint32_t battle_id, uint8_t move_index, uint8_t target = 1) = 0;
	virtual void write_switch (uint32_t battle_id, uint8_t slot) = 0;
	virtual void write_team (Team const & team, std::string const & = std::string()) = 0;
	virtual void finalize() = 0;
	void send(boost::asio::ip::tcp::socket & socket);
	std::vector <uint8_t> buffer;
};

}	// namespace technicalmachine
}	// namespace network
#endif	// NETWORK__OUTMESSAGE_HPP_
