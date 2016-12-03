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

#include "../operators.hpp"

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
template<typename Numerator, typename Denominator>
struct bounded_rational;

using namespace bounded::literal;

struct LastUsedMove {
	using index_type = bounded::checked_integer<0, 3>;
	auto has_moved() const -> bool;
	auto was_used_last(index_type index_of_move) const -> bool;
	auto increment(index_type index_of_move) -> void;
	auto fury_cutter_power() const -> bounded::integer<10, 160>;
	auto momentum_move_power() const -> bounded::integer<30, 480>;
	auto triple_kick_power() const -> bounded::integer<0, 30>;
	auto metronome_boost() const -> bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>>;
	friend auto operator==(LastUsedMove lhs, LastUsedMove rhs) -> bool;

private:
	bounded::optional<index_type> m_index_of_move = bounded::none;
	bounded::clamped_integer<0, 10> m_consecutive_turns_used = 0_bi;
};

}	// namespace technicalmachine
