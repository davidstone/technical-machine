// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.team;

import tm.string_conversions.generation;
import tm.string_conversions.pokemon;

import tm.any_team;
import tm.generation;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export template<any_team TeamType>
constexpr auto to_string(TeamType const & team) -> containers::string {
	auto output = containers::concatenate<containers::string>(
		"Generation "sv,
		to_string(generation_from<TeamType>),
		" team:\n"sv
	);
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::array{'\n'}
		);
	}
	return output;
}

} // namespace technicalmachine
