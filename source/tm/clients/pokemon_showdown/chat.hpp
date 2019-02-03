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
	if (message.type() == ":") {
		// message.remainder() == seconds since 1970
		return true;
	} else if (message.type() == "c" or message.type() == "chat") {
		// message.remainder() == username|message
		return true;
	} else if (message.type() == "c:") {
		// message.remainder() == seconds since 1970|username|message
		return true;
	} else if (message.type() == "j" or message.type() == "J" or message.type() == "join") {
		// message.remainder() == username
		return true;
	} else if (message.type() == "l" or message.type() == "L" or message.type() == "leave") {
		// message.remainder() == username
		return true;
	} else if (message.type() == "n" or message.type() == "N" or message.type() == "name") {
		// message.remainder() == new username|old username
		return true;
	} else {
		return false;
	}
}

}	// namespace ps
}	// namespace technicalmachine
