// Test container for moves
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/collections/move_container.hpp>

#include <tm/test/collections/invalid_collection.hpp>

#include <tm/move/container.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/string_conversions/move.hpp>

#include <tm/generation.hpp>

#include <containers/algorithms/compare.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/array/array.hpp>
#include <containers/integer_range.hpp>
#include <containers/push_back.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

auto create_shared_moves(TeamSize const team_size) {
	auto shared = containers::static_vector<Moves, static_cast<int>(number_of_weird_moves + numeric_traits::max_value<TeamSize>)>({
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

}	// namespace

void move_container_tests() {
	std::cout << "\tRunning MoveContainer tests.\n";
	constexpr auto team_size = TeamSize(4_bi);
	constexpr auto shared_moves_size = team_size + number_of_weird_moves;
	auto regular = RegularMoves();
	auto c = MoveContainer<generation>(regular, team_size);
	if (containers::size(c) != shared_moves_size) {
		throw InvalidCollection("MoveContainer has the wrong number of shared moves. Expecting " + bounded::to_string(shared_moves_size) + " but got " + bounded::to_string(containers::size(c)));
	}
	constexpr auto moves = containers::array{
		Moves::Absorb, Moves::Tackle, Moves::Quick_Attack, Moves::Body_Slam
	};
	for (auto const n : containers::integer_range(containers::size(moves))) {
		regular.push_back(Move(generation, moves[n]));
		if (containers::size(c) != shared_moves_size + n + 1_bi or containers::size(c) != static_cast<bounded::integer<0, 100>>(containers::size(c.regular()) + shared_moves_size)) {
			throw InvalidCollection("MoveContainer has the wrong number of moves during addition of moves.");
		}
	}
	auto const expected = containers::concatenate_view(moves, create_shared_moves(team_size));
	if (!containers::equal(c, expected)) {
		auto print_container = [](auto const & container) {
			for (auto const move : container) {
				std::cerr << to_string(move) << ", ";
			}
			std::cerr << '\n';
		};
		std::cerr << "MoveContainer has the wrong moves.\nExpected: ";
		print_container(expected);
		std::cerr << "Got: ";
		print_container(containers::transform(c, &Move::name));
		std::terminate();
	}
}

}	// namespace technicalmachine
