// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/hp_and_status.hpp>

#include <tm/clients/party.hpp>

#include <tm/end_of_turn.hpp>

#include <bounded/optional.hpp>

#include <utility>

namespace technicalmachine::ps {

struct EndOfTurnState {
	struct Result {
		struct Individual {
			bounded::optional<HPAndStatus> hp_and_status;
			EndOfTurnFlags flags = EndOfTurnFlags(false, false);
		};
		bounded::optional<Party> first_party;
		NormalWeather weather = NormalWeather::clear;
		Individual first;
		Individual last;
	};

	constexpr auto hp_change(Party const party, HPAndStatus const hp_and_status) & -> void {
		// Keep overwriting this value -- only the latest matters
		bounded::insert(individual(party).hp_and_status, hp_and_status);
	}

	constexpr auto lock_in_ends(Party const party) & -> void {
		individual(party).flags.lock_in_ends = true;
	}
	constexpr auto shed_skin(Party const party) & -> void {
		individual(party).flags.shed_skin = true;
	}

	constexpr auto active_weather(NormalWeather const weather) & -> void {
		m_result.weather = weather;
	}

	constexpr auto complete() & -> Result {
		return std::exchange(m_result, {});
	}
	
private:
	constexpr auto individual(Party const party) & -> Result::Individual &{
		// TODO: This needs something smarter -- there are end of turn tiers
		if (!m_result.first_party) {
			bounded::insert(m_result.first_party, party);
		}
		return *m_result.first_party == party ? m_result.first : m_result.last;
	}
	Result m_result;
};

} // namespace technicalmachine::ps
