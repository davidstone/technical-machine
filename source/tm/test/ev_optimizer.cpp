// Test EV optimizer
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/ev_optimizer.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <tm/string_conversions/nature.hpp>

#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>

#include <iostream>
#include <random>

namespace technicalmachine {
namespace {

auto regular_moves(Generation const generation, auto... move_names) {
	return RegularMoves({Move(generation, move_names)...});
}

void optimize_already_optimized(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Metagross;
	constexpr auto level = Level(100_bi);
	auto const base_stats = BaseStats(generation, species);
	auto pokemon = Pokemon<generation>(
		species,
		level,
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Adamant,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(96_bi),
				EV(96_bi),
				EV(0_bi),
				EV(4_bi),
				EV(60_bi)
			)
		},
		regular_moves(generation, Moves::Meteor_Mash)
	);

	auto const ivs_and_evs = calculate_ivs_and_evs(pokemon);
	auto const stats = Stats<generation>(base_stats, level, ivs_and_evs);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = false;
	BOUNDED_ASSERT(compute_minimal_spread(base_stats, stats, level, bounded::none, include_attack, include_special_attack) == ivs_and_evs);
	BOUNDED_ASSERT(pad_random_evs(ivs_and_evs, include_attack, include_special_attack, random_engine) == ivs_and_evs);
	optimize_evs(pokemon, random_engine);
	BOUNDED_ASSERT(calculate_ivs_and_evs(pokemon) == ivs_and_evs);
}

void defensive_tests() {
	std::cout << "\tRunning defensive tests.\n";
	constexpr auto generation = Generation::four;
	auto const base_stats = BaseStats(generation, Species::Celebi);
	constexpr auto level = Level(100_bi);
	constexpr auto nature = Nature::Bold;
	constexpr auto iv = IV(31_bi);
	constexpr auto hp_ev = EV(252_bi);
	constexpr auto defense_ev = EV(252_bi);
	constexpr auto special_defense_ev = EV(4_bi);
	auto const hp = HP(base_stats, level, iv, hp_ev).max();
	auto const defense = initial_stat<generation>(RegularStat::def, base_stats.def(), level, nature, iv, defense_ev);
	auto const special_defense = initial_stat<generation>(RegularStat::spd, base_stats.spd(), level, nature, iv, special_defense_ev);
	
	auto defensive_evs = DefensiveEVs(base_stats, level, DefensiveEVs::InputHP{iv, hp}, DefensiveEVs::InputStat<generation>{iv, defense}, DefensiveEVs::InputStat<generation>{iv, special_defense});
	for (auto const & candidate : defensive_evs) {
		BOUNDED_ASSERT(candidate.hp.ev == hp_ev);
		BOUNDED_ASSERT(candidate.defense.ev == defense_ev);
		BOUNDED_ASSERT(candidate.special_defense.ev >= special_defense_ev);
		BOUNDED_ASSERT(boosts_stat(candidate.nature, RegularStat::def));
	}
}

auto find(SpeedEVs const & container, Nature const nature) {
	auto const it = containers::find_if(container, [=](auto const & value) { return value.nature == nature; });
	BOUNDED_ASSERT(it != containers::end(container));
	return it->stat;
}

void speed_tests() {
	std::cout << "\tRunning speed tests.\n";
	
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Snorlax;
	constexpr auto level = Level(100_bi);
	constexpr auto original_nature = Nature::Hardy;
	constexpr auto iv = IV(31_bi);
	auto const base_stats = BaseStats(generation, species);
	auto const original_value = initial_stat<generation>(RegularStat::spe, base_stats.spe(), level, original_nature, iv, EV(76_bi));
	auto const speed_evs = SpeedEVs(base_stats, level, iv, SpeedEVs::Input<generation>{original_value});
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const found = find(speed_evs, nature);
		auto const new_value = initial_stat<generation>(RegularStat::spe, base_stats.spe(), level, nature, found.iv, found.ev);
		if (boosts_stat(nature, RegularStat::spe) and !boosts_stat(original_nature, RegularStat::spe)) {
			BOUNDED_ASSERT(new_value == original_value or new_value == original_value + 1_bi);
		} else {
			BOUNDED_ASSERT(new_value == original_value);
		}
	}
}

void not_level_100(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Masquerain;
	constexpr auto level = Level(83_bi);
	auto pokemon = Pokemon<generation>(
		species,
		level,
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Modest,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(192_bi),
				EV(0_bi),
				EV(8_bi),
				EV(120_bi),
				EV(60_bi),
				EV(128_bi)
			)
		},
		regular_moves(generation, Moves::Hydro_Pump, Moves::Bug_Buzz, Moves::Roost, Moves::Air_Slash)
	);

	optimize_evs(pokemon, random_engine);
}

void generation_two(std::mt19937 & random_engine) {
	constexpr auto generation = Generation::two;
	constexpr auto species = Species::Mew;
	constexpr auto level = Level(100_bi);
	auto const base_stats = BaseStats(generation, species);
	auto pokemon = Pokemon<generation>(
		species,
		level,
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		regular_moves(generation, Moves::Tackle, Moves::Psychic)
	);

	auto const ivs_and_evs = calculate_ivs_and_evs(pokemon);
	auto const stats = Stats<generation>(base_stats, level, ivs_and_evs);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = true;
	BOUNDED_ASSERT(compute_minimal_spread(base_stats, stats, level, bounded::none, include_attack, include_special_attack) == ivs_and_evs);
	BOUNDED_ASSERT(pad_random_evs(ivs_and_evs, include_attack, include_special_attack, random_engine) == ivs_and_evs);
	optimize_evs(pokemon, random_engine);
	BOUNDED_ASSERT(calculate_ivs_and_evs(pokemon) == ivs_and_evs);
}

}	// namespace

void ev_optimizer_tests() {
	std::cout << "Running EV optimizer tests.\n";
	
	defensive_tests();
	speed_tests();

	std::random_device rd;
	std::mt19937 random_engine(rd());
	optimize_already_optimized(random_engine);

	not_level_100(random_engine);

	generation_two(random_engine);

	std::cout << "EV optimizer tests passed.\n\n";
}

}	// namespace technicalmachine
