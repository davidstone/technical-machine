// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.validate_generation;

import tm.string_conversions.generation;

import tm.generation;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export constexpr auto validate_generation(std::string_view const received, Generation const expected) -> void {
	auto const parsed = from_string<Generation>(received);
	if (parsed != expected) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Received wrong generation. Expected "sv,
			to_string(expected),
			"but got "sv,
			received
		));
	}
}

} // namespace technicalmachine::ps
