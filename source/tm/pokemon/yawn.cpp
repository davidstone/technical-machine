// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.yawn;

import tm.pokemon.any_pokemon;
import tm.pokemon.end_of_turn_counter;
import tm.pokemon.indirect_status_can_apply;

import tm.status.status_name;

import tm.environment;
import tm.generation;
import tm.weather;

namespace technicalmachine {

export template<Generation generation>
struct YawnCounter {
	constexpr auto activate() & -> void {
		if (!m_counter.is_active()) {
			m_counter.activate();
		}
	}
	constexpr auto advance_one_turn(
		any_mutable_active_pokemon auto const pokemon,
		Environment const environment,
		Weather const weather,
		bool const either_is_uproaring,
		bool const sleep_clause_activates
	) & -> void {
		if (!m_counter.is_active()) {
			return;
		}
		m_counter.advance_one_turn();
		if (m_counter.is_active()) {
			return;
		}
		// TODO: Various edge cases in different generations
		auto const can_apply =
			!either_is_uproaring and
			!sleep_clause_activates and
			indirect_status_can_apply(StatusName::sleep, pokemon.as_const(), weather);
		if (can_apply) {
			pokemon.set_status(StatusName::sleep, environment);
		}
	}
	friend auto operator==(YawnCounter, YawnCounter) -> bool = default;
	friend constexpr auto compress(YawnCounter const value) {
		return compress(value.m_counter);
	}
private:
	[[no_unique_address]] EndOfTurnCounter<generation >= Generation::three, 1> m_counter;
};

} // namespace technicalmachine
