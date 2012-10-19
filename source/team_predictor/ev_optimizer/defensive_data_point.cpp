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

#include <cstdint>
#include <stdexcept>
#include <string>

#include "single_classification_evs.hpp"

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

DataPoint::DataPoint(SingleClassificationEVs const & physical, SingleClassificationEVs const & special):
	hp(physical.hp),
	defense(physical.defensive),
	special_defense(special.defensive),
	nature(get_nature(physical, special)) {
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

bool DataPoint::affects_defensive_stat(bool const boost) const {
	if (boost) {
		switch (nature.name) {
		case Nature::CALM:
		case Nature::IMPISH:
		case Nature::GENTLE:
		case Nature::LAX:
			return true;
		default:
			return false;
		}
	}
	else {
		switch (nature.name) {
		case Nature::HASTY:
		case Nature::NAIVE:
			return true;
		default:
			return false;
		}
	}
}

class InvalidNatureCombination : public std::logic_error {
	public:
		InvalidNatureCombination():
			std::logic_error("Attempt to create a nature that cannot exist.") {
		}
};

Nature::Natures DataPoint::get_nature(SingleClassificationEVs const & physical, SingleClassificationEVs const & special) {
	switch (physical.nature_boost) {
	case SingleClassificationEVs::Boost:
		switch (special.nature_boost) {
		case SingleClassificationEVs::Boost:
			throw InvalidNatureCombination();
		default:
			return Nature::IMPISH;
		case SingleClassificationEVs::Penalty:
			return Nature::LAX;
		}
	default:
		switch (special.nature_boost) {
		case SingleClassificationEVs::Boost:
			return Nature::CALM;
		default:
			return Nature::HARDY;
		case SingleClassificationEVs::Penalty:
			return Nature::NAIVE;
		}
	case SingleClassificationEVs::Penalty:
		switch (special.nature_boost) {
		case SingleClassificationEVs::Boost:
			return Nature::GENTLE;
		default:
			return Nature::HASTY;
		case SingleClassificationEVs::Penalty:
			throw InvalidNatureCombination();
		}
	}
}

}	// namespace technicalmachine
