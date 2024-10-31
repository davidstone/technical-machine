// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.evaluate.depth;

import tm.move.legal_selections;

import bounded;
import std_module;
import tv;

namespace technicalmachine {

using namespace bounded::literal;

export using DepthInt = bounded::integer<0, 10'000>;

// TODO: Should be a minimum of 1, but `LegalSelections` doesn't expose that yet
export using OptionsCount = bounded::integer<
	0,
	bounded::normalize<maximum_possible_selections>
>;

constexpr auto reduce_specific(DepthInt & value, OptionsCount const amount) -> void {
	value = bounded::max(0_bi, value - amount);
}

export enum class SearchType {
	full,
	single,
	evaluate
};

export struct Depth {
	constexpr Depth(DepthInt const general_, DepthInt const single_):
		general(general_),
		single(single_)
	{
	}

	constexpr auto search_type(this Depth const depth) -> SearchType {
		if (depth.general) {
			BOUNDED_ASSERT(*depth.general > 0_bi);
			return SearchType::full;
		} else if (depth.single > 0_bi) {
			return SearchType::single;
		} else {
			return SearchType::evaluate;
		}
	}

	constexpr auto remaining_general(this Depth const depth) -> DepthInt {
		return depth.general ? *depth.general : 0_bi;
	}

	constexpr auto reduced(this Depth depth, OptionsCount const amount) -> Depth {
		if (depth.general) {
			reduce_specific(*depth.general, amount);
		} else {
			reduce_specific(depth.single, amount);
		}
		return depth;
	}
	constexpr auto one_level_deeper(this Depth const original) -> Depth {
		// 3 chosen arbitrarily
		auto result = original.reduced(3_bi);
		if (result.general and *result.general == 0_bi) {
			result.general = tv::none;
		}
		return result;
	}

	friend constexpr auto operator<=>(Depth const lhs, Depth const rhs) {
		if (auto const cmp = lhs.remaining_general() <=> rhs.remaining_general(); cmp != 0) {
			return cmp;
		}
		return lhs.single <=> rhs.single;
	}
	friend constexpr auto operator==(Depth, Depth) -> bool = default;
private:
	tv::optional<DepthInt> general;
	DepthInt single;
};

} // namespace technicalmachine
