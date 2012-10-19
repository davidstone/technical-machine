// Optimize Speed EVs and nature to remove waste
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

#include "speed.hpp"
#include "../../pokemon/pokemon.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {

SpeedEVs::SpeedEVs(Pokemon pokemon) {
	unsigned const speed = initial_stat<Stat::SPE>(pokemon);
	for (auto const nature : { Nature::JOLLY, Nature::HARDY, Nature::QUIET }) {
		pokemon.nature().name = nature;
		for (unsigned ev = 0; ev <= 252; ++ev) {
			pokemon.spe().ev.set_value(ev);
			if (initial_stat<Stat::SPE>(pokemon) >= speed) {
				container.insert(std::make_pair(nature, ev));
				break;
			}
		}
	}
}

}	// namespace technicalmachine
