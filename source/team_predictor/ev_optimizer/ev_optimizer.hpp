// Optimize EVs and nature to remove waste
// Copyright (C) 2013 David Stone
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

#ifndef TEAM_PREDICTOR__EV_OPTIMIZER_HPP_
#define TEAM_PREDICTOR__EV_OPTIMIZER_HPP_

#include <initializer_list>
#include <random>
#include "../../stat/stat_names.hpp"

namespace technicalmachine {
class Pokemon;

inline constexpr std::initializer_list<StatNames> regular_stats() {
	return { StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE };
}

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine);
void minimize_evs(Pokemon & pokemon);
void pad_random_evs(Pokemon & pokemon, std::mt19937 & random_engine);

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__EV_OPTIMIZER_HPP_
