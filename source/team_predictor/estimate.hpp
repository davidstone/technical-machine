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

#include "lead_stats.hpp"

#include "../pokemon/species.hpp"

#include <containers/array/array.hpp>

#include <random>

namespace technicalmachine {

struct Multiplier;
struct Team;
struct UsageStats;

struct Estimate {
	Estimate(UsageStats const & usage_stats, LeadStats lead_stats);
	void update(Multiplier const & multiplier, Team const & team);
	void update(Multiplier const & multiplier, Species seen);
	Species most_likely() const;
	Species random(std::mt19937 & random_engine) const;
private:
	using value_type = float;
	containers::array<value_type, number_of_species> estimate;
};

}	// namespace technicalmachine
