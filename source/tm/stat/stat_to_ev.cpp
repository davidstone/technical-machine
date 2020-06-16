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

#include <containers/integer_range.hpp>

#include <stdexcept>

namespace technicalmachine {
namespace {

static_assert(round_up_divide(1_bi, 2_bi) == 1_bi);
static_assert(round_up_divide(1_bi, 1_bi) == 1_bi);
static_assert(round_up_divide(5_bi, 1_bi) == 5_bi);
static_assert(round_up_divide(6_bi, 5_bi) == 2_bi);

static_assert(stat_to_ev(614_bi, Nature::Impish, RegularStat::def, 230_bi, IV(31_bi), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(558_bi, Nature::Hardy, RegularStat::def, 230_bi, IV(DV(15_bi)), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(178_bi, Nature::Bold, RegularStat::atk, 125_bi, IV(19_bi), Level(63_bi)) == EV(152_bi));

auto calculate_ivs_and_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, StatValue> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool has_physical_move,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats<IVAndEV> {
	// TODO: Use Hidden Power power to determine IVs, not just the type
	auto const base = BaseStats(generation, species);
	
	while (true) {
		auto const ivs = hidden_power_ivs(generation, hidden_power_type, has_physical_move);
		auto const hp_ev = hp_to_ev(base, level, stats.hp, ivs.hp);

		for (auto const nature : nature_range) {
			auto compute_ev = [=](RegularStat const stat_name, auto const base_stat) {
				return stat_to_ev(stats[stat_name], nature, stat_name, base_stat, ivs[stat_name], level);
			};
			auto const attack_ev = compute_ev(RegularStat::atk, base.atk());
			if (!attack_ev) {
				continue;
			}
			auto const defense_ev = compute_ev(RegularStat::def, base.def());
			if (!defense_ev) {
				continue;
			}
			auto const special_attack_ev = compute_ev(RegularStat::spa, base.spa());
			if (!special_attack_ev) {
				continue;
			}
			auto const special_defense_ev = compute_ev(RegularStat::spd, base.spd());
			if (!special_defense_ev) {
				continue;
			}
			auto const speed_ev = compute_ev(RegularStat::spe, base.spe());
			if (!speed_ev) {
				continue;
			}

			auto const combined = CombinedStats<IVAndEV>{
				nature,
				{ivs.hp, hp_ev},
				{ivs.atk, *attack_ev},
				{ivs.def, *defense_ev},
				{ivs.spa, *special_attack_ev},
				{ivs.spd, *special_defense_ev},
				{ivs.spe, *speed_ev}
			};
			if (ev_sum(combined) > max_total_evs(generation)) {
				continue;
			}
			
			return combined;
		}
		if (has_physical_move) {
			break;
		}
		has_physical_move = true;
	}
	throw std::runtime_error(
		"No Nature + EV combination combines to give the received stats in generation " +
		std::string(to_string(generation)) +
		": Species: " + std::string(to_string(species)) +
		" Level: " + bounded::to_string(level()) +
		" HP: " + bounded::to_string(stats.hp) +
		" Attack: " + bounded::to_string(stats.atk) +
		" Defense: " + bounded::to_string(stats.def) +
		" Special Attack: " + bounded::to_string(stats.spa) +
		" Special Defense: " + bounded::to_string(stats.spd) +
		" Speed: " + bounded::to_string(stats.spe)
	);
}

} // namespace

auto calculate_ivs_and_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, StatValue> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool const has_physical_move
) -> CombinedStats<IVAndEV> {
	auto const nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_ivs_and_evs(
		generation,
		species,
		level,
		stats,
		hidden_power_type,
		has_physical_move,
		nature_range
	);
}

auto calculate_ivs_and_evs(Generation const generation, Pokemon const pokemon) -> CombinedStats<IVAndEV> {
	auto const nature = get_nature(pokemon);
	auto calculate_stat = [=](RegularStat const stat_name) {
		auto const stat = get_stat(pokemon, stat_name);
		return initial_stat(stat_name, stat.base(), stat.iv(), stat.ev(), get_level(pokemon), nature);
	};
	auto const stats = GenericStats<HP::max_type, StatValue>{
		get_hp(pokemon).max(),
		calculate_stat(RegularStat::atk),
		calculate_stat(RegularStat::def),
		calculate_stat(RegularStat::spa),
		calculate_stat(RegularStat::spd),
		calculate_stat(RegularStat::spe)
	};
	return calculate_ivs_and_evs(
		generation,
		pokemon.species(),
		get_level(pokemon),
		stats,
		get_hidden_power_type(pokemon),
		has_physical_move(generation, pokemon),
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
