// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battles;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_started;
import tm.clients.ps.parsed_request;
import tm.clients.ps.room;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_already_finished;
import tm.clients.battle_continues;
import tm.clients.battle_finished;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export struct Battles {
	Battles() = default;

	constexpr auto handle_request(
		Room const room,
		ParsedRequest const & message
	) -> void {
		auto const battle = containers::lookup(m_container, room);
		if (!battle) {
			throw std::runtime_error("Got a request for a battle that does not exist");
		}
		battle->handle_request(message);
	}

	using Result = tv::variant<
		ActionRequired,
		StartOfTurn,
		BattleFinished,
		BattleContinues,
		BattleStarted,
		BattleAlreadyFinished
	>;
	constexpr auto handle_message(
		Room const room,
		BattleMessage const & generic_message
	) -> Result {
		auto const it = m_container.find(room);
		return tv::visit(generic_message, tv::overload(
			[&](CreateBattle) -> Result {
				if (it != containers::end(m_container)) {
					throw std::runtime_error("Tried to create an existing battle");
				}
				create_battle(room);
				return BattleContinues();
			},
			[&](auto const & message) -> Result {
				if (it == containers::end(m_container)) {
					return BattleAlreadyFinished();
				}
				auto result = it->mapped.handle_message(message);
				return tv::visit(
					std::move(result),
					[&]<typename T>(T value) -> Result {
						if constexpr (std::same_as<T, BattleFinished>) {
							m_container.erase(it);
						}
						return value;
					}
				);
			}
		));
	}

private:
	constexpr auto create_battle(Room const room) -> void {
		m_container.lazy_insert(
			containers::string(room),
			bounded::construct<BattleManager>
		);
	}

	containers::linear_map<containers::string, BattleManager> m_container;
};

} // namespace technicalmachine::ps
