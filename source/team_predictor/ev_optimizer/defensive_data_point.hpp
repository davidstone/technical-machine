// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2014 David Stone
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

#include <bounded_integer/bounded_integer.hpp>

#include "../../pokemon/species_forward.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/hp.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
class Level;
class Pokemon;
class SingleClassificationEVs;
class OffensiveEVs;
class DefensiveEVs;
class SpeedEVs;
using namespace bounded::literal;

class DataPoint {
public:
	DataPoint(SingleClassificationEVs const & physical, SingleClassificationEVs const & special);
	DataPoint(DataPoint const & original, Nature const & new_nature);
	std::string to_string() const;
	auto sum() const {
		return hp.value() + defense.value() + special_defense.value();
	}
	friend bool lesser_product(DataPoint const & lhs, DataPoint const & rhs, Pokemon const & pokemon);
	static auto get_nature(SingleClassificationEVs const & physical, SingleClassificationEVs const & special) -> Nature;
private:
	void update_pokemon(Pokemon & pokemon) const;

	template<StatNames stat>
	auto product(Species const species, Level const & level) const {
		auto const initial = initial_stat<stat>(Stat(species, stat, defense), level, nature);
		return initial * HP(species, level, hp).max();
	}
	friend void combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon);
	EV hp;
	EV defense;
	EV special_defense;
	Nature nature;
};

}	// namespace technicalmachine
