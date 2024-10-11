// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.string_conversions.pokemon;

import tm.move.move;
import tm.move.regular_moves;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.stat_names;
import tm.stat.stat_style;

import tm.status.status;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;
import tm.string_conversions.stat_names;
import tm.string_conversions.status_name;

import tm.ability;
import tm.generation;
import tm.item;

import bounded;
import containers;
import numeric_traits;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

using namespace std::string_view_literals;

// TODO: Print nickname
// TODO: Print level
// TODO: Print gender
// TODO: Make this compatible with Pokemon Showdown

template<typename T>
constexpr auto is_maxed(T const x) -> bool {
	return x.value() == numeric_traits::max_value<typename T::value_type>;
}

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

constexpr auto ability_to_string(Generation const generation, Ability const ability) -> containers::string {
	return generation >= Generation::three ?
		containers::concatenate<containers::string>(
			"\tAbility: "sv,
			to_string(ability),
			"\n"sv
		) :
		containers::string();
}

constexpr auto status_to_string(Status const status) -> containers::string {
	auto const output_status = !is_clear(status);
	return output_status ?
		containers::concatenate<containers::string>(
			"\tStatus: "sv,
			to_string(status.name()),
			"\n"sv
		) :
		containers::string("");
};

constexpr auto stat_to_string(std::string_view const type, auto const range, auto const get) {
	bool is_first = true;
	auto element_string = [&](auto const stat) {
		auto const element = get(stat);
		if (!element) {
			return containers::string();
		}
		auto const was_first = std::exchange(is_first, false);
		return containers::concatenate<containers::string>(
			was_first ? ""sv : " / "sv,
			containers::to_string(element->value()),
			" "sv,
			to_string(stat)
		);
	};
	auto str = containers::string(containers::join(containers::transform_non_idempotent(range, element_string)));
	return is_first ?
		containers::string() :
		containers::concatenate<containers::string>(
			"\t"sv,
			type,
			": "sv,
			std::move(str),
			"\n"sv
		);
};

template<SpecialStyle special_style>
constexpr auto stats_to_string(CombinedStats<special_style> const stats) -> containers::string {
	auto get_iv = [&](auto const stat) {
		auto const value = stats.dvs_or_ivs[stat];
		return BOUNDED_CONDITIONAL(!is_maxed(value), value, tv::none);
	};
	if constexpr (special_style == SpecialStyle::combined) {
		auto get_ev = [&](SpecialPermanentStat const stat) {
			auto const value = stats.evs[stat];
			return BOUNDED_CONDITIONAL(value < EV::useful_max, value, tv::none);
		};
		return containers::concatenate<containers::string>(
			stat_to_string(
				"DVs"sv,
				containers::enum_range<SpecialRegularStat>(),
				get_iv
			),
			stat_to_string(
				"EVs"sv,
				containers::enum_range<SpecialPermanentStat>(),
				get_ev
			)
		);
	} else {
		static_assert(special_style == SpecialStyle::split);
		auto get_ev = [&](SplitSpecialPermanentStat const stat) {
			auto const value = stats.evs[stat];
			return BOUNDED_CONDITIONAL(value != 0_bi, value, tv::none);
		};
		return containers::concatenate<containers::string>(
			"\tNature: "sv,
			to_string(stats.nature),
			"\n"sv,
			stat_to_string(
				"IVs"sv,
				containers::enum_range<SplitSpecialRegularStat>(),
				get_iv
			),
			stat_to_string(
				"EVs"sv,
				containers::enum_range<SplitSpecialPermanentStat>(),
				get_ev
			)
		);
	}
}

constexpr auto moves_to_string(RegularMoves const moves) -> containers::string {
	auto is_first = true;
	return containers::string(containers::join(containers::transform_non_idempotent(
		moves,
		[&](Move const move) {
			auto const was_first = std::exchange(is_first, false);
			return containers::concatenate<containers::string>(
				was_first ? ""sv : "\n"sv,
				"\t- "sv,
				to_string(move.name())
			);
		}
	)));
};


export template<any_pokemon PokemonType>
constexpr auto to_string(PokemonType const & pokemon) -> containers::string {
	constexpr auto generation = generation_from<PokemonType>;
	// Boost.Format fails to compile with C++20, so we have to do this instead
	auto const hp_str = percent_to_string(100.0 * static_cast<double>(hp_ratio(pokemon)));

	auto item_to_string = [&] -> containers::string {
		return generation >= Generation::two ?
			containers::concatenate<containers::string>(
				" @ "sv,
				to_string(pokemon.item(false, false)),
				"\n"sv
			) :
			containers::string("\n");
	};


	return containers::concatenate<containers::string>(
		to_string(pokemon.species()),
		" ("sv,
		hp_str,
		"% HP)"sv,
		item_to_string(),
		ability_to_string(generation, pokemon.initial_ability()),
		status_to_string(pokemon.status()),
		stats_to_string(calculate_ivs_and_evs(pokemon)),
		moves_to_string(pokemon.regular_moves())
	);
}

} // namespace technicalmachine
