// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.disable;

import tm.move.move_name;

import tm.compress;

import bounded;
import numeric_traits;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export struct Disable {
	auto activate(MoveName const move) {
		if (!is_regular(move)) {
			return false;
		}
		insert(m_disabled_move, move);
		return true;
	}
	constexpr auto move_is_disabled(MoveName const move) const {
		return m_disabled_move == move;
	}

	constexpr auto advance_one_turn() {
		if (!m_disabled_move) {
			return;
		}
		// TODO: update with proper probability actions
		if (m_turns_disabled < numeric_traits::max_value<TurnCount>) {
			++m_turns_disabled;
		} else {
			*this = Disable{};
		}
	}

	friend auto operator==(Disable, Disable) -> bool = default;
	friend constexpr auto compress(Disable const value) {
		return compress_combine(value.m_disabled_move, value.m_turns_disabled);
	}

private:
	// TODO: Use an optional struct?
	tv::optional<MoveName> m_disabled_move = tv::none;
	using TurnCount = bounded::integer<0, 7>;
	TurnCount m_turns_disabled = 0_bi;
};

} // namespace technicalmachine
