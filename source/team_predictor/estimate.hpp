// Class to help get the next most likely Pokemon
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

#ifndef TEAM_PREDICTOR__ESTIMATE_HPP_
#define TEAM_PREDICTOR__ESTIMATE_HPP_

#include <array>
#include <random>
#include "../pokemon/species.hpp"

namespace technicalmachine {
class Multiplier;
class Team;

class Estimate {
public:
	typedef std::array<unsigned, number_of_species> Overall;
	typedef std::array<float, number_of_species> Lead;
	Estimate(Overall const & overall, Lead const & lead, unsigned total);
	void update(Multiplier const & multiplier, Team const & team);
	void update(Multiplier const & multiplier, Species seen);
	Species most_likely() const;
	Species random(std::mt19937 & random_engine) const;
private:
	typedef std::array<float, number_of_species> Container;
	Container estimate;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__MULTIPLIER_HPP_
