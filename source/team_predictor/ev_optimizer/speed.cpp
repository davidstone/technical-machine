// Optimize Speed EVs and nature to remove waste
// Copyright (C) 2015 David Stone
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
#include "../../enum_range.hpp"
#include "../../pokemon/pokemon.hpp"
#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat_names.hpp"
#include "../../string_conversions/nature.hpp"

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

SpeedEVs::SpeedEVs(Pokemon const & pokemon) {
	Stat stat = get_stat(pokemon, StatNames::SPE);
	Level const level = get_level(pokemon);
	auto const speed = initial_stat<StatNames::SPE>(stat, level, get_nature(pokemon));
	for (auto const nature : enum_range<Nature>) {
		for (EV::value_type ev = 0_bi; ; ev += 4_bi) {
			stat.ev = EV(ev);
			if (initial_stat<StatNames::SPE>(stat, level, nature) >= speed) {
				m_container.push_back({ nature, EV(ev) });
				break;
			}
			if (ev == EV::max) {
				break;
			}
		}
	}
	assert(!m_container.empty());
}

auto find(SpeedEVs const & container, Nature const nature) -> EV {
	auto const it = std::find_if(container.begin(), container.end(), [=](auto const & value) { return value.nature == nature; });
	if (it == container.end()) {
		throw InvalidNature(to_string(nature));
	}
	return it->ev;
}

}	// namespace technicalmachine
