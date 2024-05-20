// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.all_selections_valid;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;
import tm.move.switch_;

import tm.generation;
import tm.team;

import containers;
import tv;

namespace technicalmachine {

template<Generation generation>
constexpr auto selection_is_valid(Team<generation> const & team, Selection const selection) -> bool {
	return tv::visit(selection, tv::overload(
		[&](MoveName const move) {
			return move == MoveName::Struggle or containers::any_equal(team.pokemon().regular_moves(), move);
		},
		[&](Switch const switch_) {
			return switch_.value() < team.size() and switch_.value() != team.all_pokemon().index();
		},
		[](Pass) { return true; }
	));
}

export template<Generation generation>
constexpr auto all_selections_valid(Team<generation> const & team, LegalSelections const selections) -> bool {
	return containers::all(selections, [&](Selection const selection) { return selection_is_valid(team, selection); });
}

} // namespace technicalmachine
