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

import tm.clients.pl.write_team_file;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_parser;
import tm.clients.ps.chat;
import tm.clients.ps.inmessage;
import tm.clients.ps.make_party;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.parse_switch;
import tm.clients.ps.parse_team;
import tm.clients.ps.validate_generation;

import tm.clients.make_battle_manager_inputs;
import tm.clients.party;
import tm.clients.teams;
import tm.clients.write_team;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;

import tm.pokemon.max_pokemon_per_team;

import tm.team_predictor.all_usage_stats;

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
		std::filesystem::path const & base_log_directory,
		tv::optional<WriteTeam> write_team,
		containers::string id_,
		containers::string username,
		Evaluate<generation> evaluate,
		Depth depth,
		std::mt19937 random_engine
	):
		m_id(std::move(id_)),
		m_log_directory(base_log_directory / std::string_view(m_id)),
		m_write_team(std::move(write_team)),
		m_username(std::move(username)),
		m_evaluate(evaluate),
		m_depth(depth),
		m_random_engine(random_engine)
	{
	}

	auto id() const -> std::string_view final {
		return m_id;
	}

	auto handle_message(InMessage message) -> tv::optional<containers::string> final {
		if (handle_chat_message(message)) {
			return tv::none;
		}

		// Documented at
		// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
		// under the section "Battle Initialization"
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
		return tv::none;
	}

	auto completed() const -> BattleInterface::Complete final {
		// TODO: Handle NvN battles
		return m_ai_switched_in and m_foe_starter ? BattleInterface::Complete::start : BattleInterface::Complete::none;
	}
	auto make(AllUsageStats const & usage_stats) && -> BattleParser final {
		BOUNDED_ASSERT(completed() == BattleInterface::Complete::start);
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

		pl::write_team(*m_team, m_log_directory / "team.sbt");

		return BattleParser(
			AnalysisLogger(m_log_directory),
			m_write_team,
			std::move(m_id),
			std::move(m_username),
			usage_stats[generation],
			GenerationGeneric<BattleManagerInputs>(BattleManagerInputs<generation>{
				Teams<generation>{*m_team, make_foe_team()},
				m_evaluate
			}),
			*m_party,
			m_depth,
			m_random_engine
		);
	}

private:
	containers::string m_id;
	std::filesystem::path m_log_directory;
	tv::optional<WriteTeam> m_write_team;
	containers::string m_username;
	Evaluate<generation> m_evaluate;
	Depth m_depth;
	std::mt19937 m_random_engine;
	tv::optional<KnownTeam<generation>> m_team;
	tv::optional<Party> m_party;
	tv::optional<containers::string> m_type; // singles, doubles, triples
	tv::optional<containers::string> m_tier;
	tv::optional<TeamSize> m_foe_team_size;
	tv::optional<ParsedSwitch> m_foe_starter;
	bool m_ai_switched_in = false;
};

auto make_battle_factory(
	std::filesystem::path const & base_log_directory,
	tv::optional<WriteTeam> write_team,
	containers::string id,
	containers::string username,
	AllEvaluate evaluate,
	Depth depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleFactory> {
	auto const parsed_generation = parse_generation_from_format(id, "battle-gen");
	auto make = [&]<Generation generation>(constant_gen_t<generation>) -> std::unique_ptr<BattleFactory> {
		return std::make_unique<BattleFactoryImpl<generation>>(
			base_log_directory,
			std::move(write_team),
			std::move(id),
			std::move(username),
			evaluate.get<generation>(),
			depth,
			random_engine
		);
	};
	return constant_generation(parsed_generation, make);
}

} // namespace technicalmachine::ps
