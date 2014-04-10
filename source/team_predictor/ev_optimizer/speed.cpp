// Optimize Speed EVs and nature to remove waste
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

#include "speed.hpp"
#include "../../pokemon/pokemon.hpp"
#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat_names.hpp"

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

SpeedEVs::SpeedEVs(Pokemon pokemon) {
	Stat & stat = get_stat(pokemon, StatNames::SPE);
	Level const level = get_level(pokemon);
	Nature & current_nature = get_nature(pokemon);
	auto const speed = initial_stat<StatNames::SPE>(stat, level, current_nature);
	for (Nature::Natures nature = static_cast<Nature::Natures>(0); nature != Nature::END; nature = static_cast<Nature::Natures>(nature + 1)) {
		current_nature.name = nature;
		for (EV::value_type ev = 0_bi; ; ev += 4_bi) {
			stat.ev = EV(ev);
			if (initial_stat<StatNames::SPE>(stat, level, current_nature) >= speed) {
				container.insert(Container::value_type(nature, EV(ev)));
				break;
			}
			if (ev == bounded::make<EV::max>()) {
				break;
			}
		}
	}
	assert(!container.empty());
}

}	// namespace technicalmachine
