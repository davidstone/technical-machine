// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_request;

import tm.clients.ps.make_party;
import tm.clients.ps.parse_details;
import tm.clients.ps.parse_hp_and_status;
import tm.clients.ps.parse_moves;
import tm.clients.ps.parsed_request;
import tm.clients.ps.parsed_side;
import tm.clients.ps.parsed_stats;
import tm.clients.ps.parsed_team;

import tm.move.move_name;
import tm.move.pp;

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

auto parse_active_moves(nlohmann::json const & json) -> ParsedMoves {
	return ParsedMoves(containers::transform(
		json,
		[](nlohmann::json const & move) {
			return ParsedMove(
				from_string<MoveName>(move.at("id").get<std::string_view>()),
				bounded::check_in_range<PP::current_type>(
					move.at("pp").get<nlohmann::json::number_integer_t>()
				),
				!move.at("disabled").get<bool>()
			);
		}
	));
}

auto parse_switching_allowed(nlohmann::json const & json) -> SwitchPossibilities {
	if (json.value("maybeTrapped", false)) {
		return SwitchPossibilities::maybe_trapped;
	} else if (json.value("trapped", false)) {
		return SwitchPossibilities::trapped;
	} else {
		return SwitchPossibilities::allowed;
	}
}

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

auto parse_pokemon(nlohmann::json const & json) -> ParsedPokemon {
	auto get = [&](std::string_view const key) {
		return json.at(key).get<std::string_view>();
	};
	auto const details = parse_details(get("details"));
	auto const hp_and_status = parse_hp_and_status(get("condition"));
	auto const moves = parse_moves(json.at("moves"));
	// auto const nickname = parse_pokemon_identity(get("ident")).nickname;
	return ParsedPokemon(
		details.species,
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

auto parse_team_from_request(nlohmann::json const & json) -> ParsedSide {
	return ParsedSide(
		make_party(json.at("id").get<std::string_view>()),
		ParsedTeam(containers::transform(json.at("pokemon"), parse_pokemon))
	);
}

auto is_force_switch(nlohmann::json const & json) -> bool {
	auto const it = json.find("forceSwitch");
	if (it == json.end()) {
		return false;
	}
	auto const & value = *it;
	if (value.size() != 1) {
		throw std::runtime_error("Expected a single active Pokemon for forceSwitch");
	}
	if (!value[0].get<bool>()) {
		throw std::runtime_error("Got a forceSwitch message without a forced switch");
	}
	return true;
}

export auto parse_request(std::string_view const str) -> ParsedRequest {
	auto const json = nlohmann::json::parse(str);
	auto get_side = [&] {
		return parse_team_from_request(json.at("side"));
	};
	if (json.value("wait", false)) {
		// The only legal selection is pass
		return ParsedRequest(
			ParsedMoves(),
			SwitchPossibilities::trapped,
			get_side()
		);
	}
	if (is_force_switch(json)) {
		return ParsedRequest(
			ParsedMoves(),
			SwitchPossibilities::forced,
			get_side()
		);
	}
	auto const & all_active = json.at("active");
	if (all_active.size() != 1) {
		throw std::runtime_error("Expected a single active Pokemon");
	}
	auto const & active = all_active[0];
	return ParsedRequest(
		parse_active_moves(active.at("moves")),
		parse_switching_allowed(active),
		get_side()
	);
}

} // namespace technicalmachine::ps
