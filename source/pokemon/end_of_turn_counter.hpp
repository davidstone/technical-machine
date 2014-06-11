// Common functionality for flags that are advanced at the end of the turn
// Copyright (C) 2014 David Stone
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

#ifndef POKEMON_END_OF_TURN_FLAG_HPP_
#define POKEMON_END_OF_TURN_FLAG_HPP_

#include "../hash.hpp"

#include <bounded_integer/optional.hpp>

#include <type_traits>

namespace technicalmachine {
using namespace bounded::literal;

template<typename T, T value_to_find, T... ts>
struct Any;
template<typename T, T value_to_find, T first, T... ts>
struct Any<T, value_to_find, first, ts...> : std::integral_constant<bool, value_to_find == first or Any<T, value_to_find, ts...>::value> {};
template<typename T, T value_to_find>
struct Any<T, value_to_find> : std::integral_constant<bool, false> {};



enum class CounterOperations { is_active, turns_active, advance_one_turn, advance_one_turn_deactivated, activate };

template<intmax_t max_turns, CounterOperations... operations>
class EndOfTurnCounter {
public:
	constexpr auto is_active() const {
		static_assert(Any<CounterOperations, CounterOperations::is_active, operations...>::value, "This type does not support checking if it is active.");
		return static_cast<bool>(m_turns_active);
	}
	constexpr auto turns_active() const {
		static_assert(Any<CounterOperations, CounterOperations::turns_active, operations...>::value, "This type does not support querying the turn count.");
		return m_turns_active;
	}
	auto activate() {
		static_assert(Any<CounterOperations, CounterOperations::activate, operations...>::value, "This type does not support activation.");
		m_turns_active = 0_bi;
	}
	auto advance_one_turn() {
		static_assert(Any<CounterOperations, CounterOperations::advance_one_turn, operations...>::value, "This type does not support advancing the counter by one.");
		m_turns_active = next_turn_value();
	}
	auto advance_one_turn_deactivated() {
		static_assert(Any<CounterOperations, CounterOperations::advance_one_turn_deactivated, operations...>::value, "This type does not support advancing the counter by one and returning whether it just deactivated.");
		if (!static_cast<bool>(m_turns_active)) {
			return false;
		}
		m_turns_active = next_turn_value();
		return static_cast<bool>(m_turns_active);
	}
	friend constexpr auto operator==(EndOfTurnCounter const lhs, EndOfTurnCounter const rhs) -> bool {
		return lhs.m_turns_active == rhs.m_turns_active;
	}
	
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_turns_active);
	}
	constexpr auto max_hash() const noexcept {
		return technicalmachine::max_hash(m_turns_active);
	}
private:
	constexpr auto next_turn_value() {
		return (static_cast<bool>(m_turns_active) and *m_turns_active != bounded::make<max_turns>()) ?
			Counter(*m_turns_active + 1_bi) :
			bounded::none
		;
	}
	using Counter = bounded::optional<bounded::integer<0, max_turns>>;
	Counter m_turns_active;
};

template<intmax_t max_turns, CounterOperations... operations>
constexpr auto operator!=(EndOfTurnCounter<max_turns, operations...> const lhs, EndOfTurnCounter<max_turns, operations...> const rhs) {
	return !(lhs == rhs);
}

template<intmax_t max_turns, CounterOperations... operations>
constexpr auto hash(EndOfTurnCounter<max_turns, operations...> const flag) noexcept {
	return flag.hash();
}
template<intmax_t max_turns, CounterOperations... operations>
constexpr auto max_hash(EndOfTurnCounter<max_turns, operations...> const flag) noexcept {
	return flag.max_hash();
}

}	// namespace technicalmachine
#endif	// POKEMON_END_OF_TURN_FLAG_HPP_
