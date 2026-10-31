export module tm.clients.check_weathers_match;

import tm.string_conversions.weather;

import tm.weather;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export constexpr auto check_weathers_match(Weather const received, Weather const expected) -> void {
	if (received != expected) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Inconsistent weather. Received "_sv,
			to_string(received),
			" but expected "_sv,
			to_string(expected),
			"\n"_sv
		));
	}
}

} // namespace technicalmachine
