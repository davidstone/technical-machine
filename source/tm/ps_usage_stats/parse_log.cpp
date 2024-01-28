// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.ps_usage_stats.parse_log;

import tm.clients.ps.parse_gender;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.parse_moves;
import tm.clients.ps.parsed_team;

import tm.move.move_name;

import tm.pokemon.happiness;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.rating;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.generic_stats;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.nlohmann_json;
import tm.visible_hp;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps_usage_stats {

using namespace bounded::literal;

template<typename T>
auto parse_stats(nlohmann::json const & stats) {
	auto read = [&](std::string_view const key) {
		return T(bounded::check_in_range<typename T::value_type>(
			stats.at(key).get<nlohmann::json::number_integer_t>()
		));
	};
	return GenericStats(
		read("hp"),
		read("atk"),
		read("def"),
		read("spa"),
		read("spd"),
		read("spe")
	);
}

constexpr auto parse_nature(std::string_view const str) -> Nature {
	// A bug in the PS stats causes it to sometimes emit an empty nature
	return str == "" ? Nature::Hardy : from_string<Nature>(str);
}

constexpr auto parse_pokemon = [](Generation const generation, nlohmann::json const & pokemon) -> ps::ParsedPokemon {
	auto const species = from_string<Species>(pokemon.at("species").get<std::string_view>());
	auto const moves = ps::parse_moves(pokemon.at("moves"));
	auto const level = Level(bounded::check_in_range<Level::value_type>(pokemon.at("level").get<nlohmann::json::number_integer_t>()));
	auto const nature = parse_nature(pokemon.at("nature").get<std::string_view>());
	auto const evs = parse_stats<EV>(pokemon.at("evs"));
	auto const ivs = parse_stats<IV>(pokemon.at("ivs"));
	auto const base_stats = BaseStats(generation, species);
	auto get_hp = [=] {
		auto const hp = HP(base_stats.hp(), level, ivs.hp(), evs.hp());
		return VisibleHP(
			CurrentVisibleHP(hp.current()),
			MaxVisibleHP(hp.max())
		);
	};
	auto get_stat = [=](SplitSpecialRegularStat const stat_name) {
		return initial_stat<SpecialStyle::split>(
			stat_name,
			base_stats[stat_name],
			level,
			nature,
			ivs[stat_name],
			evs[stat_name]
		);
	};
	return ps::ParsedPokemon(
		species,
		level,
		ps::parse_gender(pokemon.value("gender", "")),
		StatusName::clear,
		from_string<Item>(pokemon.at("item").get<std::string_view>()),
		from_string<Ability>(pokemon.at("ability").get<std::string_view>()),
		ps::ParsedStats(
			get_hp(),
			get_stat(SplitSpecialRegularStat::atk),
			get_stat(SplitSpecialRegularStat::def),
			get_stat(SplitSpecialRegularStat::spa),
			get_stat(SplitSpecialRegularStat::spd),
			get_stat(SplitSpecialRegularStat::spe)
		),
		moves.names,
		moves.hidden_power_type,
		Happiness(bounded::check_in_range<Happiness::value_type>(pokemon.value("happiness", 255)))
	);
};

auto parse_team(Generation const generation, nlohmann::json const & team_array) -> ps::ParsedTeam {
	return ps::ParsedTeam(containers::transform(
		team_array,
		[=](nlohmann::json const & pokemon) {
			return parse_pokemon(generation, pokemon);
		}
	));
}

auto parse_rating(nlohmann::json const & json) -> tv::optional<Rating> {
	if (json.is_null()) {
		return tv::none;
	}
	// Sometimes this is a number in which case it is Elo
	if (!json.is_object()) {
		return tv::none;
	}
	auto const & rating = json.at("rpr");
	auto const & deviation = json.at("rprd");
	if (rating.is_null() or deviation.is_null()) {
		return tv::none;
	}
	return Rating(
		rating.get<nlohmann::json::number_float_t>(),
		deviation.get<nlohmann::json::number_float_t>()
	);
}

enum class Anonymization {
	full,
	hashed,
	integer,
	none
};

constexpr auto hex_digit(char const c) -> bounded::integer<0, 15> {
	switch (c) {
		case '0': return 0_bi;
		case '1': return 1_bi;
		case '2': return 2_bi;
		case '3': return 3_bi;
		case '4': return 4_bi;
		case '5': return 5_bi;
		case '6': return 6_bi;
		case '7': return 7_bi;
		case '8': return 8_bi;
		case '9': return 9_bi;
		case 'a': return 10_bi;
		case 'b': return 11_bi;
		case 'c': return 12_bi;
		case 'd': return 13_bi;
		case 'e': return 14_bi;
		case 'f': return 15_bi;
		default: throw std::runtime_error("Invalid hex digit");
	}
}

constexpr auto hex_to_integer(std::string_view const str) {
	auto result = BattleResult::Side::ID(0_bi);
	for (auto const c : str) {
		result *= 16_bi;
		result += hex_digit(c);
	}
	return result;
}

constexpr auto to_player_id(std::string_view const str) {
	constexpr auto anonymization = Anonymization::hashed;
	switch (anonymization) {
		case Anonymization::full:
			return
				str == "Player 1" ? BattleResult::Side::ID(1_bi) :
				str == "Player 2" ? BattleResult::Side::ID(2_bi) :
				throw std::runtime_error("Invalid fully anonymized username");
		case Anonymization::hashed:
			return hex_to_integer(str);
		case Anonymization::integer:
			return bounded::to_integer<BattleResult::Side::ID>(str);
		case Anonymization::none:
			throw std::runtime_error("Implement this");
	}
}

export auto parse_log(nlohmann::json const & json) -> tv::optional<BattleResult> {
	if (json.at("turns").get<nlohmann::json::number_integer_t>() < 3) {
		return tv::none;
	}
	auto const format = json.at("format").get<std::string_view>();
	auto const generation = ps::parse_generation_from_format(format, "gen");
	auto const p1 = json.at("p1").get<std::string_view>();
	auto const p2 = json.at("p2").get<std::string_view>();
	auto const winner_str = json.at("winner").get<std::string_view>();
	// Anonymized logs send you a bogus team for a tie
	auto const winner =
		winner_str == p1 ? BattleResult::Winner::side1 :
		winner_str == p2 ? BattleResult::Winner::side2 :
		BattleResult::Winner::tie;

	return BattleResult{
		BattleResult::Side{
			parse_team(generation, json.at("p1team")),
			to_player_id(p1),
			// Arr
			parse_rating(json.at("p1rating"))
		},
		BattleResult::Side{
			parse_team(generation, json.at("p2team")),
			to_player_id(p2),
			parse_rating(json.at("p2rating"))
		},
		winner
	};
}

} // namespace technicalmachine::ps_usage_stats
