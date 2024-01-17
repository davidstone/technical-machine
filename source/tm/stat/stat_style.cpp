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

// Split special for DV / IV and EV
export enum class SpecialStyle { combined, split };

export constexpr auto special_style(StatStyle const style) -> SpecialStyle {
	switch (style) {
		case StatStyle::gen1:
		case StatStyle::gen2:
			return SpecialStyle::combined;
		case StatStyle::current:
			return SpecialStyle::split;
	}
}


export constexpr auto stat_style_for(Generation const generation) {
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

export constexpr auto special_style_for(Generation const generation) {
	switch (generation) {
		case Generation::one:
		case Generation::two:
			return SpecialStyle::combined;
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return SpecialStyle::split;
	}
}

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::StatStyle> = technicalmachine::StatStyle::gen1;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::StatStyle> = technicalmachine::StatStyle::current;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SpecialStyle> = technicalmachine::SpecialStyle::combined;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SpecialStyle> = technicalmachine::SpecialStyle::split;