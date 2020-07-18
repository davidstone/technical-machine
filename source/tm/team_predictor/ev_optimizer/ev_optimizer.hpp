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

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <tm/type/type.hpp>

#include <bounded/optional.hpp>

#include <random>

namespace technicalmachine {

template<Generation generation>
auto set_stats(Pokemon<generation> & pokemon, CombinedStats<IVAndEV> const stats) {
	auto const original_hp = pokemon.hp();
	pokemon.set_nature(stats.nature);
	for (auto const stat_name : containers::enum_range<PermanentStat>()) {
		auto const stat = stats[stat_name];
		pokemon.set_ev(stat_name, stat.iv, stat.ev);
	}
	auto const new_hp = pokemon.hp();
	pokemon.set_hp(new_hp.max() * original_hp.current() / original_hp.max());
}

auto optimize_evs(
	Generation const generation,
	CombinedStats<IVAndEV> combined,
	Species const species,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack,
	std::mt19937 & random_engine
) -> CombinedStats<IVAndEV>;

template<Generation generation>
void optimize_evs(Pokemon<generation> & pokemon, std::mt19937 & random_engine) {
	auto const species = pokemon.species();
	auto const level = pokemon.level();
	auto const include_attack = has_physical_move(pokemon);
	auto const include_special_attack = has_special_move(pokemon);
	auto const optimized = optimize_evs(
		generation,
		calculate_ivs_and_evs(pokemon),
		species,
		level,
		get_hidden_power_type(pokemon),
		include_attack,
		include_special_attack,
		random_engine
	);
	set_stats(pokemon, optimized);
}

auto compute_minimal_spread(Generation, BaseStats, GenericStats<HP::max_type, InitialStat> stats, Level, bounded::optional<Type> hidden_power_type, bool include_attack, bool include_special_attack) -> CombinedStats<IVAndEV>;
auto pad_random_evs(Generation, CombinedStats<IVAndEV> combined, bool include_attack, bool include_special_attack, std::mt19937 & random_engine) -> CombinedStats<IVAndEV>;

}	// namespace technicalmachine
