// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/clients/pokemon_showdown/battle_factory.hpp>

#include <tm/clients/pokemon_showdown/battle_logger.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/constant_generation.hpp>
#include <tm/clients/pokemon_showdown/json_parser.hpp>
#include <tm/clients/pokemon_showdown/parse_team.hpp>

#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/to_integer.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace technicalmachine {
namespace ps {

BattleFactory::~BattleFactory() = default;

namespace {

void validate_generation(std::string_view const received, Generation const expected) {
	auto const parsed = static_cast<Generation>(bounded::to_integer<1, 8>(received));
	if (parsed != expected) {
		throw std::runtime_error("Received wrong generation. Expected " + std::to_string(static_cast<int>(expected)) + "but got " + std::string(received));
	}
}

auto parse_generation(std::string_view const id) -> Generation {
	// TODO: This won't work for generation 10
	constexpr auto generation_index = std::char_traits<char>::length("battle-gen");
	if (id.size() < generation_index) {
		throw std::runtime_error("Invalid battle id. Expected something in the format of: \"battle-gen[generation_number]\", but got " + std::string(id));
	}
	auto const generation_char = id[generation_index];
	auto const generation = generation_char - '0';
	if (generation < 1 or 8 < generation) {
		throw std::runtime_error("Invalid generation. Expected a value between 1 and 8, but got " + std::string(1, generation_char));
	}
	return static_cast<Generation>(generation);
}

template<Generation generation>
struct BattleFactoryImpl : BattleFactory {
	BattleFactoryImpl(
		std::filesystem::path const & base_log_directory,
		bool const log_foe_teams,
		std::string id_,
		std::string username,
		AllEvaluate evaluate,
		DepthValues depth,
		std::mt19937 random_engine
	):
		m_id(std::move(id_)),
		m_log_directory(base_log_directory / m_id),
		m_battle_logger(m_log_directory, m_id),
		m_username(std::move(username)),
		m_evaluate(evaluate),
		m_depth(depth),
		m_random_engine(random_engine),
		m_log_foe_teams(log_foe_teams)
	{
	}
	
	std::string_view id() const final {
		return m_id;
	}
	
	void handle_message(InMessage message) final {
		m_battle_logger.log(message);
		
		if (handle_chat_message(message)) {
			return;
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
				bounded::insert(m_team, parse_team<generation>(m_parse_json(json_data)));
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
			auto const parsed = parse_switch(message);
			if (*m_party == parsed.party) {
				if (m_ai_switched_in) {
					throw std::runtime_error("AI switched in twice");
				}
				m_ai_switched_in = true;
			} else {
				if (m_foe_starter) {
					throw std::runtime_error("Foe switched in twice");
				}
				insert(m_foe_starter, parsed);
			}
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
	}
	
	bool completed() const final {
		// TODO: Handle NvN battles
		return m_ai_switched_in and m_foe_starter;
	}
	auto make(AllUsageStats const & usage_stats, SendMessageFunction send_message) && -> std::unique_ptr<BattleParser> final {
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
			throw std::runtime_error("Unsupported format " + *m_type);
		}
		auto make_foe_team = [&]{
			auto team = Team<generation>(*m_foe_team_size, false);
			auto const pokemon = *m_foe_starter;
			team.add_pokemon(Pokemon<generation>(pokemon.species, pokemon.level, pokemon.gender));
			return team;
		};

		pl::write_team(*m_team, m_log_directory / "team.sbt");

		return make_battle_parser(
			std::move(send_message),
			std::move(m_battle_logger),
			std::ofstream(m_log_directory / "analysis.txt"),
			std::move(m_id),
			std::move(m_username),
			usage_stats,
			m_evaluate,
			*m_party,
			m_depth,
			m_random_engine,
			GenericTeams(Teams<generation>{*m_team, make_foe_team()}),
			m_log_foe_teams
		);
	}

private:
	JSONParser m_parse_json;

	std::string m_id;
	std::filesystem::path m_log_directory;
	BattleLogger m_battle_logger;
	std::string m_username;
	AllEvaluate m_evaluate;
	DepthValues m_depth;
	std::mt19937 m_random_engine;
	bounded::optional<Team<generation>> m_team;
	bounded::optional<Party> m_party;
	bounded::optional<std::string> m_type;	// singles, doubles, triples
	bounded::optional<std::string> m_tier;
	bounded::optional<TeamSize> m_foe_team_size;
	bounded::optional<ParsedSwitch> m_foe_starter;
	bool m_ai_switched_in = false;
	bool m_log_foe_teams;
};

} // namespace

auto make_battle_factory(
	std::filesystem::path const & base_log_directory,
	bool const log_foe_teams,
	std::string id,
	std::string username,
	AllEvaluate evaluate,
	DepthValues depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleFactory> {
	auto const parsed_generation = parse_generation(id);
	auto make = [&]<Generation generation>(std::integral_constant<Generation, generation>) -> std::unique_ptr<BattleFactory> {
		return std::make_unique<BattleFactoryImpl<generation>>(
			base_log_directory,
			log_foe_teams,
			std::move(id),
			std::move(username),
			evaluate,
			depth,
			random_engine
		);
	};
	return constant_generation(parsed_generation, make);
}

} // namespace ps
} // namespace technicalmachine
