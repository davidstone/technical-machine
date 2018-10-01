// Teammate stat multipliers
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

#include "load_stats.hpp"

#include "../pokemon/species.hpp"

#include <containers/array/array.hpp>

#include <filesystem>

namespace technicalmachine {

struct Multiplier {
	using value_type = float;
	Multiplier(OverallStats const & overall, std::filesystem::path const & teammate_stats);
	value_type operator()(Species species1, Species species2) const;
private:
	using Container = containers::array<value_type, number_of_species, number_of_species>;
	static Container species_clause();
	void load_listed_multipliers(OverallStats const & overall, std::filesystem::path const & teammate_stats, OverallStats & unaccounted);
	void estimate_remaining(OverallStats const & overall, OverallStats const & unaccounted);
	Container multiplier;
};

}	// namespace technicalmachine
