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

#include "single_classification_evs.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <containers/algorithms/find.hpp>

namespace technicalmachine {

EqualDefensiveness equal_defensiveness(Pokemon const & pokemon, bool const physical) {
	auto const stat_name = physical ? StatNames::DEF : StatNames::SPD;
	auto const stat = get_stat(pokemon, stat_name);
	auto const level = get_level(pokemon);
	auto const initial_product = get_hp(pokemon).max() * initial_stat(stat_name, stat, level, get_nature(pokemon));
	auto result = EqualDefensiveness{};
	for (auto const nature : containers::enum_range<Nature>()) {
		for (auto const hp_ev : containers::integer_range(0_bi, EV::max + 4_bi, 4_bi)) {
			auto const hp = HP(pokemon, level, EV(hp_ev));
			auto minimum_ev = [=](auto const stat_ev) {
				return initial_stat(stat_name, Stat(stat, EV(stat_ev)), level, nature) * hp.max() >= initial_product;
			};
			if (auto const ptr = containers::maybe_find_if(containers::integer_range(0_bi, EV::max + 4_bi, 4_bi), minimum_ev)) {
				result.emplace_back(hp.ev(), EV(*ptr), nature);
			}
		}
	}
	return result;
}

}	// namespace technicalmachine
