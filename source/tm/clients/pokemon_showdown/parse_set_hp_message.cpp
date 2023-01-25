// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_set_hp_message;

import tm.clients.ps.inmessage;
import tm.clients.ps.parse_effect_source;
import tm.clients.ps.parse_hp_and_status;
import tm.clients.ps.party_from_player_id;

import tm.clients.party;

import tm.status.status_name;

import tm.visible_hp;

namespace technicalmachine::ps {

export constexpr auto parse_set_hp_message(InMessage message) {
	struct Message {
		Party party;
		VisibleHP hp;
		StatusName status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message.pop());
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

} // namespace technicalmachine::ps
