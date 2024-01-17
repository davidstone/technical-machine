// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.initial_team;

import tm.move.initial_move;
import tm.move.pp;

import tm.pokemon.happiness;
import tm.pokemon.initial_pokemon;
import tm.pokemon.level;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.stat_names;
import tm.stat.stat_style;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.gender;
import tm.initial_team;
import tm.item;

import bounded;
import containers;
import numeric_traits;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto to_string(SplitSpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP"sv;
		case SplitSpecialPermanentStat::atk: return "Atk"sv;
		case SplitSpecialPermanentStat::def: return "Def"sv;
		case SplitSpecialPermanentStat::spa: return "SpA"sv;
		case SplitSpecialPermanentStat::spd: return "SpD"sv;
		case SplitSpecialPermanentStat::spe: return "Spe"sv;
	}
}

constexpr auto to_string(SpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SpecialPermanentStat::hp: return "HP"sv;
		case SpecialPermanentStat::atk: return "Atk"sv;
		case SpecialPermanentStat::def: return "Def"sv;
		case SpecialPermanentStat::spe: return "Spe"sv;
		case SpecialPermanentStat::spc: return "Spc"sv;
	}
}

template<typename Index>
constexpr auto stats_to_string(
	std::string_view const name,
	auto const stats,
	bounded::bounded_integer auto const default_value
) -> containers::string {
	auto const indexes = containers::enum_range<Index>();
	auto const all_default = containers::all_equal(
		containers::transform(indexes, [&](Index const index) { return stats[index].value(); }),
		default_value
	);
	if (all_default) {
		return ""sv;
	}
	auto separator = ""sv;
	return containers::string(containers::concatenate_view(
		name,
		containers::join(containers::transform_non_idempotent(
			indexes,
			[&](Index const index) -> containers::string {
				auto const ev = stats[index].value();
				if (ev == default_value) {
					return ""sv;
				}
				auto result = containers::concatenate<containers::string>(
					separator,
					containers::to_string(ev),
					" "sv,
					to_string(index)
				);
				separator = " / "sv;
				return result;
			}
		)),
		"\n"sv
	));
}

constexpr auto ivs_to_string(DVs const dvs) -> containers::string {
	return stats_to_string<SpecialPermanentStat>(
		"DVs: "sv,
		dvs,
		numeric_traits::max_value<DV::value_type>
	);
}

constexpr auto ivs_to_string(IVs const ivs) -> containers::string {
	return stats_to_string<SplitSpecialPermanentStat>(
		"IVs: "sv,
		ivs,
		numeric_traits::max_value<IV::value_type>
	);
}

constexpr auto evs_to_string(OldGenEVs const evs) -> containers::string {
	return stats_to_string<SpecialPermanentStat>("EVs: "sv, evs, EV::useful_max);
}

constexpr auto evs_to_string(EVs const evs) -> containers::string {
	return stats_to_string<SplitSpecialPermanentStat>("EVs: "sv, evs, 0_bi);
}

constexpr auto pp_ups_to_string(PP::pp_ups_type const pp_ups) -> containers::string {
	return pp_ups != numeric_traits::max_value<PP::pp_ups_type> ?
		containers::concatenate<containers::string>(
			" ("sv,
			containers::to_string(pp_ups),
			" PP ups)"sv
		) :
		containers::string(""sv);
}

constexpr auto moves_to_string(InitialMoves const moves) -> containers::string {
	auto result = containers::string();
	auto separator = ""sv;
	for (auto const move : moves) {
		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			"- "sv,
			to_string(move.name),
			pp_ups_to_string(move.pp_ups)
		);
		separator = "\n"sv;
	}
	return result;
};

export template<SpecialStyle style>
constexpr auto to_string(InitialPokemon<style> const & pokemon) -> containers::string {
	auto const species_str = to_string(pokemon.species);
	return containers::concatenate<containers::string>(
		pokemon.nickname.str(),
		species_str != pokemon.nickname ?
			containers::concatenate<containers::string>(" ("sv, species_str, ")"sv) :
			containers::string(""sv),
		pokemon.gender != Gender::genderless ?
			containers::concatenate<containers::string>(
				" ("sv,
				to_string(pokemon.gender),
				")"sv
			) :
			containers::string(""sv),
		pokemon.item != Item::None ?
			containers::concatenate<containers::string>(" @ "sv, to_string(pokemon.item)) :
			containers::string(""sv),
		"\n"sv,
		// Technically not the right check, but in practice it's always accurate
		style == SpecialStyle::split ?
			containers::concatenate<containers::string>(
				"Ability: "sv,
				to_string(pokemon.ability),
				"\n"sv
			) :
			containers::string(""sv),
		pokemon.level != Level(100_bi) ?
			containers::concatenate<containers::string>(
				"Level: "sv,
				containers::to_string(pokemon.level()),
				"\n"sv
			) :
			containers::string(""sv),
		pokemon.happiness != Happiness() ?
			containers::concatenate<containers::string>(
				"Happiness: "sv,
				containers::to_string(pokemon.happiness()),
				"\n"sv
			) :
			containers::string(""sv),
		evs_to_string(pokemon.stats.evs),
		// Technically not the right check, but in practice it's always accurate
		style == SpecialStyle::split ?
			containers::concatenate<containers::string>(
				to_string(pokemon.stats.nature),
				" Nature\n"sv
			) :
			containers::string(""sv),
		ivs_to_string(pokemon.stats.dvs_or_ivs),
		moves_to_string(pokemon.moves)
	);
}

export template<SpecialStyle style>
constexpr auto to_string(InitialTeam<style> const & team) -> containers::string {
	auto result = containers::string();
	auto separator = ""sv;
	for (auto const & pokemon : team) {
		result = containers::concatenate<containers::string>(
			std::move(result),
			separator,
			to_string(pokemon)
		);
		separator = "\n\n"sv;
	}
	return result;
}

export constexpr auto to_string(AnyInitialTeam const & team) -> containers::string {
	return tv::visit(team, [](auto const & t) { return to_string(t); });
}

} // namespace technicalmachine
