// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/evaluate/best_move.hpp>

#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/index_type.hpp>
#include <containers/is_empty.hpp>
#include <containers/size.hpp>

#include <random>

namespace technicalmachine {

template<Generation generation>
constexpr auto random_selection(Team<generation> const & user, Team<generation> const & other, Weather const weather, std::mt19937 & random_engine) -> BestMove {
	auto const possible = legal_selections(user, other, weather);
	BOUNDED_ASSERT(!containers::is_empty(possible));
	auto distribution = std::uniform_int_distribution(0, static_cast<int>(containers::size(possible) - 1_bi));
	auto const index = bounded::assume_in_range<containers::index_type<LegalSelections>>(distribution(random_engine));
	return BestMove{
		possible[index],
		0.0
	};
}

} // namespace technicalmachine
