// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>

#include <tm/string_conversions/nature.hpp>

#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/team.hpp>

#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>

#include <random>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto regular_moves(Generation const generation, auto... move_names) {
	return RegularMoves({Move(generation, move_names)...});
}

TEST_CASE("Optimize already optimized EVs", "[EV Optimizer]") {
	auto rd = std::random_device();
	auto random_engine = std::mt19937(rd());
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
	CHECK(compute_minimal_spread(base_stats, stats, level, bounded::optional<HiddenPower<generation>>(), include_attack, include_special_attack) == ivs_and_evs);
	CHECK(pad_random_evs(ivs_and_evs, include_attack, include_special_attack, random_engine) == ivs_and_evs);
	optimize_evs(pokemon, random_engine);
	CHECK(calculate_ivs_and_evs(pokemon) == ivs_and_evs);
}

TEST_CASE("Optimize defensive EVs", "[EV Optimizer]") {
	constexpr auto generation = Generation::four;
	constexpr auto base_hp = 100_bi;
	constexpr auto base_def = 100_bi;
	constexpr auto base_spd = 100_bi;
	constexpr auto level = Level(100_bi);
	constexpr auto nature = Nature::Bold;
	constexpr auto possible_ivs = possible_dvs_or_ivs(bounded::optional<HiddenPower<generation>>());
	constexpr auto hp_ev = EV(252_bi);
	constexpr auto defense_ev = EV(252_bi);
	constexpr auto special_defense_ev = EV(4_bi);
	constexpr auto hp = HP(base_hp, level, IV(31_bi), hp_ev).max();
	constexpr auto defense = initial_stat<generation>(SplitSpecialRegularStat::def, base_def, level, nature, IV(31_bi), defense_ev);
	constexpr auto special_defense = initial_stat<generation>(SplitSpecialRegularStat::spd, base_spd, level, nature, IV(31_bi), special_defense_ev);
	constexpr auto input_hp = DefensiveEVHP{base_hp, possible_optimized_ivs(possible_ivs.hp()), hp};
	constexpr auto input_def = DefensiveEVDef<generation>{base_def, possible_optimized_ivs(possible_ivs.def()), defense};
	constexpr auto input_spd = DefensiveEVSpD<generation>{base_spd, possible_optimized_ivs(possible_ivs.spd()), special_defense};
	// Too many steps to constant evaluate
	auto const defensive_evs = DefensiveEVs(level, input_hp, input_def, input_spd);
	for (auto const & candidate : defensive_evs) {
		CHECK(candidate.hp.ev == hp_ev);
		CHECK(candidate.defense.ev == defense_ev);
		CHECK(candidate.special_defense.ev >= special_defense_ev);
		CHECK(boosts_stat(candidate.nature, SplitSpecialRegularStat::def));
	}
}

constexpr auto find(SpeedEVs const & container, Nature const nature) {
	auto const it = containers::find_if(container, [=](auto const & value) { return value.nature == nature; });
	BOUNDED_ASSERT(it != containers::end(container));
	return *it;
}

static_assert([] {
	constexpr auto generation = Generation::four;
	constexpr auto level = Level(100_bi);
	constexpr auto original_nature = Nature::Hardy;
	constexpr auto base_spe = 30_bi;
	constexpr auto original_value = initial_stat<generation>(SplitSpecialRegularStat::spe, base_spe, level, original_nature, IV(31_bi), EV(76_bi));
	constexpr auto possible_ivs = possible_optimized_ivs(possible_dvs_or_ivs(bounded::optional<HiddenPower<generation>>()).spe());
	constexpr auto speed_evs = SpeedEVs(base_spe, level, possible_ivs, SpeedEVs::Input<generation>{original_value});
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const found = find(speed_evs, nature);
		auto const new_value = initial_stat<generation>(SplitSpecialRegularStat::spe, base_spe, level, nature, found.iv, found.ev);
		if (boosts_stat(nature, SplitSpecialRegularStat::spe) and !boosts_stat(original_nature, SplitSpecialRegularStat::spe)) {
			BOUNDED_ASSERT(new_value == original_value or new_value == original_value + 1_bi);
		} else {
			BOUNDED_ASSERT(new_value == original_value);
		}
	}
	return true;
}());

TEST_CASE("Optimize EVs below level 100", "[EV Optimizer]") {
	auto rd = std::random_device();
	auto random_engine = std::mt19937(rd());
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

TEST_CASE("Optimize generation 2 EVs", "[EV Optimizer]") {
	auto rd = std::random_device();
	auto random_engine = std::mt19937(rd());
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
		default_combined_stats<generation>,
		regular_moves(generation, Moves::Tackle, Moves::Psychic)
	);

	auto const ivs_and_evs = calculate_ivs_and_evs(pokemon);
	auto const stats = Stats<generation>(base_stats, level, ivs_and_evs);
	
	constexpr auto include_attack = true;
	constexpr auto include_special_attack = true;
	CHECK(compute_minimal_spread(base_stats, stats, level, bounded::optional<HiddenPower<generation>>(), include_attack, include_special_attack) == ivs_and_evs);
	CHECK(pad_random_evs(ivs_and_evs, include_attack, include_special_attack, random_engine) == ivs_and_evs);
	optimize_evs(pokemon, random_engine);
	CHECK(calculate_ivs_and_evs(pokemon) == ivs_and_evs);
}

} // namespace
} // namespace technicalmachine
