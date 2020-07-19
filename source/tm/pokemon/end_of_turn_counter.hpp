// Common functionality for flags that are advanced at the end of the turn
// Copyright (C) 2016 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/compress.hpp>
#include <tm/exists_if.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class CounterOperations { is_active, turns_active, advance_one_turn_fixed, advance_one_turn_variable, activate };

template<bool is_supported_this_generation, int max_turns, CounterOperations... operations>
struct EndOfTurnCounter {
	constexpr auto is_active() const -> bool {
		static_assert((... or (operations == CounterOperations::is_active)));
		if constexpr (is_supported_this_generation) {
			return static_cast<bool>(m_turns_active);
		} else {
			return false;
		}
	}
	constexpr auto turns_active() const {
		static_assert((... or (operations == CounterOperations::turns_active)));
		if constexpr (is_supported_this_generation) {
			return m_turns_active;
		} else {
			return bounded::none;
		}
	}
	constexpr auto activate() {
		static_assert((... or (operations == CounterOperations::activate)));
		if constexpr (is_supported_this_generation) {
			m_turns_active = 0_bi;
		}
	}
	constexpr auto advance_one_turn() {
		constexpr auto is_fixed = (... or (operations == CounterOperations::advance_one_turn_fixed));
		constexpr auto is_variable = (... or (operations == CounterOperations::advance_one_turn_variable));
		static_assert(is_fixed xor is_variable);
		if constexpr (is_supported_this_generation) {
			if (m_turns_active and *m_turns_active != bounded::constant<max_turns>) {
				++*m_turns_active;
			} else {
				m_turns_active = bounded::none;
			}
		}
		if constexpr (is_supported_this_generation and is_fixed) {
			return static_cast<bool>(m_turns_active);
		}
	}
	friend auto operator==(EndOfTurnCounter const &, EndOfTurnCounter const &) -> bool = default;
	friend constexpr auto compress(EndOfTurnCounter const value) {
		return compress(value.m_turns_active);
	}
private:
	using Counter = bounded::optional<bounded::integer<0, max_turns>>;
	[[no_unique_address]] ExistsIf<Counter, is_supported_this_generation> m_turns_active{bounded::none};
};

}	// namespace technicalmachine
