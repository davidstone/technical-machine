// Keeps track of the last used move
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

#pragma once

#include "../hash.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

namespace technicalmachine {
template<typename Numerator, typename Denominator>
class bounded_rational;

using namespace bounded::literal;

class LastUsedMove {
public:
	using index_type = bounded::checked_integer<0, 3>;
	auto has_moved() const -> bool;
	auto was_used_last(index_type index_of_move) const -> bool;
	auto increment(index_type index_of_move) -> void;
	auto fury_cutter_power() const -> bounded::integer<10, 160>;
	auto momentum_move_power() const -> bounded::integer<30, 480>;
	auto triple_kick_power() const -> bounded::integer<0, 30>;
	auto metronome_boost() const -> bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>>;
	friend auto operator==(LastUsedMove lhs, LastUsedMove rhs) -> bool;

	constexpr auto hash() const noexcept {
		return ::technicalmachine::hash(m_index_of_move, m_consecutive_turns_used);
	}
private:
	bounded::optional<index_type> m_index_of_move;
	bounded::clamped_integer<0, 10> m_consecutive_turns_used = 0_bi;
};

auto operator!=(LastUsedMove lhs, LastUsedMove rhs) -> bool;

constexpr auto hash(LastUsedMove const last_used_move) noexcept {
	return last_used_move.hash();
}

}	// namespace technicalmachine
