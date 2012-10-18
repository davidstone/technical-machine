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

#include "defensive_data_point.hpp"

#include <algorithm>
#include <cstdint>
#include <string>

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
namespace {

template<Stat::Stats stat>
unsigned product(Pokemon const & pokemon) {
	return initial_stat<stat>(pokemon) * initial_stat<Stat::HP>(pokemon);
}

}	// unnamed namespace

DataPoint::DataPoint(unsigned hp_ev, unsigned defense_ev, unsigned special_defense_ev, Nature const a_nature):
	hp(hp_ev),
	defense(defense_ev),
	special_defense(special_defense_ev),
	nature(a_nature) {
}

std::string DataPoint::to_string() const {
	return nature.to_string() + " " + std::to_string(hp) + " HP / " + std::to_string(defense) + " Def / " + std::to_string(special_defense) + " SpD";
}

unsigned DataPoint::sum() const {
	return hp + defense + special_defense;
}

bool lesser_product(DataPoint const & lhs, DataPoint const & rhs, Pokemon pokemon) {
	lhs.update_pokemon(pokemon);
	auto const left_physical = product<Stat::DEF>(pokemon);
	auto const left_special = product<Stat::SPD>(pokemon);
	rhs.update_pokemon(pokemon);
	auto const right_physical = product<Stat::DEF>(pokemon);
	auto const right_special = product<Stat::SPD>(pokemon);
	if (left_physical < right_physical and left_special < right_special)
		return true;
	if (right_physical < left_physical and right_special < left_special)
		return false;
	auto const left = static_cast<uint64_t>(left_physical) * left_special;
	auto const right = static_cast<uint64_t>(right_physical) * right_special;
	return left < right;
}

void DataPoint::update_pokemon(Pokemon & pokemon) const {
	pokemon.nature() = nature;
	pokemon.hp().ev.set_value(hp);
	pokemon.def().ev.set_value(defense);
	pokemon.spd().ev.set_value(special_defense);
}
}	// namespace technicalmachine
