// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.end_of_turn_state_builder;

import tm.clients.ps.end_of_turn_state;

import tm.clients.party;

import tm.status.status_name;

import tm.end_of_turn_flags;
import tm.visible_hp;
import tm.weather;

import tv;
import std_module;

namespace technicalmachine::ps {

export struct EndOfTurnStateBuilder {
	constexpr auto set_expected(Party const party, VisibleHP const hp) & -> void {
		tv::insert(individual(party).hp, hp);
	}
	constexpr auto set_expected(Party const party, StatusName const status) & -> void {
		tv::insert(individual(party).status, status);
	}

	constexpr auto lock_in_ends(Party const party) & -> void {
		individual(party).flags.lock_in_ends = true;
	}
	constexpr auto shed_skin(Party const party) & -> void {
		individual(party).flags.shed_skin = true;
		set_expected(party, StatusName::clear);
	}
	constexpr auto thaw(Party const party) & -> void {
		individual(party).flags.thaws = true;
		set_expected(party, StatusName::clear);
	}

	constexpr auto active_weather(Weather const weather) & -> void {
		m_result.weather = weather;
	}

	constexpr auto complete() & -> EndOfTurnState {
		return std::exchange(m_result, {});
	}

private:
	constexpr auto individual(Party const party) & -> EndOfTurnState::Individual & {
		// TODO: This needs something smarter -- there are end of turn tiers
		if (!m_result.first_party) {
			tv::insert(m_result.first_party, party);
		}
		return *m_result.first_party == party ? m_result.first : m_result.last;
	}
	EndOfTurnState m_result;
};

} // namespace technicalmachine::ps
