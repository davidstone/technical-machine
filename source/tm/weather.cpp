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
