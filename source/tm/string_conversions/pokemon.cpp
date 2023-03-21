// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <tm/for_each_generation.hpp>

export module tm.string_conversions.pokemon;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.any_pokemon;
import tm.pokemon.hp_ratio;
import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.combined_stats;
import tm.stat.current_hp;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;
import tm.string_conversions.status_name;

import tm.status.status;
import tm.status.status_name;

import tm.ability;
import tm.buffer_view;
import tm.gender;
import tm.generation;
import tm.item;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

using namespace std::string_view_literals;

constexpr auto species_hp = " ("sv;
constexpr auto hp_item = "% HP) @ "sv;
constexpr auto item_ability = "\n\tAbility: "sv;
constexpr auto ability_status = "\n\tStatus: "sv;

constexpr auto status_nature = "\n\tNature: "sv;
constexpr auto nature_hp_iv = "\n\tIVs: "sv;
constexpr auto hp_iv_atk_iv = " HP / "sv;
constexpr auto atk_iv_def_iv = " Atk / "sv;
constexpr auto def_iv_spa_iv = " Def / "sv;
constexpr auto spa_iv_spd_iv = " SpA / "sv;
constexpr auto spd_iv_spe_iv = " SpD / "sv;
constexpr auto spe_iv_hp_ev = " Spe\n\tEVs: "sv;
constexpr auto hp_ev_atk_ev = " HP / "sv;
constexpr auto atk_ev_def_ev = " Atk / "sv;
constexpr auto def_ev_spa_ev = " Def / "sv;
constexpr auto spa_ev_spd_ev = " SpA / "sv;
constexpr auto spd_ev_spe_ev = " SpD / "sv;
constexpr auto spe_ev_moves = " Spe"sv;

constexpr auto status_atk_dv = "\n\tDVs: "sv;
constexpr auto atk_dv_def_dv = " Atk / "sv;
constexpr auto def_dv_spe_dv = " Def / "sv;
constexpr auto spe_dv_spc_dv = " Spe / "sv;
constexpr auto spc_dv_old_hp_ev = " Spc\n\tEVs: "sv;
constexpr auto old_hp_ev_atk_ev = " HP / "sv;
constexpr auto old_atk_ev_def_ev = " Atk / "sv;
constexpr auto old_def_ev_spe_ev = " Def / "sv;
constexpr auto old_spe_ev_spc_ev = " Spe / "sv;
constexpr auto old_spc_ev_moves = " Spc"sv;

constexpr auto moves_separator = "\n\t- "sv;

// TODO: Print nickname
// TODO: Print level
// TODO: Print gender
// TODO: Make this compatible with Pokemon Showdown

constexpr auto percent_to_string(double const value) -> containers::string {
	BOUNDED_ASSERT(0.0 <= value <= 100.0);
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
			containers::concatenate<containers::string>(ability_status, to_string(pokemon.status().name())) :
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
				atk_dv_def_dv,
				stat_to_iv_string(SpecialRegularStat::def),
				def_dv_spe_dv,
				stat_to_iv_string(SpecialRegularStat::spe),
				spe_dv_spc_dv,
				stat_to_iv_string(SpecialRegularStat::spc),
				spc_dv_old_hp_ev,
				stat_to_ev_string(SpecialPermanentStat::hp),
				old_hp_ev_atk_ev,
				stat_to_ev_string(SpecialPermanentStat::atk),
				old_atk_ev_def_ev,
				stat_to_ev_string(SpecialPermanentStat::def),
				old_def_ev_spe_ev,
				stat_to_ev_string(SpecialPermanentStat::spe),
				old_spe_ev_spc_ev,
				stat_to_ev_string(SpecialPermanentStat::spc),
				old_spc_ev_moves
			);
		} else {
			return containers::concatenate<containers::string>(
				to_string(pokemon.nature()),
				nature_hp_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::hp),
				hp_iv_atk_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::atk),
				atk_iv_def_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::def),
				def_iv_spa_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spa),
				spa_iv_spd_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spd),
				spd_iv_spe_iv,
				stat_to_iv_string(SplitSpecialPermanentStat::spe),
				spe_iv_hp_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::hp),
				hp_ev_atk_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::atk),
				atk_ev_def_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::def),
				def_ev_spa_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spa),
				spa_ev_spd_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spd),
				spd_ev_spe_ev,
				stat_to_ev_string(SplitSpecialPermanentStat::spe),
				spe_ev_moves
			);
		}
	};

	auto moves_to_string = [&] {
		containers::string output;
		for (auto const & move : pokemon.regular_moves()) {
			output = containers::concatenate<containers::string>(std::move(output), moves_separator, to_string(move.name()));
		}
		return output;
	};

	return containers::concatenate<containers::string>(
		to_string(pokemon.species()),
		species_hp,
		hp_str,
		hp_item,
		to_string(pokemon.item(false, false)),
		item_ability,
		to_string(pokemon.initial_ability()),
		status_to_string(),
		generation <= Generation::two ? status_atk_dv : status_nature,
		stats_to_string(),
		moves_to_string()
	);
}

