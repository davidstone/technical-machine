// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.pokemon.potentially_selectable_moves;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.generation;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto weird_moves(Generation const generation) {
	return containers::array({
		Move(generation, MoveName::Struggle)
	});
}

export constexpr auto potentially_selectable_moves(Generation const generation, RegularMoves regular_moves) {
	return containers::concatenate_view(
		std::move(regular_moves),
		weird_moves(generation)
	);
}

constexpr auto make_all(auto... move_names) {
	constexpr auto generation = Generation::one;
	return potentially_selectable_moves(
		generation,
		RegularMoves({Move(generation, move_names)...})
	);
}

static_assert(containers::equal(
	make_all(
		MoveName::Tackle
	),
	containers::array({
		MoveName::Tackle,
		MoveName::Struggle
	})
));

static_assert(containers::equal(
	make_all(
		MoveName::Tackle
	),
	containers::array({
		MoveName::Tackle,
		MoveName::Struggle,
	})
));

static_assert(containers::equal(
	make_all(
		MoveName::Tackle,
		MoveName::Thunder,
		MoveName::Psychic,
		MoveName::Body_Slam
	),
	containers::array({
		MoveName::Tackle,
		MoveName::Thunder,
		MoveName::Psychic,
		MoveName::Body_Slam,
		MoveName::Struggle,
	})
));

} // namespace technicalmachine
