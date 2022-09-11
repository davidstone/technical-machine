// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/exists_if.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

enum class Resettable { no, yes };

template<bool is_supported_this_generation, int max_turns, Resettable resettable = Resettable::yes>
struct EndOfTurnCounter {
	constexpr auto is_active() const -> bool {
		if constexpr (is_supported_this_generation) {
			return static_cast<bool>(m_turns_active);
		} else {
			return false;
		}
	}
	constexpr auto activate() & -> void {
		if constexpr (is_supported_this_generation) {
			if (resettable == Resettable::no and m_turns_active) {
				return;
			}
			m_turns_active = 0_bi;
		}
	}
	constexpr auto advance_one_turn() & -> void {
		if constexpr (is_supported_this_generation) {
			if (m_turns_active and *m_turns_active != bounded::constant<max_turns>) {
				++*m_turns_active;
			} else {
				m_turns_active = bounded::none;
			}
		}
	}
	friend auto operator==(EndOfTurnCounter, EndOfTurnCounter) -> bool = default;
	friend constexpr auto compress(EndOfTurnCounter const value) {
		return compress(value.m_turns_active);
	}

private:
	using Counter = bounded::optional<bounded::integer<0, max_turns>>;
	[[no_unique_address]] ExistsIf<Counter, is_supported_this_generation> m_turns_active{bounded::none};
};

} // namespace technicalmachine
