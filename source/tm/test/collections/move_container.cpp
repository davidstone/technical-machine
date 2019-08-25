// Test container for moves
// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
#include <containers/static_vector/static_vector.hpp>
#include <containers/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

auto create_shared_moves(TeamSize const team_size) {
	auto shared = containers::static_vector<Moves, static_cast<int>(number_of_weird_moves + TeamSize::max())>{
		Moves::Pass,
		Moves::Struggle,
	};
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
	auto c = MoveContainer(team_size);
	if (size(c) != shared_moves_size) {
		throw InvalidCollection("MoveContainer has the wrong number of shared moves. Expecting " + bounded::to_string(shared_moves_size) + " but got " + bounded::to_string(size(c)));
	}
	constexpr auto moves = containers::array{
		Moves::Absorb, Moves::Tackle, Moves::Quick_Attack, Moves::Body_Slam
	};
	for (auto const n : containers::integer_range(size(moves))) {
		containers::emplace_back(c, generation, moves[n]);
		if (size(c) != shared_moves_size + n + 1_bi or size(c) != static_cast<bounded::checked_integer<0, 100>>(c.number_of_regular_moves()) + shared_moves_size) {
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
