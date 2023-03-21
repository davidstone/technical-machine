// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string>

export module tm.clients.ps.to_packed_format;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.iv;
import tm.stat.stat_names;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.any_team;
import tm.item;

import bounded;
import containers;
import std_module;

// https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md#packed-format

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export auto to_packed_format(any_known_team auto const & team) -> containers::string {
	constexpr auto separator = containers::array{'|'};
	auto result = containers::string();
	for (auto const & pokemon : team.all_pokemon()) {
		auto const is_first = containers::is_empty(result);
		auto const item = pokemon.item(false, false);
		auto const species_str = to_string(pokemon.species());
		result = containers::concatenate<containers::string>(
			std::move(result),
			is_first ? ""sv : "]"sv,
			pokemon.nickname(),
			separator,
			species_str == pokemon.nickname() ? ""sv : species_str,
			separator,
			item == Item::None ? ""sv : to_string(item),
			separator,
			to_string(pokemon.initial_ability()),
			separator
		);

		auto const moves = pokemon.regular_moves();
		for (auto it = containers::begin(moves); it != containers::end(moves); ++it) {
			if (it != containers::begin(moves)) {
				containers::push_back(result, ',');
			}
			containers::append(result, to_string(it->name()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			to_string(pokemon.nature()),
			separator
		);

		auto const stats = calculate_ivs_and_evs(pokemon);
		// Spc goes in SpA. SpD is ignored.
		for (auto const stat_name : containers::enum_range<SplitSpecialPermanentStat>()) {
			if (stat_name != SplitSpecialPermanentStat::hp) {
				containers::push_back(result, ',');
			}
			containers::append(result, containers::to_string(stats.evs[stat_name].value()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			to_string(pokemon.gender()),
			separator
		);

		// Spc goes in SpA. SpD is ignored.
		for (auto const stat_name : containers::enum_range<SplitSpecialPermanentStat>()) {
			if (stat_name != SplitSpecialPermanentStat::hp) {
				containers::push_back(result, ',');
			}
			containers::append(result, containers::to_string(IV(stats.dvs_or_ivs[stat_name]).value()));
		}

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			// Assume non-shiny
			separator,
			containers::to_string(pokemon.level()()),
			separator
			// Assume max happiness
			// Assume regular Poke Ball
			// Generations with Hyper Training are not yet supported
		);
	}
	return result;
}

} // namespace technicalmachine::ps