template<typename T>
constexpr auto typed_pop(BufferView<std::string_view> & buffer, std::string_view const delimiter) {
	auto const str = pop_to_delimiter(buffer, delimiter);
	if constexpr (std::same_as<T, double>) {
		// TODO: Use std::from_chars when that is implemented
		return std::stod(std::string(str));
	} else if constexpr (bounded::bounded_integer<T>) {
		return bounded::to_integer<T>(str);
	} else {
		return from_string<T>(str);
	}
}

struct AbilityAndStatus {
	Ability ability;
	StatusName status;
};

constexpr auto pop_ability_and_status(BufferView<std::string_view> & buffer, Generation const generation) {
	auto const ability_and_status_str = pop_to_delimiter(buffer, generation <= Generation::two ? status_atk_dv : status_nature);
	auto const [ability_str, status_str] = split_view(ability_and_status_str, ability_status);
	return AbilityAndStatus{
		from_string<Ability>(ability_str),
		status_str.empty() ? StatusName::clear : from_string<StatusName>(status_str)
	};
}

template<typename T>
constexpr auto pop_value_type(BufferView<std::string_view> & buffer, std::string_view const delimiter) {
	return T(typed_pop<typename T::value_type>(buffer, delimiter));
}

template<Generation generation>
constexpr auto pop_dvs_or_ivs(BufferView<std::string_view> & buffer) {
	if constexpr (generation <= Generation::two) {
		auto const atk = pop_value_type<DV>(buffer, atk_dv_def_dv);
		auto const def = pop_value_type<DV>(buffer, def_dv_spe_dv);
		auto const spe = pop_value_type<DV>(buffer, spe_dv_spc_dv);
		auto const spc = pop_value_type<DV>(buffer, spc_dv_old_hp_ev);
		return DVs(atk, def, spe, spc);
	} else {
		auto const hp = pop_value_type<IV>(buffer, hp_iv_atk_iv);
		auto const atk = pop_value_type<IV>(buffer, atk_iv_def_iv);
		auto const def = pop_value_type<IV>(buffer, def_iv_spa_iv);
		auto const spa = pop_value_type<IV>(buffer, spa_iv_spd_iv);
		auto const spd = pop_value_type<IV>(buffer, spd_iv_spe_iv);
		auto const spe = pop_value_type<IV>(buffer, spe_iv_hp_ev);
		return IVs(hp, atk, def, spa, spd, spe);
	}
}

template<Generation generation>
constexpr auto pop_evs(BufferView<std::string_view> & buffer) {
	if constexpr (generation <= Generation::two) {
		auto const hp = pop_value_type<EV>(buffer, old_hp_ev_atk_ev);
		auto const atk = pop_value_type<EV>(buffer, old_atk_ev_def_ev);
		auto const def = pop_value_type<EV>(buffer, old_def_ev_spe_ev);
		auto const spe = pop_value_type<EV>(buffer, old_spe_ev_spc_ev);
		auto const spc = pop_value_type<EV>(buffer, old_spc_ev_moves);
		return OldGenEVs(hp, atk, def, spe, spc);
	} else {
		auto const hp = pop_value_type<EV>(buffer, hp_ev_atk_ev);
		auto const atk = pop_value_type<EV>(buffer, atk_ev_def_ev);
		auto const def = pop_value_type<EV>(buffer, def_ev_spa_ev);
		auto const spa = pop_value_type<EV>(buffer, spa_ev_spd_ev);
		auto const spd = pop_value_type<EV>(buffer, spd_ev_spe_ev);
		auto const spe = pop_value_type<EV>(buffer, spe_ev_moves);
		return EVs(hp, atk, def, spa, spd, spe);
	}
}

export template<Generation generation>
constexpr auto pokemon_from_string(std::string_view const str) -> Pokemon<generation> {
	auto buffer = BufferView(str);
	auto const species = typed_pop<Species>(buffer, species_hp);
	auto const hp_percent = typed_pop<double>(buffer, hp_item);
	auto const item = typed_pop<Item>(buffer, item_ability);
	auto const [ability, status] = pop_ability_and_status(buffer, generation);
	auto const nature = generation <= Generation::two ? Nature::Hardy : typed_pop<Nature>(buffer, nature_hp_iv);

	auto const dvs_or_ivs = pop_dvs_or_ivs<generation>(buffer);
	auto const evs = pop_evs<generation>(buffer);

	auto const should_be_empty = pop_to_delimiter(buffer, moves_separator);
	if (!should_be_empty.empty()) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected empty string while parsing Pokemon string, got "sv, should_be_empty));
	}

	auto moves = RegularMoves();
	while (!buffer.remainder().empty()) {
		auto const move_name = typed_pop<MoveName>(buffer, moves_separator);
		containers::push_back(moves, Move(generation, move_name));
	}

	auto pokemon = Pokemon<generation>(
		species,
		Level(100_bi),
		Gender::genderless,
		item,
		ability,
		CombinedStats<generation>{nature, dvs_or_ivs, evs},
		moves
	);
	pokemon.set_hp(bounded::assume_in_range<CurrentHP>(static_cast<int>(static_cast<double>(pokemon.hp().max()) * hp_percent / 100.0)));
	pokemon.set_status(status);

	return pokemon;
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(PokemonType) \
	template auto to_string(PokemonType const & pokemon) -> containers::string

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(Pokemon<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownPokemon<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenPokemon<generation>); \
	template auto pokemon_from_string<generation>(std::string_view const str) -> Pokemon<generation>

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
