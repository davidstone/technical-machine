// Teammate stat multipliers
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

#ifndef TEAM_PREDICTOR__MULTIPLIER_HPP_
#define TEAM_PREDICTOR__MULTIPLIER_HPP_

#include <array>
#include "../pokemon/species.hpp"

namespace technicalmachine {

class Multiplier {
	public:
		typedef std::array<unsigned, number_of_species> Overall;
		typedef float value_type;
		Multiplier(Overall const & overall);
		value_type operator() (Species species1, Species species2) const;
	private:
		typedef std::array<std::array<value_type, number_of_species>, number_of_species> Container;
		static constexpr unsigned pokemon_per_team = 6;
		static constexpr unsigned other_pokemon_per_team = pokemon_per_team - 1;
		static Container species_clause();
		void load_listed_multipliers(Overall const & overall, Overall & unaccounted);
		void estimate_remaining(Overall const & overall, Overall const & unaccounted);
		Container multiplier;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__MULTIPLIER_HPP_
