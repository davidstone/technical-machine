// Optimize Speed EVs and nature to remove waste
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

#include "speed.hpp"
#include "../../pokemon/pokemon.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
using namespace bounded_integer::literal;

SpeedEVs::SpeedEVs(Pokemon pokemon) {
	unsigned const speed = initial_stat<Stat::SPE>(pokemon);
	for (Nature::Natures nature = static_cast<Nature::Natures>(0); nature != Nature::END; nature = static_cast<Nature::Natures>(nature + 1)) {
		get_nature(pokemon).name = nature;
		for (EV::value_type ev = 0_bi; ; ev += 4_bi) {
			get_stat(pokemon, Stat::SPE).ev = EV(ev);
			if (initial_stat<Stat::SPE>(pokemon) >= speed) {
				container.insert(Container::value_type(nature, EV(ev)));
				break;
			}
			if (ev == bounded_integer::make_bounded<EV::max>()) {
				break;
			}
		}
	}
}

}	// namespace technicalmachine
