// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.selection;

import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.any_team;

import containers;
import std_module;
import tv;

namespace technicalmachine {
using namespace std::string_view_literals;

export constexpr auto to_string(
	Selection const selection,
	any_team auto const & team
) -> containers::string {
	return tv::visit(selection, tv::overload(
		[&](Switch const switch_) {
			return containers::concatenate<containers::string>(
				"Switch to "sv,
				to_string(team.pokemon(switch_.value()).species())
			);
		},
		[](MoveName const move) {
			return containers::concatenate<containers::string>(
				"Use "sv,
				to_string(move)
			);
		},
		[](Pass) {
			return containers::string("Pass"sv);
		}
	));
}

} // namespace technicalmachine
