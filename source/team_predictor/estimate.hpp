// Class to help get the next most likely Pokemon
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

#pragma once

#include "../pokemon/species.hpp"

#include <bounded_integer/array.hpp>

#include <random>

namespace technicalmachine {
class Multiplier;
class Team;

class Estimate {
public:
	using Overall = bounded::array<unsigned, number_of_species>;
	using Lead = bounded::array<float, number_of_species>;
	Estimate(Overall const & overall, Lead const & lead, unsigned total);
	void update(Multiplier const & multiplier, Team const & team);
	void update(Multiplier const & multiplier, Species seen);
	Species most_likely() const;
	Species random(std::mt19937 & random_engine) const;
private:
	using Container = bounded::array<float, number_of_species>;
	Container estimate;
};

}	// namespace technicalmachine
