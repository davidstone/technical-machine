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
using namespace containers::string_literals;

export constexpr auto check_weathers_match(Weather const received, Weather const expected) -> void {
	if (received != expected) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Inconsistent weather. Received "_s,
			to_string(received),
			" but expected "_s,
			to_string(expected),
			"\n"_s
		));
	}
}

} // namespace technicalmachine
