// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.weather;

import numeric_traits;
import std_module;

namespace technicalmachine {

export enum class Weather : std::uint8_t {
	clear,
	hail,
	sand,
	sun,
	rain
};

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::Weather> = technicalmachine::Weather::clear;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::Weather> = technicalmachine::Weather::rain;
