// Copyright (C) 2018 David Stone
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

#include <bounded/assert.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {

struct Depth {
	constexpr Depth(unsigned const depth_to_search_, unsigned max_print_depth):
		m_searched_so_far(0U),
		m_depth_to_search(depth_to_search_),
		m_max_print_depth(max_print_depth),
		m_indentation(1U)
	{
	}
	
	constexpr auto depth_to_search() const {
		return m_depth_to_search;
	}
	
	constexpr auto is_final_iteration() const {
		return m_depth_to_search - 1U == m_searched_so_far;
	}
	constexpr auto indentation() const -> bounded::optional<unsigned> {
		if (m_searched_so_far >= m_max_print_depth) {
			return bounded::none;
		}
		return m_indentation;
	}
	
	constexpr auto increased_indentation(Moves const move) const {
		auto copy = *this;
		if (move != Moves::Pass) {
			++copy.m_indentation;
		}
		return copy;
	}
	constexpr auto one_level_deeper() const {
		auto copy = *this;
		++copy.m_searched_so_far;
		return copy;
	}
	constexpr auto iterative_deepening_value() const {
		return Depth(next_depth_to_search(), 0U);
	}
private:
	constexpr auto next_depth_to_search() const -> unsigned {
		BOUNDED_ASSERT(m_depth_to_search != m_searched_so_far);
		return m_depth_to_search - m_searched_so_far - 1U;
	}

	unsigned m_searched_so_far;
	unsigned m_depth_to_search;
	unsigned m_max_print_depth;
	unsigned m_indentation;
};

}	// namespace technicalmachine
