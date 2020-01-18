// Copyright (C) 2019 David Stone
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

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/operators.hpp>

#include <containers/array/array.hpp>

#include <random>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Pokemon;

constexpr auto regular_stats() {
	return containers::array{StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE};
}

auto pull_out_stats(Pokemon const & pokemon) -> CombinedStats;

void optimize_evs(Generation, Pokemon & pokemon, std::mt19937 & random_engine);
auto minimize_evs(Generation, CombinedStats stats, Species, Level, bool include_attack, bool include_special_attack) -> CombinedStats;
auto pad_random_evs(Generation, CombinedStats combined, bool include_attack, bool include_special_attack, std::mt19937 & random_engine) -> CombinedStats;

}	// namespace technicalmachine
