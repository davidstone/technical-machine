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

#include <bounded/integer.hpp>

#include "../../pokemon/species_forward.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/hp.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
struct Level;
struct Pokemon;
struct SingleClassificationEVs;
struct OffensiveEVs;
struct DefensiveEVs;
struct SpeedEVs;
using namespace bounded::literal;

struct DataPoint {
	DataPoint(SingleClassificationEVs physical, SingleClassificationEVs special);
	DataPoint(DataPoint original, Nature new_nature);
	auto sum() const {
		return hp.value() + defense.value() + special_defense.value();
	}
	friend bool lesser_product(DataPoint const & lhs, DataPoint const & rhs, Pokemon const & pokemon);
private:
	void update_pokemon(Pokemon & pokemon) const;

	template<StatNames stat>
	auto product(Species const species, Level const level) const {
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
