// Optimize defensive EVs and nature to remove waste
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

#include <bounded/integer.hpp>

#include "../../pokemon/species_forward.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/hp.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
struct Pokemon;
struct SingleClassificationEVs;
using namespace bounded::literal;

struct DataPoint {
	DataPoint(SingleClassificationEVs physical, SingleClassificationEVs special);
	DataPoint(DataPoint original, Nature new_nature);

	EV hp;
	EV defense;
	EV special_defense;
	Nature nature;
};

inline auto ev_sum(DataPoint const value) {
	return value.hp.value() + value.defense.value() + value.special_defense.value();
}

bool lesser_product(DataPoint lhs, DataPoint rhs, Pokemon const & pokemon);

}	// namespace technicalmachine
