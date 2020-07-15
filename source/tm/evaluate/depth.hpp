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
#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/variant/visit.hpp>
#include <bounded/detail/overload.hpp>

namespace technicalmachine {

struct DepthValues {
	unsigned general;
	unsigned single;
	friend constexpr auto operator<=>(DepthValues const &, DepthValues const &) = default;
};

struct Depth {
	constexpr Depth(DepthValues const to_search, unsigned const max_print_depth):
		m_to_search(to_search),
		m_searched_so_far(m_to_search.general == 0U ? SearchedSoFar(SingleDepth{0U}) : SearchedSoFar(GeneralDepth{0U})),
		m_max_print_depth(max_print_depth)
	{
	}
	
	constexpr auto general_initial() const {
		return m_to_search.general;
	}
	constexpr auto single_initial() const {
		return m_to_search.single;
	}

	constexpr auto remaining() const {
		return bounded::visit(m_searched_so_far, bounded::overload(
			[&](GeneralDepth const depth) { return DepthValues{m_to_search.general - depth.value, m_to_search.single}; },
			[&](SingleDepth const depth) { return DepthValues{0U, m_to_search.single - depth.value}; }
		));
	}
	
	constexpr auto indentation() const -> bounded::optional<unsigned> {
		return bounded::visit(m_searched_so_far, bounded::overload(
			[&](GeneralDepth const depth) { return BOUNDED_CONDITIONAL(depth.value >= m_max_print_depth, bounded::none, m_indentation); },
			[](SingleDepth) { return bounded::optional<unsigned>(bounded::none); }
		));
	}
	
	constexpr auto increased_indentation(Moves const move) const {
		auto copy = *this;
		if (move != Moves::Pass) {
			++copy.m_indentation;
		}
		return copy;
	}
	constexpr auto one_level_deeper() const;
private:
	struct GeneralDepth {
		unsigned value;
	};
	struct SingleDepth {
		unsigned value;
	};
	using SearchedSoFar = bounded::variant<GeneralDepth, SingleDepth>;

	DepthValues m_to_search;
	SearchedSoFar m_searched_so_far;
	unsigned m_max_print_depth;
	unsigned m_indentation = 1U;
};

struct SingleOnlyDepth {
	Depth value;
};
struct FinishedSearching {
};

constexpr auto Depth::one_level_deeper() const {
	auto copy = *this;
	using Result = bounded::variant<Depth, SingleOnlyDepth, FinishedSearching>;
	return bounded::visit(copy.m_searched_so_far, bounded::overload(
		[&](GeneralDepth & depth) {
			++depth.value;
			auto const is_single = depth.value == m_to_search.general;
			auto const is_none = is_single and copy.m_to_search.single == 0U;
			if (is_none) {
				return Result(FinishedSearching());
			}
			if (is_single) {
				insert(copy.m_searched_so_far, SingleDepth{0U});
				return Result(SingleOnlyDepth{copy});
			}
			return Result(copy);
		},
		[&](SingleDepth & depth) {
			++depth.value;
			return depth.value == m_to_search.single ? Result(FinishedSearching()) : Result(SingleOnlyDepth{copy});
		}
	));
}

constexpr auto iterative_deepening_value(Depth const original_depth) {
	auto const remaining = original_depth.remaining();
	return BOUNDED_CONDITIONAL(
		remaining.general > 1U,
		Depth(DepthValues{remaining.general - 1U, remaining.single}, 0U),
		bounded::none
	);
}


} // namespace technicalmachine
