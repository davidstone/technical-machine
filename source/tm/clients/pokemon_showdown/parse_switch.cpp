// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_switch;

import tm.clients.ps.in_message;
import tm.clients.ps.parse_details;
import tm.clients.ps.parse_hp_and_status;
import tm.clients.ps.parse_pokemon_identity;
import tm.clients.ps.switch_message;

namespace technicalmachine::ps {

export constexpr auto parse_switch(InMessage message) -> SwitchMessage {
	auto const identity = parse_pokemon_identity(message.pop());
	auto const details = parse_details(message.pop());
	auto const [visible_hp, status] = parse_hp_and_status(message.pop());
	return SwitchMessage(
		identity.party,
		details.species,
		identity.nickname,
		details.level,
		details.gender,
		visible_hp,
		status
	);
}

} // namespace technicalmachine::ps
