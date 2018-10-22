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

#include "move_container.hpp"

#include "invalid_collection.hpp"

#include "../../move/container.hpp"
#include "../../move/moves.hpp"
#include "../../move/is_switch.hpp"

#include "../../string_conversions/move.hpp"

#include <containers/array/array.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

using ExpectedRegular = containers::array<Moves, 4>;
using ExpectedShared = containers::static_vector<Moves, static_cast<int>(number_of_weird_moves + TeamSize::max())>;

constexpr auto create_shared_moves(TeamSize const team_size) {
	auto shared = ExpectedShared{
		Moves::Struggle,
	};
	if (team_size != 1_bi) {
		for (auto const n : containers::integer_range(team_size)) {
			shared.emplace_back(to_switch(n));
		}
	}
	return shared;
}

struct Verify {
	Verify(ExpectedRegular m, TeamSize team_size):
		m_moves(std::move(m)),
		m_shared_moves(create_shared_moves(team_size)),
		m_index(0_bi) {
	}
	void operator()(Moves const move) {
		auto const mine = (m_index < size(m_moves)) ?
			at(m_moves, m_index) :
			at(m_shared_moves, m_index - size(m_moves));
		if (mine != move) {
			throw InvalidCollection(
				"MoveContainer has the wrong moves. Expected: " +
				std::string(to_string(mine)) +
				" but got " +
				std::string(to_string(move))
			);
		}
		++m_index;
	}
private:
	ExpectedRegular m_moves;
	ExpectedShared m_shared_moves;
	using Index = decltype(
		std::declval<containers::index_type<ExpectedRegular>>() +
		std::declval<containers::index_type<ExpectedShared>>()
	);
	Index m_index;
};

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
		c.emplace_back(moves[n]);
		if (size(c) != shared_moves_size + n + 1_bi or size(c) != static_cast<bounded::checked_integer<0, 100>>(c.number_of_regular_moves()) + shared_moves_size) {
			throw InvalidCollection("MoveContainer has the wrong number of moves during addition of moves.");
		}
	}
	auto verify = Verify(moves, team_size);
	for (auto const move : c) {
		verify(move);
	}
}

}	// namespace technicalmachine
