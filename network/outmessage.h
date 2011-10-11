// Generic outgoing messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef NETWORK_OUTMESSAGE_H_
#define NETWORK_OUTMESSAGE_H_

#include <cstdint>
#include <vector>

namespace technicalmachine {
namespace network {

class OutMessage {
	public:
		std::vector <uint8_t> buffer;
		explicit OutMessage (uint8_t code);
		void write_byte (uint8_t byte);
		void write_short (uint16_t bytes);
		void write_int (uint32_t bytes);
};

} // namespace technicalmachine
} // namespace network
#endif  // NETWORK_OUTMESSAGE_H_
