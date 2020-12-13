// Connect to Pokemon Showdown
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
