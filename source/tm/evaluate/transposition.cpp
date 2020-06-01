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

#include <tm/evaluate/transposition.hpp>

#include <tm/team.hpp>

namespace technicalmachine {
namespace {

}	// namespace

auto TranspositionTable::add_score(Team const & ai, Team const & foe, Weather weather, unsigned depth, BestMove best_move) -> void {
	static_cast<void>(ai);
	static_cast<void>(foe);
	static_cast<void>(weather);
	static_cast<void>(depth);
	static_cast<void>(best_move);
}

auto TranspositionTable::get_score(Team const & ai, Team const & foe, Weather weather, unsigned depth) const -> bounded::optional<BestMove> {
	static_cast<void>(ai);
	static_cast<void>(foe);
	static_cast<void>(weather);
	static_cast<void>(depth);
	return bounded::none;
}

}	// namespace technicalmachine
