// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.parse_team;

import tm.clients.ps.parse_details;
import tm.clients.ps.parse_pokemon_identity;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.hidden_power;
import tm.pokemon.known_pokemon;
import tm.pokemon.max_pokemon_per_team;

import tm.stat.calculate_ivs_and_evs;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.max_hp;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.type;

import tm.type.type;

import tm.ability;
import tm.generation;
import tm.item;
import tm.nlohmann_json;
import tm.split_view;
import tm.team;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;

template<StatStyle stat_style>
auto parse_stats(MaxHP const hp, nlohmann::json const & stats) {
	auto get = [&](std::string_view const str) {
		return bounded::check_in_range<InitialStat<stat_style>>(bounded::integer(stats.at(str).get<nlohmann::json::number_integer_t>()));
	};
	auto const attack = get("atk");
	auto const defense = get("def");
	auto const special_attack = get("spa");
	auto const special_defense = get("spd");
	auto const speed = get("spe");
	return Stats<stat_style>(HP(hp), attack, defense, special_attack, special_defense, speed);
}

template<Generation generation>
constexpr auto parse_hidden_power(std::string_view const str) {
	constexpr auto prefix = std::string_view("hiddenpower");
	if (!str.starts_with(prefix)) {
		throw std::runtime_error("Hidden Power doesn't start with \"hiddenpower\" in PS team string");
	}
	// TODO: ???
	constexpr auto const power = [] {
		if constexpr (generation <= Generation::five) {
			return 70_bi;
		} else {
			return 60_bi;
		}
	}();
	auto const type = from_string<Type>(str.substr(prefix.size()));
	return HiddenPower<generation>(power, type);
}

template<Generation generation>
auto parse_moves(nlohmann::json const & moves) {
	struct Result {
		RegularMoves moves;
		tv::optional<HiddenPower<generation>> hidden_power;
	};
	auto result = Result();
	for (auto const & move : moves) {
		auto const move_str = move.get<std::string_view>();
		auto const move_name = from_string<MoveName>(move_str);
		result.moves.push_back(Move(generation, move_name));
		if (move_name == MoveName::Hidden_Power) {
			if constexpr (generation == Generation::one) {
				throw std::runtime_error("Generation 1 does not have Hidden Power");
			} else {
				insert(result.hidden_power, parse_hidden_power<generation>(move_str));
			}
		}
	}
	return result;
}

template<Generation generation>
auto parse_pokemon(nlohmann::json const & pokemon_data) {
	auto get = [&](std::string_view const key) { return pokemon_data.at(key).get<std::string_view>(); };

	auto const nickname = parse_pokemon_identity(get("ident")).nickname;
	auto const details = parse_details(get("details"));

	auto const condition = get("condition");
	// current_hp/max_hp
	// Presumably also gives me status information? Should be useful for
	// rejoining battles
	// TODO: If we disconnect in a battle when the HP is 0, we might not
	// have a '/'
	auto const hp = bounded::to_integer<MaxHP>(split_view(condition, '/').first);

	auto const move_data = parse_moves<generation>(pokemon_data.at("moves"));
	auto const stats = calculate_ivs_and_evs(
		details.species,
		details.level,
		parse_stats<stat_style_for(generation)>(hp, pokemon_data.at("stats")),
		move_data.hidden_power
	);

	auto const ability = from_string<Ability>(get("baseAbility"));

	auto const item = from_string<Item>(get("item"));

	return KnownPokemon<generation>(
		details.species,
		nickname,
		details.level,
		details.gender,
		item,
		ability,
		stats,
		move_data.moves
	);
}

export template<Generation generation>
auto parse_team(std::string_view const str) -> KnownTeam<generation> {
	auto const json = nlohmann::json::parse(str).at("side").at("pokemon");
	return KnownTeam<generation>(
		containers::transform(
			json,
			parse_pokemon<generation>
		)
	);
}

} // namespace technicalmachine::ps
