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

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/algorithms/binary_search.hpp>
#include <containers/algorithms/transform.hpp>

#include <stdexcept>

namespace technicalmachine {
enum class Generation : std::uint8_t;
using namespace bounded::literal;

constexpr auto round_up_divide(auto const lhs, auto const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

inline auto hp_to_ev(Generation const generation, Species const species, Level const level, HP::max_type const stat, IV const iv) {
	auto const stat_range = containers::transform(ev_range(), [=](EV const ev) { return HP(generation, species, level, iv, ev).max(); });
	auto const it = containers::lower_bound(stat_range, stat);
	if (it == end(stat_range)) {
		throw std::runtime_error("No valid HP EV for a given stat value");
	}
	return *it.base();
}

// `target` is not just bounded::integer<4, 614> because this function is also
// used in the EV optimizer, where values outside the legal range are regularly
// encountered as part of speculative computation.
constexpr auto stat_to_ev(auto const target, Nature const nature, StatNames const stat_name, Stat::base_type const base, IV const iv, Level const level) {
	return bounded::max(
		0_bi,
		(round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi
	);
}

using StatValue = bounded::integer<4, 614>;
auto calculate_evs(Generation const generation, Species const species, Level const level, GenericStats<HP::max_type, StatValue> const stats, IVs const ivs, auto const nature_range) -> CombinedStats {
	// TODO: Give the correct IVs for the Hidden Power type
	
	auto const base = BaseStats(generation, species);
	
	auto to_ev = [](auto const integer) { return EV(EV::value_type(integer)); };
	auto const hp_ev = hp_to_ev(generation, species, level, stats.hp, ivs.hp);

	for (auto const nature : nature_range) {
		auto const attack_ev = stat_to_ev(stats.attack, nature, StatNames::ATK, base.atk(), ivs.attack, level);
		if (attack_ev > EV::max) {
			continue;
		}
		auto const defense_ev = stat_to_ev(stats.defense, nature, StatNames::DEF, base.def(), ivs.defense, level);
		if (defense_ev > EV::max) {
			continue;
		}
		auto const special_attack_ev = stat_to_ev(stats.special_attack, nature, StatNames::SPA, base.spa(), ivs.special_attack, level);
		if (special_attack_ev > EV::max) {
			continue;
		}
		auto const special_defense_ev = stat_to_ev(stats.special_defense, nature, StatNames::SPD, base.spd(), ivs.special_defense, level);
		if (special_defense_ev > EV::max) {
			continue;
		}
		auto const speed_ev = stat_to_ev(stats.speed, nature, StatNames::SPE, base.spe(), ivs.speed, level);
		if (speed_ev > EV::max) {
			continue;
		}

		auto const combined = CombinedStats{nature, hp_ev, to_ev(attack_ev), to_ev(defense_ev), to_ev(special_attack_ev), to_ev(special_defense_ev), to_ev(speed_ev)};
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

inline auto calculate_evs(Generation const generation, Species const species, Level const level, GenericStats<HP::max_type, StatValue> const stats, IVs const ivs) -> CombinedStats {
	auto const nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_evs(generation, species, level, stats, ivs, nature_range);
}

inline auto calculate_evs(Generation const generation, Pokemon const pokemon) -> CombinedStats {
	auto const nature = get_nature(pokemon);
	auto calculate_stat = [=](StatNames const stat_name) {
		return initial_stat(stat_name, get_stat(pokemon, stat_name), get_level(pokemon), nature);
	};
	auto const stats = GenericStats<HP::max_type, StatValue>{
		get_hp(pokemon).max(),
		calculate_stat(StatNames::ATK),
		calculate_stat(StatNames::DEF),
		calculate_stat(StatNames::SPA),
		calculate_stat(StatNames::SPD),
		calculate_stat(StatNames::SPE)
	};
	auto const ivs = hidden_power_ivs(generation, get_hidden_power(pokemon).type(), has_physical_move(generation, pokemon));
	// TODO: Use Hidden Power power to determine IVs, not just the type
	return calculate_evs(generation, get_species(pokemon), get_level(pokemon), stats, ivs, containers::enum_range(nature, nature));
}

} // namespace technicalmachine
