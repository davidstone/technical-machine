// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.check_weathers_match;

import tm.string_conversions.weather;

import tm.weather;

import containers;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;

export constexpr auto check_weathers_match(NormalWeather const received, Weather const expected_full) -> void {
	auto const expected = to_normal_weather(expected_full);
	if (received != expected) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Inconsistent weather. Received "sv,
			to_string(received),
			" but expected "sv,
			to_string(expected),
			"\n"sv
		));
	}
}

} // namespace technicalmachine
