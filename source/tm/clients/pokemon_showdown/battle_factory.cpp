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
#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/parse_team.hpp>

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/generic_stats.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>

#include <algorithm>
#include <stdexcept>
#include <string>

namespace technicalmachine {
namespace ps {

namespace {

void validate_generation(std::string_view const received, Generation const expected) {
	auto const parsed = static_cast<Generation>(bounded::to_integer<1, 8>(received));
	if (parsed != expected) {
		throw std::runtime_error("Received wrong generation. Expected " + std::to_string(static_cast<int>(expected)) + "but got " + std::string(received));
	}
}

}	// namespace

void BattleFactory::handle_message(InMessage message) {
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
		validate_generation(message.pop(), m_generation);
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
			m_team = parse_team(m_parse_json(json_data), m_generation);
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

BattleParser BattleFactory::make(AllUsageStats const & all_usage_stats, BattleParser::SendMessageFunction send_message) && {
	BOUNDED_ASSERT(completed());
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
		auto team = Team(*m_foe_team_size, false);
		auto const pokemon = *m_foe_starter;
		team.add_pokemon(m_generation, pokemon.species, pokemon.level, pokemon.gender);
		return team;
	};
	return BattleParser(
		std::move(send_message),
		std::move(m_battle_logger),
		std::ofstream(m_log_directory / "analysis.txt"),
		std::move(m_id),
		std::move(m_username),
		m_generation,
		all_usage_stats[m_generation],
		m_evaluate,
		*m_party,
		m_depth,
		m_random_engine,
		std::move(m_team),
		make_foe_team(),
		m_log_foe_teams
	);
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

}	// namespace ps
}	// namespace technicalmachine
