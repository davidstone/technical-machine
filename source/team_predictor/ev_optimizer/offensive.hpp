// Optimize offensive EVs and nature to remove waste
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

#ifndef TEAM_PREDICTOR__EV_OPTIMIZER__OFFENSIVE_HPP_
#define TEAM_PREDICTOR__EV_OPTIMIZER__OFFENSIVE_HPP_

#include <map>
#include <vector>

#include <bounded_integer/bounded_integer.hpp>

#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

namespace technicalmachine {
class Pokemon;
class DefensiveEVs;
class SpeedEVs;
using namespace bounded_integer::literal;

class OffensiveStats {
public:
	OffensiveStats():
		attack(0_bi),
		special_attack(0_bi) {
	}
	bounded_integer::native_integer<0, EV::max * 2> sum() const {
		return attack.value() + special_attack.value();
	}
	EV attack;
	EV special_attack;
};

class OffensiveEVs {
public:
	explicit OffensiveEVs(Pokemon pokemon);
private:
	void optimize(Pokemon & pokemon);
	void remove_unused(Pokemon & pokemon);
	void equal_stats(Pokemon & pokemon);
	friend void combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon);
	typedef std::map<Nature::Natures, OffensiveStats> Container;
	Container container;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__EV_OPTIMIZER__OFFENSIVE_HPP_
