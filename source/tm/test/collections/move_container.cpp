// Test container for moves
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/container.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/string_conversions/move.hpp>

#include <tm/generation.hpp>

#include <containers/algorithms/compare.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/array.hpp>
#include <containers/integer_range.hpp>
#include <containers/push_back.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto generation = Generation::four;

auto create_shared_moves(TeamSize const team_size) {
	auto shared = containers::static_vector<Moves, number_of_weird_moves + numeric_traits::max_value<TeamSize>>({
		Moves::Pass,
		Moves::Struggle,
	});
	if (team_size != 1_bi) {
		for (auto const n : containers::integer_range(team_size)) {
			containers::push_back(shared, to_switch(n));
		}
	}
	return shared;
}

TEST_CASE("MoveContainer", "[MoveContainer]") {
	constexpr auto team_size = TeamSize(4_bi);
	constexpr auto shared_moves_size = team_size + number_of_weird_moves;
	auto regular = RegularMoves();
	auto make_move_container = [&] {
		return MoveContainer<generation>(regular, team_size);
	};
	CHECK(containers::size(make_move_container()) == shared_moves_size);
	constexpr auto moves = containers::array{
		Moves::Absorb,
		Moves::Tackle,
		Moves::Quick_Attack,
		Moves::Body_Slam
	};
	for (auto const n : containers::integer_range(containers::size(moves))) {
		regular.push_back(Move(generation, moves[n]));
		auto const c = make_move_container();
		CHECK(containers::size(c.regular()) == n + 1_bi);
		CHECK(containers::size(c) == containers::size(c.regular()) + shared_moves_size);
	}
	auto const expected = containers::concatenate_view(moves, create_shared_moves(team_size));
	CHECK(containers::equal(make_move_container(), expected));
}

} // namespace
} // namespace technicalmachine
