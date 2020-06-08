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

#include <tm/stat/stat_to_ev.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/hidden_power_ivs.hpp>

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/algorithms/binary_search.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace {

static_assert(round_up_divide(1_bi, 2_bi) == 1_bi);
static_assert(round_up_divide(1_bi, 1_bi) == 1_bi);
static_assert(round_up_divide(5_bi, 1_bi) == 5_bi);
static_assert(round_up_divide(6_bi, 5_bi) == 2_bi);

static_assert(stat_to_ev(614_bi, Nature::Impish, StatNames::DEF, 230_bi, IV(31_bi), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(558_bi, Nature::Hardy, StatNames::DEF, 230_bi, IV(DV(15_bi)), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(178_bi, Nature::Bold, StatNames::ATK, 125_bi, IV(19_bi), Level(63_bi)) == EV(152_bi));

} // namespace

auto hp_to_ev(BaseStats const base, Level const level, HP::max_type const stat, IV const iv) -> EV {
	auto const stat_range = containers::transform(ev_range(), [=](EV const ev) { return HP(base, level, iv, ev).max(); });
	auto const it = containers::lower_bound(stat_range, stat);
	if (it == end(stat_range)) {
		throw std::runtime_error("No valid HP EV for a given stat value");
	}
	return *it.base();
}

namespace {

auto calculate_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, StatValue> const stats,
	IVs const ivs,
	auto const nature_range
) -> CombinedStats<EV> {
	// TODO: Give the correct IVs for the Hidden Power type
	
	auto const base = BaseStats(generation, species);
	
	auto const hp_ev = hp_to_ev(base, level, stats.hp, ivs.hp);

	for (auto const nature : nature_range) {
		auto const attack_ev = stat_to_ev(stats.attack, nature, StatNames::ATK, base.atk(), ivs.attack, level);
		if (!attack_ev) {
			continue;
		}
		auto const defense_ev = stat_to_ev(stats.defense, nature, StatNames::DEF, base.def(), ivs.defense, level);
		if (!defense_ev) {
			continue;
		}
		auto const special_attack_ev = stat_to_ev(stats.special_attack, nature, StatNames::SPA, base.spa(), ivs.special_attack, level);
		if (!special_attack_ev) {
			continue;
		}
		auto const special_defense_ev = stat_to_ev(stats.special_defense, nature, StatNames::SPD, base.spd(), ivs.special_defense, level);
		if (!special_defense_ev) {
			continue;
		}
		auto const speed_ev = stat_to_ev(stats.speed, nature, StatNames::SPE, base.spe(), ivs.speed, level);
		if (!speed_ev) {
			continue;
		}

		auto const combined = CombinedStats<EV>{
			nature,
			hp_ev,
			*attack_ev,
			*defense_ev,
			*special_attack_ev,
			*special_defense_ev,
			*speed_ev
		};
		if (ev_sum(combined) > max_total_evs(generation)) {
			continue;
		}
		
		return combined;
	}
	throw std::runtime_error(
		"No Nature + EV combination combines to give the received stats in generation " +
		std::string(to_string(generation)) +
		": Species: " + std::string(to_string(species)) +
		" Level: " + bounded::to_string(level()) +
		" HP: " + bounded::to_string(stats.hp) +
		" Attack: " + bounded::to_string(stats.attack) +
		" Defense: " + bounded::to_string(stats.defense) +
		" Special Attack: " + bounded::to_string(stats.special_attack) +
		" Special Defense: " + bounded::to_string(stats.special_defense) +
		" Speed: " + bounded::to_string(stats.speed)
	);
}

} // namespace

auto calculate_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, StatValue> const stats,
	IVs const ivs
) -> CombinedStats<EV> {
	auto const nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_evs(
		generation,
		species,
		level,
		stats,
		ivs,
		nature_range
	);
}

auto calculate_evs(Generation const generation, Pokemon const pokemon) -> CombinedStats<EV> {
	auto const nature = get_nature(pokemon);
	auto calculate_stat = [=](StatNames const stat_name) {
		auto const stat = get_stat(pokemon, stat_name);
		return initial_stat(stat_name, stat.base(), stat.iv(), stat.ev(), get_level(pokemon), nature);
	};
	auto const stats = GenericStats<HP::max_type, StatValue>{
		get_hp(pokemon).max(),
		calculate_stat(StatNames::ATK),
		calculate_stat(StatNames::DEF),
		calculate_stat(StatNames::SPA),
		calculate_stat(StatNames::SPD),
		calculate_stat(StatNames::SPE)
	};
	auto const ivs = hidden_power_ivs(generation, get_hidden_power_type(pokemon), has_physical_move(generation, pokemon));
	// TODO: Use Hidden Power power to determine IVs, not just the type
	return calculate_evs(
		generation,
		get_species(pokemon),
		get_level(pokemon),
		stats,
		ivs,
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
