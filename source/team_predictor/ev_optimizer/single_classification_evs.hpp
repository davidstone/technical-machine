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

#pragma once

#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

#include <containers/static_vector/static_vector.hpp>

#include <string>

namespace technicalmachine {
struct Pokemon;

struct SingleClassificationEVs {
	EV hp;
	EV defensive;
	Nature nature;
};

constexpr auto are_compatible(SingleClassificationEVs const & physical, SingleClassificationEVs const & special) -> bool {
	return physical.hp == special.hp and physical.nature == special.nature;
}


namespace detail {

constexpr auto possible_defensive_ev_combinations() {
	constexpr auto possible_hp_evs = 252_bi / 4_bi + 1_bi;
	constexpr auto possible_natures = std::numeric_limits<Nature>::max() + 1_bi;
	// Given a stat, a nature, and an HP EV, the defensive EV can be calculated
	return static_cast<std::intmax_t>(possible_hp_evs * possible_natures);
}

}	// namespace detail

using EqualDefensiveness = containers::static_vector<SingleClassificationEVs, detail::possible_defensive_ev_combinations()>;

EqualDefensiveness equal_defensiveness(Pokemon const & pokemon, bool physical);

}	// namespace technicalmachine
