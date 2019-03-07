// Copyright (C) 2019 David Stone
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

#include <tm/move/moves.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct LastUsedMove {
	constexpr auto name() const {
		return m_move;
	}
	
	constexpr auto unsucessful_move(Moves const move) & {
		m_move = move;
		m_consecutive_successes = 0_bi;
	}

	constexpr auto increment(Moves const move) & {
		if (m_move == move) {
			++m_consecutive_successes;
		} else {
			m_move = move;
			m_consecutive_successes = 1_bi;
		}
	}


	constexpr auto fury_cutter_power() const {
		// 10 * 2 ^ n
		auto const result = 10_bi << bounded::min(m_consecutive_successes, 4_bi);
		static_assert(result.min() == 10_bi);
		static_assert(result.max() == 160_bi);
		return result;
	}

	constexpr auto momentum_move_power() const {
		auto const result = 30_bi << bounded::min(m_consecutive_successes, 4_bi);
		static_assert(result.min() == 30_bi);
		static_assert(result.max() == 480_bi);
		return result;
	}

	constexpr auto triple_kick_power() const {
		auto const result = 10_bi * bounded::min(m_consecutive_successes + 1_bi, 3_bi);
		static_assert(result.min() == 10_bi);
		static_assert(result.max() == 30_bi);
		return result;
	}

	// TODO: Does Metronome boost Struggle?
	constexpr auto metronome_boost() const {
		return rational(10_bi + m_consecutive_successes, 10_bi);
	}

	friend constexpr auto operator==(LastUsedMove const lhs, LastUsedMove const rhs) {
		return
			lhs.m_move == rhs.m_move and
			lhs.m_consecutive_successes == rhs.m_consecutive_successes;
	}

private:
	Moves m_move = Moves::Switch0;
	bounded::clamped_integer<0, 10> m_consecutive_successes = 0_bi;
};

}	// namespace technicalmachine
