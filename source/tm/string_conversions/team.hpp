// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/single_element_range.hpp>
#include <containers/string.hpp>

namespace technicalmachine {

using namespace std::string_view_literals;

template<any_team TeamType>
auto to_string(TeamType const & team, bool const include_owner = true) -> containers::string {
	auto output_owner = [&] {
		if constexpr (std::is_same_v<TeamType, KnownTeam<generation_from<TeamType>>>) {
			return "AI's "sv;
		} else if constexpr (std::is_same_v<TeamType, SeenTeam<generation_from<TeamType>>>) {
			return "Foe's "sv;
		} else {
			return containers::concatenate<containers::string>(team.who(), "'s "sv);
		}
	};
	auto output = containers::concatenate<containers::string>(
		include_owner ? output_owner() : "",
		"Generation "sv,
		to_string(generation_from<TeamType>),
		" team:\n"sv
	);
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::single_element_range('\n')
		);
	}
	return output;
}

} // namespace technicalmachine
