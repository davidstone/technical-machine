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

#include "single_classification_evs.hpp"

#include "../../pokemon/level.hpp"
#include "../../pokemon/species_forward.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/hp.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct DataPoint {
	constexpr DataPoint(SingleClassificationEVs const physical, SingleClassificationEVs const special):
		hp(physical.hp),
		defense(physical.defensive),
		special_defense(special.defensive),
		nature(physical.nature)
	{
		assert(physical.nature == special.nature);
		assert(physical.hp == special.hp);
	}

	constexpr DataPoint(DataPoint const original, Nature const new_nature):
		hp(original.hp),
		defense(original.defense),
		special_defense(original.special_defense),
		nature(new_nature)
	{
	}

	EV hp;
	EV defense;
	EV special_defense;
	Nature nature;
};

constexpr auto ev_sum(DataPoint const value) {
	return value.hp.value() + value.defense.value() + value.special_defense.value();
}

inline auto defensive_product(DataPoint const value, Level const level, Species const species) {
	auto single_product = [=](StatNames const stat) {
		auto const defensive = (stat == StatNames::DEF) ? value.defense : value.special_defense;
		auto const initial = initial_stat(stat, Stat(species, stat, defensive), level, value.nature);
		return initial * HP(species, level, value.hp).max();
	};

	return single_product(StatNames::DEF) * single_product(StatNames::SPD);
}

}	// namespace technicalmachine
