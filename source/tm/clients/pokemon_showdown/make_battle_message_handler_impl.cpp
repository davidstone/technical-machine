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
import tm.clients.ps.switch_message;
import tm.clients.ps.validate_generation;

import tm.clients.client_battle_inputs;
import tm.clients.party;
import tm.clients.teams;
import tm.clients.turn_count;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;

import tm.pokemon.max_pokemon_per_team;

import tm.team_predictor.usage_stats;

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
	TeamSize foe_team_size;
	SwitchMessage foe_starter;
	Party party;
};
constexpr auto read_all_messages(
	Generation const generation,
	tv::variant<std::string_view, Party> user,
	MessageBlock const messages
) -> Parsed {
	auto foe_team_size = tv::optional<TeamSize>();
	auto foe_starter = tv::optional<SwitchMessage>();
	auto ai_switched_in = false;
	for (auto message : messages) {
		auto const type = message.type();
		if (type == "") {
			// Unnecessary
#if 0
		} else if (type == "clearpoke") {
			// This appears to mean nothing
#endif
		} else if (type == "gen") {
			validate_generation(message.pop(), generation);
		} else if (type == "player") {
			tv::visit(user, tv::overload(
				[&](std::string_view const username) {
					auto const player_id = message.pop();
					auto const parsed_username = message.pop();
					auto const party = make_party(player_id);
					user = (parsed_username == username) ? party : other(party);
				},
				[](Party) {}
			));
			// message.remainder() == AVATAR
#if 0
		} else if (type == "poke") {
			// message.remainder() == PLAYER_ID|DETAILS|ITEM
#endif
		} else if (type == "rated") {
			// Received if and only if the game is rated. We don't care about this
		} else if (type == "rule") {
			// message.remainder() == RULE: DESCRIPTION
			// Received for each clause in effect
#if 0
		} else if (type == "seed") {
			// I have no idea what this is
#endif
		} else if (type == "start") {
			// We can't actually start the battle until we see the initial switch-in
		} else if (type == "switch") {
			tv::visit(user, tv::overload(
				[](std::string_view) {
					throw std::runtime_error("Received a switch message before receiving a party");
				},
				[&](Party const user_party) {
					auto parsed = parse_switch(message);
					if (user_party == parsed.party) {
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
				}
			));
		} else if (type == "t:") {
			// message.remainder() == time_t
		} else if (type == "teampreview") {
			// This appears to mean nothing
		} else if (type == "teamsize") {
			tv::visit(user, tv::overload(
				[](std::string_view) {
					throw std::runtime_error("Received a teamsize message before receiving a player id");
				},
				[&](Party const user_party) {
					auto const party = make_party(message.pop());
					auto const team_size = bounded::to_integer<TeamSize>(message.pop());
					// TODO: validate that the received teamsize matches my team size
					if (user_party != party) {
						insert(foe_team_size, team_size);
					}
				}
			));
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

	if (!foe_team_size) {
		throw std::runtime_error("Did not see foe's team size");
	}
	if (!ai_switched_in) {
		throw std::runtime_error("Did not see my team switch in the first Pokemon");
	}
	if (!foe_starter) {
		throw std::runtime_error("Did not see foe in the first Pokemon");
	}
	if (user.index() != bounded::type<Party>) {
		throw std::runtime_error("Did not see my own party");
	}
	return Parsed(
		*foe_team_size,
		*foe_starter,
		user[bounded::type<Party>]
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
	GenerationGeneric<KnownTeam> generic_team,
	tv::variant<std::string_view, Party> user,
	AllEvaluate const all_evaluate,
	UsageStats const & usage_stats,
	Depth const depth,
	AnalysisLogger analysis_logger,
	BattleInitMessage const & message
) -> BattleMessageHandler {
	auto parsed = read_all_messages(
		get_generation<KnownTeam>(generic_team),
		user,
		message.messages()
	);

	auto make = [&]<Generation generation>(KnownTeam<generation> team) -> GenerationGeneric<ClientBattleInputs> {
		return ClientBattleInputs<generation>(
			Teams<generation>{
				std::move(team),
				make_foe<generation>(std::move(parsed))
			},
			all_evaluate.get<generation>()
		);
	};
	return BattleMessageHandler(
		parsed.party,
		tv::visit(generic_team, make),
		std::move(analysis_logger),
		usage_stats,
		depth
	);
}

} // namespace technicalmachine::ps
