// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.parse_generation_from_format;

import tm.string_conversions.generation;

import tm.generation;

import containers;
import std_module;

namespace technicalmachine::ps {

using namespace std::string_view_literals;

constexpr auto is_digit = [](char const c) {
	return '0' <= c and c <= '9';
};

export constexpr auto parse_generation_from_format(std::string_view const battle_format, std::string_view const expected_prefix) -> Generation {
	if (!battle_format.starts_with(expected_prefix)) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected battle format "sv, battle_format, " to start with "sv, expected_prefix));
	}
	auto const remainder = battle_format.substr(expected_prefix.size());
	auto const it = containers::find_if(remainder, std::not_fn(is_digit));
	return from_string<Generation>(remainder.substr(0, static_cast<std::size_t>(it - remainder.begin())));
}

static_assert(parse_generation_from_format("gen5ou", "gen") == Generation::five);

} // namespace technicalmachine::ps
