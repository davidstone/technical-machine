// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
