// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_generation_from_format;

import tm.string_conversions.generation;

import tm.generation;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

constexpr auto is_digit = [](char const c) {
	return '0' <= c and c <= '9';
};

export constexpr auto parse_generation_from_format(containers::string_view const battle_format, containers::string_view const expected_prefix) -> Generation {
	if (!containers::starts_with(battle_format, expected_prefix)) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Expected battle format "_s,
			battle_format,
			" to start with "_s,
			expected_prefix
		));
	}
	auto const remainder = containers::drop_exactly(battle_format, expected_prefix.size());
	return from_string<Generation>(containers::subrange(
		containers::begin(remainder),
		containers::find_if(remainder, std::not_fn(is_digit))
	));
}

static_assert(parse_generation_from_format("gen5ou"_s, "gen"_s) == Generation::five);

} // namespace technicalmachine::ps
