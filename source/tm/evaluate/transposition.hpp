// Cache scores in case we re-evaluate the same position
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

#pragma once

#include <tm/weather.hpp>

#include <bounded/optional.hpp>

#include <containers/array/array.hpp>

namespace technicalmachine {

struct Team;

struct BestMove {
	Moves move;
	double score;
};

struct TranspositionTable {
	auto add_score(Team const & ai, Team const & foe, Weather, unsigned depth, BestMove) -> void;
	auto get_score(Team const & ai, Team const & foe, Weather, unsigned depth) const -> bounded::optional<BestMove>;
};

}	// namespace technicalmachine
