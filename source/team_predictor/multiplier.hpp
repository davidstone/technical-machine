// Teammate stat multipliers
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

#ifndef TEAM_PREDICTOR__MULTIPLIER_HPP_
#define TEAM_PREDICTOR__MULTIPLIER_HPP_

#include "../pokemon/species.hpp"

#include <bounded_integer/array.hpp>

namespace technicalmachine {

class Multiplier {
public:
	using Overall = bounded::array<unsigned, number_of_species>;
	using value_type = float;
	Multiplier(Overall const & overall);
	value_type operator() (Species species1, Species species2) const;
private:
	using Container = bounded::array<bounded::array<value_type, number_of_species>, number_of_species>;
	static Container species_clause();
	void load_listed_multipliers(Overall const & overall, Overall & unaccounted);
	void estimate_remaining(Overall const & overall, Overall const & unaccounted);
	Container multiplier;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__MULTIPLIER_HPP_
