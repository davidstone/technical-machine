// Optimize defensive EVs and nature to remove waste
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

#ifndef TEAM_PREDICTOR__EV_OPTIMIZER__DEFENSIVE_DATA_POINT_HPP_
#define TEAM_PREDICTOR__EV_OPTIMIZER__DEFENSIVE_DATA_POINT_HPP_

#include <string>

#include <bounded_integer/bounded_integer.hpp>

#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

namespace technicalmachine {
class Pokemon;
class SingleClassificationEVs;
class OffensiveEVs;
class DefensiveEVs;
class SpeedEVs;

class DataPoint {
public:
	DataPoint(SingleClassificationEVs const & physical, SingleClassificationEVs const & special);
	DataPoint(DataPoint const & original, Nature const & new_nature);
	std::string to_string() const;
	bounded_integer::native_integer<0, EV::max * 3> sum() const;
	friend bool lesser_product(DataPoint const & lhs, DataPoint const & rhs, Pokemon pokemon);
	static Nature::Natures get_nature(SingleClassificationEVs const & physical, SingleClassificationEVs const & special);
private:
	void update_pokemon(Pokemon & pokemon) const;
	friend void combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon);
	EV hp;
	EV defense;
	EV special_defense;
	Nature nature;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__EV_OPTIMIZER__DEFENSIVE_DATA_POINT_HPP_
