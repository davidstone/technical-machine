// Exception class for improper message length
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

#ifndef NETWORK_BUFFER_OVERRUN_H_
#define NETWORK_BUFFER_OVERRUN_H_

#include "invalid_packet.h"

namespace technicalmachine {
namespace network {

class BufferOverrun : public InvalidPacket {
	public:
		BufferOverrun (): InvalidPacket ("Read past the end of the InMessage buffer.") { }
};
} // namespace network
} // namespace technicalmachine
#endif // NETWORK_BUFFER_OVERRUN_H_
