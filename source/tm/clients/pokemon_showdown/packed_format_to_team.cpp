// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string_view>

export module tm.clients.ps.packed_format_to_team;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.happiness;
import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.generic_stats;
import tm.stat.iv;
import tm.stat.nature;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.ability;
import tm.buffer_view;
import tm.gender;
import tm.generation;
import tm.item;
export import tm.team;

import bounded;
import containers;
import numeric_traits;
import std_module;

// https://github.com/smogon/pokemon-showdown/blob/master/sim/TEAMS.md#packed-format

namespace technicalmachine::ps {
using namespace bounded::literal;

constexpr auto pokemon_delimiter = ']';

auto parse_species(std::string_view const species_str, std::string_view const nickname) {
	return from_string<Species>(species_str.empty() ? nickname : species_str);
}

constexpr auto parse_ability(std::string_view const ability_str, Species const species [[maybe_unused]]) {
	// TODO: Get the correct values for this
	return
		ability_str == "" ? Ability::Honey_Gather :
		ability_str == "0" ? Ability::Honey_Gather :
		ability_str == "1" ? Ability::Honey_Gather :
		ability_str == "H" ? Ability::Honey_Gather :
		from_string<Ability>(ability_str);
}

constexpr auto parse_moves(std::string_view const str, Generation const generation) {
	auto buffer = DelimitedBufferView(str, ',');
	auto moves = RegularMoves();
	while (!buffer.remainder().empty()) {
		moves.push_back(Move(generation, from_string<MoveName>(buffer.pop())));
	}
	return moves;
}

constexpr auto parse_nature(std::string_view const str) {
	return str.empty() ? Nature::Serious : from_string<Nature>(str);
}

constexpr auto parse_stat_components(std::string_view const str, auto parse_value) {
	auto buffer = DelimitedBufferView(str, ',');
	auto next = [&] {
		return parse_value(buffer.pop());
	};
	auto const hp = next();
	auto const atk = next();
	auto const def = next();
	auto const spa = next();
	auto const spd = next();
	auto const spe = next();
	return GenericStats(hp, atk, def, spa, spd, spe);
}

auto parse_gender(std::string_view const str) {
	return str.empty() ? Gender::genderless : from_string<Gender>(str);
}

constexpr auto parse_shiny(std::string_view const str) {
	return
		str == "S" ? true :
		str == "" ? false :
		throw std::runtime_error(containers::concatenate<std::string>(std::string_view("Invalid shiny string: "), str));
}

template<typename T>
constexpr auto parse_integer_wrapper(std::string_view const str) {
	using integer = typename T::value_type;
	return T(str.empty() ? numeric_traits::max_value<integer> : bounded::to_integer<integer>(str));
}

template<typename T>
constexpr auto parse_ev_or_iv(T const default_value) {
	return [=](std::string_view const maybe) {
		return maybe.empty() ? default_value : T(bounded::to_integer<typename T::value_type>(maybe));
	};
}

template<Generation generation>
constexpr auto parse_evs(std::string_view const str) {
	auto const evs = parse_stat_components(str, parse_ev_or_iv(EV(0_bi)));
	if constexpr (generation <= Generation::two) {
		return to_old_gen_evs(evs);
	} else {
		return evs;
	}
}

template<Generation generation>
constexpr auto parse_dv_or_iv = [](std::string_view const maybe) {
	auto const iv = parse_ev_or_iv(IV(31_bi))(maybe);
	if constexpr (generation <= Generation::two) {
		return DV(iv);
	} else {
		return iv;
	}
};

template<Generation generation>
constexpr auto parse_dvs_or_ivs(std::string_view const str) {
	auto const stats = parse_stat_components(str, parse_dv_or_iv<generation>);
	if constexpr (generation <= Generation::two) {
		return to_dvs(stats);
	} else {
		return stats;
	}
}

template<Generation generation>
auto parse_pokemon(std::string_view const str) {
	auto buffer = DelimitedBufferView(str, '|');
	auto const nickname = Nickname(buffer.pop());
	auto const species = parse_species(buffer.pop(), nickname);
	auto const item = from_string<Item>(buffer.pop());
	auto const ability = parse_ability(buffer.pop(), species);
	auto const moves = parse_moves(buffer.pop(), generation);
	auto const nature = parse_nature(buffer.pop());
	auto const evs = parse_evs<generation>(buffer.pop());
	auto const gender = parse_gender(buffer.pop());
	auto const dvs_or_ivs = parse_dvs_or_ivs<generation>(buffer.pop());
	auto const shiny [[maybe_unused]] = parse_shiny(buffer.pop());
	auto const level = parse_integer_wrapper<Level>(buffer.pop());
	auto const happiness = parse_integer_wrapper<Happiness>(buffer.pop(','));
	auto const pokeball [[maybe_unused]] = buffer.pop(',');
	// TODO: Support Hyper Training
	auto const hidden_power_type [[maybe_unused]] = buffer.remainder();
	auto pokemon = KnownPokemon<generation>(
		species,
		nickname,
		level,
		gender,
		item,
		ability,
		CombinedStats<generation>{nature, dvs_or_ivs, evs},
		moves,
		happiness
	);
	return pokemon;
}

export template<Generation generation>
auto packed_format_to_team(std::string_view const str) -> KnownTeam<generation> {
	// TODO: Update this when DelimitedBufferView is a range
	auto buffer = DelimitedBufferView(str, pokemon_delimiter);
	auto all_pokemon = containers::static_vector<KnownPokemon<generation>, max_pokemon_per_team>();
	while (!buffer.remainder().empty()) {
		containers::push_back(all_pokemon, parse_pokemon<generation>(buffer.pop()));
	}
	return KnownTeam<generation>(std::move(all_pokemon));
}

} // namespace technicalmachine::ps
