// Generic outgoing messages
// Copyright (C) 2011 David Stone
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

#include "outmessage.hpp"
#include <cstdint>
#include <arpa/inet.h>

namespace technicalmachine {
namespace network {

OutMessage::OutMessage (uint8_t code) {
	write_byte (code);
}

void OutMessage::write_byte (uint8_t byte) {
	buffer.push_back (byte);
}

void OutMessage::write_short (uint16_t bytes) {
	uint16_t network_byte = htons (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint16_t); ++n)
		write_byte (*(byte + n));
}

void OutMessage::write_int (uint32_t bytes) {
	uint32_t network_byte = htonl (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		write_byte (*(byte + n));
}

}	// namespace technicalmachine
}	// namespace network
