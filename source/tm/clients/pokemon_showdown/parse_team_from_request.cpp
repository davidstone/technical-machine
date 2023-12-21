// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_team_from_request;

import tm.clients.ps.make_party;
import tm.clients.ps.parse_details;
import tm.clients.ps.parse_hp_and_status;
import tm.clients.ps.parse_pokemon_identity;
import tm.clients.ps.parsed_team;

import tm.move.move_name;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.type;

import tm.type.type;

import tm.ability;
import tm.item;
import tm.nlohmann_json;
import tm.visible_hp;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;

auto parse_stats(VisibleHP const hp, nlohmann::json const & stats) -> ParsedStats {
	auto get = [&](std::string_view const str) {
		auto const received = stats.at(str).get<nlohmann::json::number_integer_t>();
		return bounded::check_in_range<ParsedStat>(received);
	};
	return ParsedStats(
		hp,
		get("atk"),
		get("def"),
		get("spa"),
		get("spd"),
		get("spe")
	);
}

constexpr auto parse_hidden_power(std::string_view const str) -> Type {
	constexpr auto prefix = std::string_view("hiddenpower");
	if (!str.starts_with(prefix)) {
		throw std::runtime_error("Hidden Power doesn't start with \"hiddenpower\" in PS team string");
	}
	return from_string<Type>(str.substr(prefix.size()));
}

struct MovesAndHiddenPower {
	ParsedMoves names;
	tv::optional<Type> hidden_power_type;
};

auto parse_moves(nlohmann::json const & moves) -> MovesAndHiddenPower {
	auto result = MovesAndHiddenPower();
	for (auto const & move : moves) {
		auto const move_str = move.get<std::string_view>();
		auto const move_name = from_string<MoveName>(move_str);
		containers::push_back(result.names, move_name);
		if (move_name == MoveName::Hidden_Power) {
			if (result.hidden_power_type) {
				throw std::runtime_error("Got two Hidden Powers");
			}
			result.hidden_power_type = parse_hidden_power(move_str);
		}
	}
	return result;
}

auto parse_pokemon(nlohmann::json const & json) -> ParsedPokemon {
	auto get = [&](std::string_view const key) {
		return json.at(key).get<std::string_view>();
	};
	auto const details = parse_details(get("details"));
	auto const hp_and_status = parse_hp_and_status(get("condition"));
	auto const moves = parse_moves(json.at("moves"));
	return ParsedPokemon(
		details.species,
		parse_pokemon_identity(get("ident")).nickname,
		details.level,
		details.gender,
		hp_and_status.status,
		from_string<Item>(get("item")),
		from_string<Ability>(get("baseAbility")),
		parse_stats(hp_and_status.hp, json.at("stats")),
		moves.names,
		moves.hidden_power_type
	);
}

export auto parse_team_from_request(std::string_view const str) -> ParsedSide {
	auto const json = nlohmann::json::parse(str).at("side");
	return ParsedSide(
		make_party(json.at("id").get<std::string_view>()),
		ParsedTeam(containers::transform(json.at("pokemon"), parse_pokemon))
	);
}

} // namespace technicalmachine::ps
