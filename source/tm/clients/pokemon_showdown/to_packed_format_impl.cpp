// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string_view>

module tm.clients.ps.to_packed_format;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.stat_names;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.any_team;
import tm.generation;
import tm.generation_generic;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;
import tv;

// https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md#packed-format

namespace technicalmachine::ps {
using namespace std::string_view_literals;

constexpr auto comma_separator = containers::array({','});

constexpr auto get_ev(EVs const evs, SplitSpecialRegularStat const stat) -> EV {
	return evs[stat];
}
constexpr auto get_ev(OldGenEVs const evs, SplitSpecialRegularStat const stat) -> EV {
	return evs[to_combined(stat)];
}

// Spc goes in SpA. SpD is ignored.
constexpr auto evs_to_string(auto const evs) -> containers::string {
	auto result = containers::to_string(evs.hp.value());
	for (auto const stat_name : containers::enum_range<SplitSpecialRegularStat>()) {
		result = containers::concatenate<containers::string>(
			std::move(result),
			comma_separator,
			containers::to_string(get_ev(evs, stat_name).value())
		);
	}
	return result;
}

constexpr auto get_iv(IVs const ivs, SplitSpecialRegularStat const stat) -> IV {
	return ivs[stat];
}
constexpr auto get_iv(DVs const dvs, SplitSpecialRegularStat const stat) -> IV {
	return IV(dvs[to_combined(stat)]);
}

// Spc goes in SpA. SpD is ignored.
constexpr auto dvs_or_ivs_to_string(auto const dvs_or_ivs) -> containers::string {
	auto result = containers::to_string(IV(get_hp(dvs_or_ivs)).value());
	for (auto const stat_name : containers::enum_range<SplitSpecialRegularStat>()) {
		result = containers::concatenate<containers::string>(
			std::move(result),
			comma_separator,
			containers::to_string(get_iv(dvs_or_ivs, stat_name).value())
		);
	}
	return result;
}



constexpr auto impl = []<Generation generation>(KnownTeam<generation> const & team) -> containers::string {
	constexpr auto separator = containers::array{'|'};
	auto result = containers::string();
	for (auto const & pokemon : team.all_pokemon()) {
		auto const is_first = containers::is_empty(result);
		auto const item = pokemon.item(false, false);
		auto const species_str = to_string(pokemon.species());
		result = containers::concatenate<containers::string>(
			std::move(result),
			is_first ? ""sv : "]"sv,
			pokemon.nickname().str(),
			separator,
			species_str == pokemon.nickname().str() ? ""sv : species_str,
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

		auto const stats = calculate_ivs_and_evs(pokemon);

		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			to_string(pokemon.nature()),
			separator,
			evs_to_string(stats.evs),
			separator,
			to_string(pokemon.gender()),
			separator,
			dvs_or_ivs_to_string(stats.dvs_or_ivs),
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
};

auto to_packed_format(GenerationGeneric<KnownTeam> const & team) -> containers::string {
	return tv::visit(team, impl);
}

} // namespace technicalmachine::ps
