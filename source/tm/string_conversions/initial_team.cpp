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
using namespace containers::string_literals;

constexpr auto to_string(SplitSpecialPermanentStat const stat) -> containers::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP"_sv;
		case SplitSpecialPermanentStat::atk: return "Atk"_sv;
		case SplitSpecialPermanentStat::def: return "Def"_sv;
		case SplitSpecialPermanentStat::spa: return "SpA"_sv;
		case SplitSpecialPermanentStat::spd: return "SpD"_sv;
		case SplitSpecialPermanentStat::spe: return "Spe"_sv;
	}
}

constexpr auto to_string(SpecialPermanentStat const stat) -> containers::string_view {
	switch (stat) {
		case SpecialPermanentStat::hp: return "HP"_sv;
		case SpecialPermanentStat::atk: return "Atk"_sv;
		case SpecialPermanentStat::def: return "Def"_sv;
		case SpecialPermanentStat::spe: return "Spe"_sv;
		case SpecialPermanentStat::spc: return "Spc"_sv;
	}
}

constexpr auto to_string(SpecialRegularStat const stat) -> containers::string_view {
	switch (stat) {
		case SpecialRegularStat::atk: return "Atk"_sv;
		case SpecialRegularStat::def: return "Def"_sv;
		case SpecialRegularStat::spe: return "Spe"_sv;
		case SpecialRegularStat::spc: return "Spc"_sv;
	}
}

template<typename Index>
constexpr auto stats_to_string(
	containers::string_view const name,
	auto const stats,
	bounded::bounded_integer auto const default_value
) -> containers::string {
	auto const indexes = containers::enum_range<Index>();
	auto const all_default = containers::all_equal(
		containers::transform(indexes, [&](Index const index) { return stats[index].value(); }),
		default_value
	);
	if (all_default) {
		return containers::string();
	}
	return containers::string(containers::concatenate_view(
		name,
		containers::join_with(
			containers::remove_none(containers::transform(
				indexes,
				[&](Index const index) -> tv::optional<containers::string> {
					auto const ev = stats[index].value();
					if (ev == default_value) {
						return tv::none;
					}
					return containers::concatenate<containers::string>(
						containers::to_string(ev),
						" "_sv,
						to_string(index)
					);
				}
			)),
			" / "_sv
		),
		"\n"_sv
	));
}

constexpr auto ivs_to_string(DVs const dvs) -> containers::string {
	return stats_to_string<SpecialRegularStat>(
		"DVs: "_sv,
		dvs,
		numeric_traits::max_value<DV::value_type>
	);
}

constexpr auto ivs_to_string(IVs const ivs) -> containers::string {
	return stats_to_string<SplitSpecialPermanentStat>(
		"IVs: "_sv,
		ivs,
		numeric_traits::max_value<IV::value_type>
	);
}

constexpr auto evs_to_string(OldGenEVs const evs) -> containers::string {
	return stats_to_string<SpecialPermanentStat>("EVs: "_sv, evs, EV::useful_max);
}

constexpr auto evs_to_string(EVs const evs) -> containers::string {
	return stats_to_string<SplitSpecialPermanentStat>("EVs: "_sv, evs, 0_bi);
}

constexpr auto pp_ups_to_string(PP::pp_ups_type const pp_ups) -> containers::string {
	return pp_ups != numeric_traits::max_value<PP::pp_ups_type> ?
		containers::concatenate<containers::string>(
			" ("_sv,
			containers::to_string(pp_ups),
			" PP ups)"_sv
		) :
		containers::string(""_sv);
}

constexpr auto moves_to_string(InitialMoves const moves) -> containers::string {
	return containers::string(containers::join_with(
		containers::transform(moves, [](InitialMove const move) {
			return containers::concatenate<containers::string>(
				"- "_sv,
				to_string(move.name),
				pp_ups_to_string(move.pp_ups)
			);
		}),
		"\n"_sv
	));
};

export template<SpecialInputStyle style>
constexpr auto to_string(InitialPokemon<style> const & pokemon) -> containers::string {
	auto const species_str = to_string(pokemon.species);
	return containers::concatenate<containers::string>(
		pokemon.nickname.str(),
		species_str != pokemon.nickname ?
			containers::concatenate<containers::string>(" ("_sv, species_str, ")"_sv) :
			containers::string(""_sv),
		pokemon.gender != Gender::genderless ?
			containers::concatenate<containers::string>(
				" ("_sv,
				to_string(pokemon.gender),
				")"_sv
			) :
			containers::string(""_sv),
		pokemon.item != Item::None ?
			containers::concatenate<containers::string>(" @ "_sv, to_string(pokemon.item)) :
			containers::string(""_sv),
		"\n"_sv,
		// Technically not the right check, but in practice it's always accurate
		style == SpecialInputStyle::split ?
			containers::concatenate<containers::string>(
				"Ability: "_sv,
				to_string(pokemon.ability),
				"\n"_sv
			) :
			containers::string(""_sv),
		pokemon.level != Level(100_bi) ?
			containers::concatenate<containers::string>(
				"Level: "_sv,
				containers::to_string(pokemon.level()),
				"\n"_sv
			) :
			containers::string(""_sv),
		pokemon.happiness != Happiness() ?
			containers::concatenate<containers::string>(
				"Happiness: "_sv,
				containers::to_string(pokemon.happiness()),
				"\n"_sv
			) :
			containers::string(""_sv),
		evs_to_string(pokemon.stats.evs),
		// Technically not the right check, but in practice it's always accurate
		style == SpecialInputStyle::split ?
			containers::concatenate<containers::string>(
				to_string(pokemon.stats.nature),
				" Nature\n"_sv
			) :
			containers::string(""_sv),
		ivs_to_string(pokemon.stats.dvs_or_ivs),
		moves_to_string(pokemon.moves)
	);
}

export template<SpecialInputStyle style>
constexpr auto to_string(InitialTeam<style> const & team) -> containers::string {
	return containers::string(containers::join_with(
		containers::transform(team, [](auto const & pokemon) {
			return to_string(pokemon);
		}),
		"\n\n"_sv
	));
}

export constexpr auto to_string(AnyInitialTeam const & team) -> containers::string {
	return tv::visit(team, [](auto const & t) { return to_string(t); });
}

} // namespace technicalmachine
