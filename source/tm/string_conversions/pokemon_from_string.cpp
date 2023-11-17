// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.pokemon_from_string;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.current_hp;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.pokemon_helpers;
import tm.string_conversions.species;
import tm.string_conversions.status_name;

import tm.ability;
import tm.buffer_view;
import tm.gender;
import tm.generation;
import tm.item;
import tm.split_view;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

using namespace std::string_view_literals;

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
		CombinedStatsFor<generation>{nature, dvs_or_ivs, evs},
		moves
	);
	pokemon.set_hp(bounded::assume_in_range<CurrentHP>(static_cast<int>(static_cast<double>(pokemon.hp().max()) * hp_percent / 100.0)));
	pokemon.set_status(status);

	return pokemon;
}

} // namespace technicalmachine
