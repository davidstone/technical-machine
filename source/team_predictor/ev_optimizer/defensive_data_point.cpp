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

#include "defensive_data_point.hpp"

#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {

DataPoint::DataPoint(SingleClassificationEVs const physical, SingleClassificationEVs const special):
	hp(physical.hp),
	defense(physical.defensive),
	special_defense(special.defensive),
	nature(physical.nature)
{
	assert(physical.nature == special.nature);
	assert(physical.hp == special.hp);
}

DataPoint::DataPoint(DataPoint const original, Nature const new_nature):
	hp(original.hp),
	defense(original.defense),
	special_defense(original.special_defense),
	nature(new_nature)
{
}

bool lesser_product(DataPoint const lhs, DataPoint const rhs, Pokemon const & pokemon) {
	auto const level = get_level(pokemon);
	auto const species = static_cast<Species>(pokemon);

	auto product = [=](DataPoint const value, StatNames const stat) {
		auto const defensive = stat == StatNames::DEF ? value.defense : value.special_defense;
		auto const initial = initial_stat(stat, Stat(species, stat, defensive), level, value.nature);
		return initial * HP(species, level, value.hp).max();
	};

	auto const left = product(lhs, StatNames::DEF) * product(lhs, StatNames::SPD);
	auto const right = product(rhs, StatNames::DEF) * product(rhs, StatNames::SPD);
	return left < right;
}

}	// namespace technicalmachine
