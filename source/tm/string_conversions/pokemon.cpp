// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.string_conversions.pokemon;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.stat_names;

import tm.status.status;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;
import tm.string_conversions.status_name;

import tm.generation;
import tm.item;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

using namespace std::string_view_literals;

// TODO: Print nickname
// TODO: Print level
// TODO: Print gender
// TODO: Make this compatible with Pokemon Showdown

constexpr auto percent_to_string(double const value) -> containers::string {
	BOUNDED_ASSERT(0.0 <= value and value <= 100.0);
	auto const integer_part = bounded::check_in_range(
		static_cast<std::uint64_t>(value),
		0_bi,
		100_bi
	);
	auto const decimal_part = bounded::check_in_range(
		static_cast<std::uint64_t>((value - static_cast<double>(integer_part)) * 10.0),
		0_bi,
		9_bi
	);
	return containers::concatenate<containers::string>(containers::to_string(integer_part), "."sv, containers::to_string(decimal_part));
}

export template<any_pokemon PokemonType>
constexpr auto to_string(PokemonType const & pokemon) -> containers::string {
	constexpr auto generation = generation_from<PokemonType>;
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto const hp_str = percent_to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));

	auto status_to_string = [&] {
		auto const output_status = !is_clear(pokemon.status());
		return output_status ?
			containers::concatenate<containers::string>(
				"\n\tStatus: "sv,
				to_string(pokemon.status().name())
			) :
			containers::string("");
	};

	auto stats = calculate_ivs_and_evs(pokemon);

	auto stat_to_iv_string = [&](auto const stat_name) {
		return containers::to_string(stats.dvs_or_ivs[stat_name].value());
	};

	auto stat_to_ev_string = [&](auto const stat_name) {
		return containers::to_string(stats.evs[stat_name].value());
	};

	auto stats_to_string = [&] {
		if constexpr (generation <= Generation::two) {
			return containers::concatenate<containers::string>(
				stat_to_iv_string(SpecialRegularStat::atk),
				" Atk / "sv,
				stat_to_iv_string(SpecialRegularStat::def),
				" Def / "sv,
				stat_to_iv_string(SpecialRegularStat::spe),
				" Spe / "sv,
				stat_to_iv_string(SpecialRegularStat::spc),
				" Spc\n\tEVs: "sv,
				stat_to_ev_string(SpecialPermanentStat::hp),
				" HP / "sv,
				stat_to_ev_string(SpecialPermanentStat::atk),
				" Atk / "sv,
				stat_to_ev_string(SpecialPermanentStat::def),
				" Def / "sv,
				stat_to_ev_string(SpecialPermanentStat::spe),
				" Spe / "sv,
				stat_to_ev_string(SpecialPermanentStat::spc),
				" Spc"sv
			);
		} else {
			return containers::concatenate<containers::string>(
				to_string(pokemon.nature()),
				"\n\tIVs: "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::hp),
				" HP / "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::atk),
				" Atk / "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::def),
				" Def / "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::spa),
				" SpA / "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::spd),
				" SpD / "sv,
				stat_to_iv_string(SplitSpecialPermanentStat::spe),
				" Spe\n\tEVs: "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::hp),
				" HP / "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::atk),
				" Atk / "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::def),
				" Def / "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::spa),
				" SpA / "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::spd),
				" SpD / "sv,
				stat_to_ev_string(SplitSpecialPermanentStat::spe),
				" Spe"sv
			);
		}
	};

	auto moves_to_string = [&] {
		containers::string output;
		for (auto const & move : pokemon.regular_moves()) {
			output = containers::concatenate<containers::string>(
				std::move(output),
				"\n\t- "sv,
				to_string(move.name())
			);
		}
		return output;
	};

	return containers::concatenate<containers::string>(
		to_string(pokemon.species()),
		" ("sv,
		hp_str,
		"% HP) @ "sv,
		to_string(pokemon.item(false, false)),
		"\n\tAbility: "sv,
		to_string(pokemon.initial_ability()),
		status_to_string(),
		generation <= Generation::two ? "\n\tDVs: "sv : "\n\tNature: "sv,
		stats_to_string(),
		moves_to_string()
	);
}

} // namespace technicalmachine
