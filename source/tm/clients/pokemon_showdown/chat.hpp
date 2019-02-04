// Connect to Pokemon Showdown
// Copyright (C) 2019 David Stone
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

#include <tm/clients/pokemon_showdown/inmessage.hpp>

namespace technicalmachine {
namespace ps {

inline bool handle_chat_message(InMessage message) {
	auto const type = message.type();
	if (type == ":") {
		// message.remainder() == seconds since 1970
		return true;
	} else if (type == "c" or type == "chat") {
		// message.remainder() == username|message
		return true;
	} else if (type == "c:") {
		// message.remainder() == seconds since 1970|username|message
		return true;
	} else if (type == "deinit") {
		// When you stay in a room too long
		return true;
	} else if (type == "expire") {
		// When you stay in a room too long
		return true;
	} else if (type == "j" or type == "J" or type == "join") {
		// message.remainder() == username
		return true;
	} else if (type == "l" or type == "L" or type == "leave") {
		// message.remainder() == username
		return true;
	} else if (type == "n" or type == "N" or type == "name") {
		// message.remainder() == new username|old username
		return true;
	} else {
		return false;
	}
}

}	// namespace ps
}	// namespace technicalmachine
