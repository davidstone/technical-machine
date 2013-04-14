// Optimize defensive EVs and nature to remove waste
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

#ifndef TEAM_PREDICTOR__EV_OPTIMIZER__SINGLE_CLASSIFICATION_EVS_HPP_
#define TEAM_PREDICTOR__EV_OPTIMIZER__SINGLE_CLASSIFICATION_EVS_HPP_

#include <string>
#include <vector>

namespace technicalmachine {
class Nature;
class Pokemon;

class SingleClassificationEVs {
public:
	enum NatureBoost { Penalty, Neutral, Boost };
	SingleClassificationEVs(unsigned hp_ev, unsigned defensive_ev, Nature nature, bool physical);
	std::string to_string() const;
	friend bool are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special, unsigned max_evs);
private:
	friend class DataPoint;
	std::string stat_name() const;
	unsigned hp;
	unsigned defensive;
	NatureBoost nature_boost;
	bool physical;
};

template<bool physical>
std::vector<SingleClassificationEVs> equal_defensiveness(Pokemon pokemon);

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__EV_OPTIMIZER__SINGLE_CLASSIFICATION_EVS_HPP_
