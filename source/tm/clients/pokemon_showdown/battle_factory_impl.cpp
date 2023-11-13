// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

#include <bounded/assert.hpp>

module tm.clients.ps.battle_factory;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.battle_parser;
import tm.clients.ps.handle_chat_message;
import tm.clients.ps.in_message;
import tm.clients.ps.make_party;
import tm.clients.ps.parse_switch;
import tm.clients.ps.parse_team;
import tm.clients.ps.validate_generation;

import tm.clients.client_battle_inputs;
import tm.clients.party;
import tm.clients.teams;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;

import tm.pokemon.max_pokemon_per_team;

import tm.team_predictor.usage_stats;

import tm.constant_generation;
import tm.generation;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

template<Generation generation>
struct BattleFactoryImpl : BattleFactory {
	BattleFactoryImpl(
		containers::string username,
		Evaluate<generation> evaluate,
		UsageStats const & usage_stats,
		Depth depth,
		AnalysisLogger analysis_logger
	):
		m_username(std::move(username)),
		m_evaluate(evaluate),
		m_usage_stats(usage_stats),
		m_depth(depth),
		m_analysis_logger(std::move(analysis_logger))
	{
	}

	auto handle_message(InMessage message) -> BattleMessageResult final {
		if (handle_chat_message(message)) {
			return BattleContinues();
		}

		// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
		auto const type = message.type();
		if (type == "") {
			// Unnecessary
		} else if (type == "clearpoke") {
			// This appears to mean nothing
		} else if (type == "gametype") {
			insert(m_type, message.pop());
		} else if (type == "gen") {
			validate_generation(message.pop(), generation);
		} else if (type == "player") {
			auto const player_id = message.pop();
			auto const username = message.pop();
			if (username == m_username) {
				insert(m_party, make_party(player_id));
			}
			// message.remainder() == AVATAR
		} else if (type == "poke") {
			// message.remainder() == PLAYER_ID|DETAILS|ITEM
		} else if (type == "rated") {
			// Received if and only if the game is rated. We don't care about this
		} else if (type == "request") {
			// It seems that the game starts off with a single "request" that has
			// no data. Then it sends another request with data, which we need to
			// parse for Random Battles.

			// TODO: maybe just do this on random battles, since we know what our
			// team is otherwise?
			auto const json_data = message.remainder();
			if (!json_data.empty()) {
				tv::insert(m_team, parse_team<generation>(json_data));
			}
		} else if (type == "rule") {
			// message.remainder() == RULE: DESCRIPTION
			// Received for each clause in effect
		} else if (type == "seed") {
			// I have no idea what this is
		} else if (type == "start") {
			// We can't actually start the battle until we see the initial switch-in
		} else if (type == "switch") {
			if (!m_party) {
				throw std::runtime_error("Received a switch message before receiving a party");
			}
			auto parsed = parse_switch(message);
			if (*m_party == parsed.party) {
				if (m_ai_switched_in) {
					throw std::runtime_error("AI switched in twice");
				}
				m_ai_switched_in = true;
			} else {
				if (m_foe_starter) {
					throw std::runtime_error("Foe switched in twice");
				}
				insert(m_foe_starter, std::move(parsed));
			}
			if (completed()) {
				return BattleStarted();
			}
		} else if (type == "t:") {
			// message.remainder() == time_t
		} else if (type == "teampreview") {
			// This appears to mean nothing
		} else if (type == "teamsize") {
			if (!m_party) {
				throw std::runtime_error("Received a teamsize message before receiving a player id");
			}
			auto const party = make_party(message.pop());
			auto const team_size = bounded::to_integer<TeamSize>(message.pop());
			// TODO: validate that the received teamsize matches my team size
			if (*m_party != party) {
				insert(m_foe_team_size, team_size);
			}
		} else if (type == "tier") {
			insert(m_tier, message.pop());
		} else if (type == "title") {
			// message.remainder() == PLAYER1 vs. PLAYER2
		} else {
			std::cerr << "Received battle setup message of unknown type: " << type << ": " << message.remainder() << '\n';
		}
		return BattleContinues();
	}

	auto make() && -> BattleParser final {
		BOUNDED_ASSERT(completed());
		if (!m_team) {
			throw std::runtime_error("Did not receive team");
		}
		if (!m_party) {
			throw std::runtime_error("Did not receive party");
		}
		if (!m_type) {
			throw std::runtime_error("Did not receive battle format");
		}
		if (!m_tier) {
			throw std::runtime_error("Did not receive tier");
		}
		if (!m_foe_team_size) {
			throw std::runtime_error("Did not receive foe team size");
		}
		if (!m_foe_starter) {
			throw std::runtime_error("Did not receive foe's starting species");
		}
		if (*m_type != "singles") {
			throw std::runtime_error(containers::concatenate<std::string>("Unsupported format "sv, *m_type));
		}
		auto make_foe_team = [&] {
			auto team = SeenTeam<generation>(*m_foe_team_size);
			auto & pokemon = *m_foe_starter;
			team.add_pokemon({
				pokemon.species,
				std::move(pokemon).nickname,
				pokemon.level,
				pokemon.gender
			});
			return team;
		};

		return BattleParser(
			std::move(m_analysis_logger),
			m_usage_stats,
			GenerationGeneric<ClientBattleInputs>(ClientBattleInputs<generation>{
				Teams<generation>{*m_team, make_foe_team()},
				m_evaluate
			}),
			*m_party,
			m_depth
		);
	}

private:
	auto completed() const -> bool {
		// TODO: Handle NvN battles
		return m_ai_switched_in and m_foe_starter;
	}

	containers::string m_username;
	Evaluate<generation> m_evaluate;
	UsageStats const & m_usage_stats;
	Depth m_depth;
	AnalysisLogger m_analysis_logger;
	tv::optional<KnownTeam<generation>> m_team;
	tv::optional<Party> m_party;
	tv::optional<containers::string> m_type; // singles, doubles, triples
	tv::optional<containers::string> m_tier;
	tv::optional<TeamSize> m_foe_team_size;
	tv::optional<ParsedSwitch> m_foe_starter;
	bool m_ai_switched_in = false;
};

auto make_battle_factory(
	Generation const runtime_generation,
	containers::string username,
	AllEvaluate evaluate,
	UsageStats const & usage_stats,
	Depth depth,
	AnalysisLogger analysis_logger
) -> std::unique_ptr<BattleFactory> {
	auto make = [&]<Generation generation>(constant_gen_t<generation>) -> std::unique_ptr<BattleFactory> {
		return std::make_unique<BattleFactoryImpl<generation>>(
			std::move(username),
			evaluate.get<generation>(),
			usage_stats,
			depth,
			std::move(analysis_logger)
		);
	};
	return constant_generation(runtime_generation, make);
}

} // namespace technicalmachine::ps
