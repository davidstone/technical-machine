// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stat_style;

import tm.generation;

import numeric_traits;

namespace technicalmachine {

// Split special for base stat, DV / IV, and EV
export enum class StatStyle { gen1, gen2, current };
export constexpr auto stat_style_for(Generation const generation) -> StatStyle {
	switch (generation) {
		case Generation::one:
			return StatStyle::gen1;
		case Generation::two:
			return StatStyle::gen2;
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return StatStyle::current;
	}
}

// Split special for DVs / IVs and EVs
export enum class SpecialInputStyle { combined, split };
export constexpr auto special_input_style_for(Generation const generation) -> SpecialInputStyle {
	switch (generation) {
		case Generation::one:
		case Generation::two:
			return SpecialInputStyle::combined;
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return SpecialInputStyle::split;
	}
}

export constexpr auto to_input_style(StatStyle const style) -> SpecialInputStyle {
	switch (style) {
		case StatStyle::gen1:
		case StatStyle::gen2:
			return SpecialInputStyle::combined;
		case StatStyle::current:
			return SpecialInputStyle::split;
	}
}

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::StatStyle> = technicalmachine::StatStyle::gen1;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::StatStyle> = technicalmachine::StatStyle::current;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SpecialInputStyle> = technicalmachine::SpecialInputStyle::combined;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SpecialInputStyle> = technicalmachine::SpecialInputStyle::split;