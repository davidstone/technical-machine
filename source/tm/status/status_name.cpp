export module tm.status.status_name;

import numeric_traits;
import std_module;

namespace technicalmachine {

export enum class StatusName : std::uint8_t {
	clear,
	burn,
	freeze,
	paralysis,
	poison,
	toxic,
	sleep,
	rest
};

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::StatusName> = technicalmachine::StatusName();

template<>
constexpr auto numeric_traits::max_value<technicalmachine::StatusName> = technicalmachine::StatusName::rest;
