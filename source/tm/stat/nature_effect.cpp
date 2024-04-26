// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.nature_effect;

import tm.rational;

import bounded;
import numeric_traits;
import std_module;

using namespace bounded::literal;

namespace technicalmachine {

export enum class NatureEffect : std::uint8_t {
	negative,
	neutral,
	positive
};

using NatureBoost = rational<bounded::integer<9, 11>, bounded::constant_t<10>>;
export constexpr auto boost(NatureEffect const effect) -> NatureBoost {
	switch (effect) {
		case NatureEffect::negative:
			return rational(9_bi, 10_bi);
		case NatureEffect::neutral:
			return rational(10_bi, 10_bi);
		case NatureEffect::positive:
			return rational(11_bi, 10_bi);
	}
}

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::NatureEffect> = technicalmachine::NatureEffect::negative;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::NatureEffect> = technicalmachine::NatureEffect::positive;