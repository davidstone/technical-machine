// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.send_selection;

import tm.clients.ps.send_message_function;
import tm.clients.ps.room;
import tm.clients.ps.slot_memory;

import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.string_conversions.move_name;

import containers;
import std_module;
import tv;

// https://github.com/smogon/pokemon-showdown/blob/master/PROTOCOL.md

namespace technicalmachine::ps {
using namespace std::string_view_literals;

struct SendSelection {
	constexpr auto operator()(Switch const switch_) const -> void {
		m_send_message(containers::concatenate<containers::string>(
			m_room,
			"|/choose switch "sv,
			containers::to_string(m_slot_memory[switch_.value()])
		));
	}
	constexpr auto operator()(MoveName const move) const -> void {
		m_send_message(containers::concatenate<containers::string>(
			m_room,
			"|/choose move "sv,
			to_string(move)
		));
	}
	static constexpr auto operator()(Pass) -> void {
	}

	SendMessageFunction m_send_message;
	Room m_room;
	SlotMemory m_slot_memory;
};

export constexpr auto send_selection(
	Selection const selection,
	SendMessageFunction const send_message,
	Room const room,
	SlotMemory const slot_memory
) -> void {
	tv::visit(
		selection,
		SendSelection(send_message, room, slot_memory)
	);
}

} // namespace technicalmachine::ps
