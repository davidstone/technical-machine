// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.make_battle_init_message;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.in_message;
import tm.clients.ps.make_party;
import tm.clients.ps.parse_switch;
import tm.clients.ps.switch_message;

import tm.clients.turn_count;

import tm.pokemon.max_pokemon_per_team;

import tm.string_conversions.generation;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

struct PerTeam {
	tv::optional<TeamSize> size;
	tv::optional<SwitchMessage> starter;
};

// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
export constexpr auto make_battle_init_message(auto const messages) -> BattleInitMessage {
	auto generation = tv::optional<Generation>();
	auto teams = containers::array<PerTeam, 2_bi>();
	for (InMessage message : messages) {
		auto const type = message.type();
		auto matches = [&](auto... strs) {
			return (... or (strs == type));
		};
		if (matches("gen"sv)) {
			if (generation) {
				throw std::runtime_error("Received gen multiple times");
			}
			generation = from_string<Generation>(message.pop());
		} else if (matches("switch"sv)) {
			auto parsed = parse_switch(message);
			auto & target = teams[parsed.party.value()].starter;
			if (target) {
				throw std::runtime_error("Same party switched in twice");
			}
			target = parsed;
		} else if (matches("teamsize"sv)) {
			auto const party = make_party(message.pop());
			auto const team_size = bounded::to_integer<TeamSize>(message.pop());
			auto & target = teams[party.value()].size;
			if (target) {
				throw std::runtime_error("Same party gave a team size twice");
			}
			target = team_size;
		} else if (matches("turn"sv)) {
			auto const turn = bounded::to_integer<TurnCount>(message.pop());
			if (turn != 1_bi) {
				throw std::runtime_error("Got a turn after turn 1 during battle set-up");
			}
		} else if (matches(""sv, "player"sv, "rated"sv, "rule"sv, "start"sv, "t:"sv, "teampreview"sv, "tier"sv)) {
			// Do nothing, junk
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Received battle setup message of unknown type: "sv,
				type,
				": "sv,
				message.remainder()
			));
		}
	}

	if (!generation) {
		throw std::runtime_error("Did not see generation");
	}
	for (auto const team : teams) {
		if (!team.size or !team.starter) {
			throw std::runtime_error("Did not receive all starting team information");
		}
	}
	auto make_team = [&](auto const index) {
		auto const team = teams[index];
		return BattleInitMessage::Team(*team.starter, *team.size);
	};
	return BattleInitMessage(
		*generation,
		{make_team(0_bi), make_team(1_bi)}
	);
}

} // namespace technicalmachine::ps
