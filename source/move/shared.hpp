// Handle common moves that all Pokemon can select
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

#ifndef MOVE__SHARED_HPP_
#define MOVE__SHARED_HPP_

#include "../pokemon/max_pokemon_per_team.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <utility>

namespace technicalmachine {
class Move;
using namespace bounded_integer::literal;

class SharedMoves {
public:
	// Struggle is the only other special move right now
	static constexpr auto number_of_non_switches() noexcept -> decltype(1_bi) {
		return 1_bi;
	}
	using size_type = decltype(std::declval<TeamSize>() + number_of_non_switches());
	using index_type = decltype(std::declval<size_type>() - 1_bi);
	explicit SharedMoves(TeamSize team_size = max_pokemon_per_team);
	void remove_switch();
	Move const & operator[](index_type index) const;
	size_type size() const;
	friend bool operator==(SharedMoves const & lhs, SharedMoves const & rhs);
private:
	bounded_integer::equivalent_type<TeamSize, bounded_integer::throw_policy> m_number_of_switches;
};

}	// namespace technicalmachine
#endif	// MOVE__SHARED_HPP_
