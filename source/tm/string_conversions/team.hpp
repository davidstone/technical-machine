// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/pokemon.hpp>

#include <tm/generation.hpp>

#include <containers/single_element_range.hpp>
#include <containers/string.hpp>

namespace technicalmachine {

template<Generation>
struct Team;

template<Generation generation>
auto to_string(Team<generation> const & team, bool const include_owner = true) -> containers::string {
	containers::string output;
	if (include_owner) {
		output = containers::concatenate<containers::string>(std::move(output), team.who(), std::string_view("'s team:\n"));
	}
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::single_element_range('\n')
		);
	}
	return output;
}


}	// namespace technicalmachine
