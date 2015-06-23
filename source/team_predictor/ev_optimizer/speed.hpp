// Optimize Speed EVs and nature to remove waste
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

#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

#include <unordered_map>

namespace technicalmachine {
class Pokemon;
class OffensiveEVs;
class DefensiveEVs;

class SpeedEVs {
public:
	explicit SpeedEVs(Pokemon const & pokemon);
private:
	friend void combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon);
	using Container = std::unordered_map<Nature, EV>;
	Container container;
};

}	// namespace technicalmachine
