// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/party.hpp>

#include <tm/end_of_turn.hpp>
#include <tm/status.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/optional.hpp>

#include <utility>

namespace technicalmachine::ps {

struct EndOfTurnState {
	struct Result {
		struct Individual {
			bounded::optional<VisibleHP> hp;
			bounded::optional<Statuses> status;
			EndOfTurnFlags flags = EndOfTurnFlags(false, false);
		};
		bounded::optional<Party> first_party;
		NormalWeather weather = NormalWeather::clear;
		Individual first;
		Individual last;
	};

	constexpr auto set_expected(Party const party, VisibleHP const hp) & -> void {
		bounded::insert(individual(party).hp, hp);
	}
	constexpr auto set_expected(Party const party, Statuses const status) & -> void {
		bounded::insert(individual(party).status, status);
	}

	constexpr auto lock_in_ends(Party const party) & -> void {
		individual(party).flags.lock_in_ends = true;
	}
	constexpr auto shed_skin(Party const party) & -> void {
		individual(party).flags.shed_skin = true;
		set_expected(party, Statuses::clear);
	}

	constexpr auto active_weather(NormalWeather const weather) & -> void {
		m_result.weather = weather;
	}

	constexpr auto complete() & -> Result {
		return std::exchange(m_result, {});
	}

private:
	constexpr auto individual(Party const party) & -> Result::Individual & {
		// TODO: This needs something smarter -- there are end of turn tiers
		if (!m_result.first_party) {
			bounded::insert(m_result.first_party, party);
		}
		return *m_result.first_party == party ? m_result.first : m_result.last;
	}
	Result m_result;
};

} // namespace technicalmachine::ps
