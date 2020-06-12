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

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/stat.hpp>

#include <tm/type/type.hpp>

#include <bounded/optional.hpp>

#include <random>

namespace technicalmachine {
enum class Generation : std::uint8_t;
struct Pokemon;

void optimize_evs(Generation, Pokemon & pokemon, std::mt19937 & random_engine);
auto compute_minimal_spread(Generation, BaseStats, GenericStats<HP::max_type, initial_stat_type> stats, Level, bounded::optional<Type> hidden_power_type, bool include_attack, bool include_special_attack) -> CombinedStats<IVAndEV>;
auto pad_random_evs(Generation, CombinedStats<IVAndEV> combined, bool include_attack, bool include_special_attack, std::mt19937 & random_engine) -> CombinedStats<IVAndEV>;

}	// namespace technicalmachine
