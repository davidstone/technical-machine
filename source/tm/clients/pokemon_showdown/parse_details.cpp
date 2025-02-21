// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string_view>

export module tm.clients.ps.parse_details;

import tm.clients.ps.parse_gender;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.string_conversions.species;

import tm.buffer_view;
import tm.gender;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

struct ParsedDetails {
	Species species;
	Level level;
	Gender gender;
};
export constexpr auto parse_details(std::string_view const details) -> ParsedDetails {
	auto parser = DelimitedBufferView(details, std::string_view(", "));
	auto const species = from_string<Species>(parser.pop());

	auto const level_or_gender_or_shiny_str = parser.pop();
	auto const has_level = !level_or_gender_or_shiny_str.empty() and level_or_gender_or_shiny_str.front() == 'L';
	auto const level = Level(has_level ?
		bounded::to_integer<Level::value_type>(level_or_gender_or_shiny_str.substr(1)) :
		100_bi
	);

	auto const gender_or_shiny_str = has_level ? parser.pop() : level_or_gender_or_shiny_str;
	auto const gender = parse_gender(gender_or_shiny_str);

	auto const shiny_str = gender != Gender::genderless ? parser.pop() : gender_or_shiny_str;
	auto throw_exception = [&] {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid PS details string: "sv, details));
	};
	if (shiny_str != "shiny" and shiny_str != "") {
		throw_exception();
	}
	if (!parser.remainder().empty()) {
		throw_exception();
	}
	return ParsedDetails{species, level, gender};
}

} // namespace technicalmachine::ps
