// Generic outgoing messages
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

#ifndef NETWORK__OUTMESSAGE_HPP_
#define NETWORK__OUTMESSAGE_HPP_

#include <cstdint>
#include <string>
#include <vector>

namespace technicalmachine {
class Team;
namespace network {

class OutMessage {
	public:
		std::vector <uint8_t> buffer;
		explicit OutMessage (uint8_t code);
		virtual ~OutMessage () {}
		template<typename Integer>
		void write_bytes(Integer bytes);
		// The next three are the same as above for sizeof(Integer) = 1, 2, 4
		void write_byte (uint8_t byte);
		void write_short (uint16_t bytes);
		void write_int (uint32_t bytes);
		virtual void write_string (std::string const & str) = 0;
		virtual void write_move (uint32_t battle_id, uint8_t move_index, uint8_t target = 1) = 0;
		virtual void write_switch (uint32_t battle_id, uint8_t slot) = 0;
		virtual void write_team (Team const & team, std::string const & = std::string()) = 0;
};

}	// namespace technicalmachine
}	// namespace network
#endif	// NETWORK__OUTMESSAGE_HPP_
