// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.message_block;

import tm.clients.ps.in_message;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {

// Guaranteed to return a range of at least one (possibly empty) message
export constexpr auto message_block(std::string_view str) {
	return containers::transform(
		containers::split(std::move(str), '\n'),
		bounded::construct<InMessage>
	);
}
export using MessageBlock = decltype(message_block(""));

} // namespace technicalmachine::ps
