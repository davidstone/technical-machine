// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/compress.hpp>
#include <tm/exists_if.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/optional.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation generation>
struct Wish {
	constexpr auto activate() & -> void {
		if constexpr (exists) {
			if (!m_turns_until_activation) {
				constexpr auto turn_delay = 1_bi;
				m_turns_until_activation = counter_type(turn_delay);
			}
		}
	}

	auto decrement(any_mutable_active_pokemon auto const pokemon, Weather const weather) & -> void {
		if constexpr (exists) {
			if (!m_turns_until_activation) {
				return;
			}
			if (*m_turns_until_activation == 0_bi) {
				m_turns_until_activation = bounded::none;
				heal(pokemon, weather, rational(1_bi, 2_bi));
			} else {
				--*m_turns_until_activation;
			}
		}
	}

	friend auto operator==(Wish, Wish) -> bool = default;
	friend constexpr auto compress(Wish<generation> const value) {
		if constexpr (exists) {
			return compress(value.m_turns_until_activation);
		} else {
			return 0_bi;
		}
	}

private:
	static constexpr auto exists = generation >= Generation::three;
	using counter_type = bounded::integer<0, 1>;
	[[no_unique_address]] ExistsIf<bounded::optional<counter_type>, exists> m_turns_until_activation;
};

} // namespace technicalmachine
