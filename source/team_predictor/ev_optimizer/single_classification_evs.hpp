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

#include <string>
#include <vector>

#include "../../stat/ev.hpp"

namespace technicalmachine {
enum class Nature : uint8_t;
struct Pokemon;

struct SingleClassificationEVs {
	enum NatureBoost { Penalty, Neutral, Boost };
	SingleClassificationEVs(EV hp, EV defensive, Nature nature);
	friend bool are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special);
private:
	friend struct DataPoint;
	EV hp;
	EV defensive;
	NatureBoost nature_boost;
};

template<bool physical>
std::vector<SingleClassificationEVs> equal_defensiveness(Pokemon const & pokemon);

}	// namespace technicalmachine
