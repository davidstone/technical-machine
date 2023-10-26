// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.evaluate.random_selection;

import tm.evaluate.scored_move;

import tm.move.legal_selections;

import tm.block;
import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
constexpr auto random_selection(Team<generation> const & user, Team<generation> const & other, Environment const environment, std::mt19937 & random_engine) -> ScoredMove {
	auto const possible = legal_selections(user, other, environment);
	BOUNDED_ASSERT(!containers::is_empty(possible));
	auto distribution = std::uniform_int_distribution<int>(0, static_cast<int>(containers::size(possible) - 1_bi));
	auto const index = bounded::assume_in_range<containers::index_type<LegalSelections>>(distribution(random_engine));
	return ScoredMove{
		possible[index],
		0.0
	};
}

} // namespace technicalmachine
