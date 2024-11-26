// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.pokemon.partial_trap;

import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.end_of_turn_counter;

import tm.stat.current_hp;

import tm.compress;
import tm.environment;
import tm.generation;
import tm.heal;
import tm.item;
import tm.rational;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

// The number of turns remaining on moves like Wrap in later generations
export template<Generation generation>
struct PartialTrap {
	constexpr auto is_active() const -> bool {
		return m_counter.is_active();
	}
	constexpr auto activate() & -> void {
		if (!m_counter.is_active()) {
			m_counter.activate();
		}
	}
	constexpr auto end_of_turn_damage(
		any_mutable_active_pokemon auto const pokemon,
		Environment const environment
	) & -> void {
		if (!is_active()) {
			return;
		}
		auto const denominator = [=] {
			if constexpr (generation <= Generation::four) {
				return 16_bi;
			} else if constexpr (generation == Generation::five) {
				return BOUNDED_CONDITIONAL(
					pokemon.item(environment) == Item::Binding_Band,
					8_bi,
					16_bi
				);
			} else {
				return BOUNDED_CONDITIONAL(
					pokemon.item(environment) == Item::Binding_Band,
					6_bi,
					8_bi
				);
			}
		}();
		heal(pokemon, environment, rational(-1_bi, denominator));
		m_counter.advance_one_turn();
	}

	friend auto operator==(PartialTrap, PartialTrap) -> bool = default;
	friend constexpr auto compress(PartialTrap const value) {
		return compress(value.m_counter);
	}

private:
	// TODO:
	//	Gen 2-4 end of turn:
	//		2: 37.5%
	//		3: 37.5%
	//		4: 12.5%
	//		5: 12.5%
	//		Grip Claw makes it always 5
	//	Gen 5+ end of turn:
	//		4: 50%
	//		5: 50%
	//		Grip Claw makes it always 7
	EndOfTurnCounter<generation >= Generation::two, generation <= Generation::four ? 5 : 7> m_counter;
};

} // namespace technicalmachine
