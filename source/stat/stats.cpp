// All 'normal' stats that a Pokemon has
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

#include "stats.hpp"

namespace technicalmachine {

Stats::Stats(Species const species):
	stats({{
		Stat(species, Stat::HP),
		Stat(species, Stat::ATK),
		Stat(species, Stat::DEF),
		Stat(species, Stat::SPA),
		Stat(species, Stat::SPD),
		Stat(species, Stat::SPE)
	}}) {
}

// The indexing requires a +1 because I have the first stat in the array, HP,
// set to -1 in the enum. This allowed me to index stat boosting without having
// to offset the index.
Stat const & Stats::operator[](Stat::Stats const stat) const {
	return stats[static_cast<size_t>(stat + 1)];
}
Stat & Stats::operator[](Stat::Stats const stat) {
	return stats[static_cast<size_t>(stat + 1)];
}


}	// namespace technicalmachine
