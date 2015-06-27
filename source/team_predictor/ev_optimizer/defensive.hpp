// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2015 David Stone
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

#include "defensive_data_point.hpp"

#include "../../stat/nature.hpp"

#include <unordered_map>
#include <vector>

namespace technicalmachine {
struct Pokemon;
struct OffensiveEVs;
struct SpeedEVs;

struct DefensiveEVs {
	explicit DefensiveEVs(Pokemon const & pokemon);
	typedef std::unordered_map<Nature, DataPoint> BestPerNature;
private:
	void remove_inefficient_natures(std::vector<Nature> const & divided_natures);
	void add_other_potential_natures();
	friend void combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon);
	BestPerNature container;
};

}	// namespace technicalmachine
