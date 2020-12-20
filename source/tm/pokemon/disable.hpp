// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/operators.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Disable {
	auto activate(Moves const move) {
		if (is_regular(move)) {
			insert(m_disabled_move, move);
		}
	}
	constexpr auto move_is_disabled(Moves const move) const {
		return m_disabled_move == move;
	}

	constexpr auto advance_one_turn() {
		if (!m_disabled_move) {
			return;
		}
		// TODO: update with proper probability actions
		if (m_turns_disabled < bounded::max_value<TurnCount>) {
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
	bounded::optional<Moves> m_disabled_move = bounded::none;
	using TurnCount = bounded::integer<0, 7>;
	TurnCount m_turns_disabled = 0_bi;
};

}	// namespace technicalmachine
