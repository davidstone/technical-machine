// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.wish;

import tm.pokemon.any_pokemon;

import tm.compress;
import tm.environment;
import tm.exists_if;
import tm.generation;
import tm.heal;
import tm.rational;

import bounded;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct Wish {
	constexpr auto activate() & -> void {
		if constexpr (exists) {
			if (!m_turns_until_activation) {
				constexpr auto turn_delay = 1_bi;
				m_turns_until_activation = counter_type(turn_delay);
			}
		}
	}

	auto decrement(any_mutable_active_pokemon auto const pokemon, Environment const environment) & -> void {
		if constexpr (exists) {
			if (!m_turns_until_activation) {
				return;
			}
			if (*m_turns_until_activation == 0_bi) {
				m_turns_until_activation = tv::none;
				heal(pokemon, environment, rational(1_bi, 2_bi));
			} else {
				--*m_turns_until_activation;
			}
		}
	}

	friend auto operator==(Wish, Wish) -> bool = default;
	friend constexpr auto compress(Wish<generation> const value) {
		return compress(value.m_turns_until_activation);
	}

private:
	static constexpr auto exists = generation >= Generation::three;
	using counter_type = bounded::integer<0, 1>;
	[[no_unique_address]] ExistsIf<tv::optional<counter_type>, exists> m_turns_until_activation;
};

} // namespace technicalmachine
