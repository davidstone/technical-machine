// Optimize defensive EVs and nature to remove waste
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

#include "defensive_data_point.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>

#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include "../../string_conversions/nature.hpp"

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

DataPoint::DataPoint(DataPoint const & original, Nature const & new_nature):
	hp(original.hp),
	defense(original.defense),
	special_defense(original.special_defense),
	nature(new_nature)
	{
}

std::string DataPoint::to_string() const {
	return ::technicalmachine::to_string(nature.name) + " " + bounded_integer::to_string(hp.value()) + " HP / " + bounded_integer::to_string(defense.value()) + " Def / " + bounded_integer::to_string(special_defense.value()) + " SpD";
}

bounded_integer::native_integer<0, EV::max * 3> DataPoint::sum() const {
	return hp.value() + defense.value() + special_defense.value();
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
	::technicalmachine::get_nature(pokemon) = nature;
	get_stat(pokemon, Stat::HP).ev = hp;
	get_stat(pokemon, Stat::DEF).ev = defense;
	get_stat(pokemon, Stat::SPD).ev = special_defense;
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
