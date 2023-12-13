// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

module tm.clients.ps.make_battle_message_handler;

import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.make_party;
import tm.clients.ps.message_block;
import tm.clients.ps.parse_switch;
import tm.clients.ps.parsed_team_to_known_team;
import tm.clients.ps.switch_message;

import tm.clients.party;
import tm.clients.teams;
import tm.clients.turn_count;

import tm.pokemon.max_pokemon_per_team;

import tm.string_conversions.generation;

import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace std::string_view_literals;

struct Parsed {
	Generation generation;
	TeamSize foe_team_size;
	SwitchMessage foe_starter;
};
constexpr auto read_all_messages(
	Party const party,
	MessageBlock const messages
) -> Parsed {
	auto generation = tv::optional<Generation>();
	auto foe_team_size = tv::optional<TeamSize>();
	auto foe_starter = tv::optional<SwitchMessage>();
	auto ai_switched_in = false;
	for (auto message : messages) {
		auto const type = message.type();
		if (type == "") {
			// Unnecessary
		} else if (type == "gen") {
			if (generation) {
				throw std::runtime_error("Received gen multiple times");
			}
			generation = from_string<Generation>(message.pop());
		} else if (type == "player") {
			// message.remainder() == PLAYER_ID|USERNAME|AVATAR
		} else if (type == "rated") {
			// Received if and only if the game is rated. We don't care about this
		} else if (type == "rule") {
			// message.remainder() == RULE: DESCRIPTION
			// Received for each clause in effect
		} else if (type == "start") {
			// We can't actually start the battle until we see the initial switch-in
		} else if (type == "switch") {
			auto parsed = parse_switch(message);
			if (party == parsed.party) {
				if (ai_switched_in) {
					throw std::runtime_error("AI switched in twice");
				}
				ai_switched_in = true;
			} else {
				if (foe_starter) {
					throw std::runtime_error("Foe switched in twice");
				}
				insert(foe_starter, std::move(parsed));
			}
		} else if (type == "t:") {
			// message.remainder() == time_t
		} else if (type == "teampreview") {
			// This appears to mean nothing
		} else if (type == "teamsize") {
			auto const team_party = make_party(message.pop());
			auto const team_size = bounded::to_integer<TeamSize>(message.pop());
			// TODO: validate that the received teamsize matches my team size
			if (party != team_party) {
				insert(foe_team_size, team_size);
			}
		} else if (type == "tier") {
			// message.remainder() == string tier
		} else if (type == "turn") {
			auto const turn = bounded::to_integer<TurnCount>(message.pop());
			if (turn != 1_bi) {
				throw std::runtime_error("Got a turn after turn 1 during battle set-up");
			}
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
	if (!foe_team_size) {
		throw std::runtime_error("Did not see foe's team size");
	}
	if (!ai_switched_in) {
		throw std::runtime_error("Did not see my team switch in the first Pokemon");
	}
	if (!foe_starter) {
		throw std::runtime_error("Did not see foe in the first Pokemon");
	}
	return Parsed(
		*generation,
		*foe_team_size,
		*foe_starter
	);
}

template<Generation generation>
constexpr auto make_foe(Parsed parsed) -> SeenTeam<generation> {
	auto team = SeenTeam<generation>(parsed.foe_team_size);
	auto & pokemon = parsed.foe_starter;
	team.add_pokemon({
		pokemon.species,
		std::move(pokemon).nickname,
		pokemon.level,
		pokemon.gender
	});
	return team;
}

// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
auto make_battle_message_handler(
	ParsedTeam const & team,
	BattleInitMessage const & message
) -> BattleMessageHandler {
	auto parsed = read_all_messages(
		team.party,
		message.messages()
	);
	return BattleMessageHandler(
		team.party,
		constant_generation(parsed.generation, [&]<Generation generation>(constant_gen_t<generation>) -> GenerationGeneric<Teams> {
			return Teams<generation>(
				parsed_team_to_known_team<generation>(team),
				make_foe<generation>(std::move(parsed))
			);
		})
	);
}

} // namespace technicalmachine::ps
