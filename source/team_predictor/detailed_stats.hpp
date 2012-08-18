// Detailed Pokemon stats
// Copyright (C) 2012 David Stone
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

#ifndef TEAM_PREDICTOR__DETAILED_STATS_HPP_
#define TEAM_PREDICTOR__DETAILED_STATS_HPP_

#include <array>
#include <cstdint>
#include <vector>

#include "../ability.hpp"
#include "../item.hpp"

#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;

class DetailedStats {
	public:
		DetailedStats();
		std::array<Ability::Abilities, static_cast<unsigned>(Species::END)> ability;
		std::array<Item::Items, static_cast<unsigned>(Species::END)> item;
		std::array<Nature::Natures, static_cast<unsigned>(Species::END)> nature;
		std::array<std::vector<Moves>, static_cast<unsigned>(Species::END)> move;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__DETAILED_STATS_HPP_
