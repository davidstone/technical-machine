// Pokemon Showdown battle logic
// Copyright (C) 2014 David Stone
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

#include "battle.hpp"

#include <charconv>
#include <iostream>

namespace technicalmachine {
namespace ps {

void BattleFactory::handle_message(InMessage message) {
	// Documented at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
	// under the section "Battle Initialization"
	if (message.type() == "clearpoke") {
		// This appears to mean nothing
	} else if (message.type() == "gametype") {
		m_type.emplace(message.next());
	} else if (message.type() == "gen") {
		auto const generation_str = message.next();
		// Why do we have no good solution for string parsing in C++?
		std::uint8_t generation = 0;
		auto const result = std::from_chars(generation_str.data(), generation_str.data() + generation_str.size(), generation);
		if (result.ec == std::error_code{}) {
			m_generation.emplace(generation);
		}
	} else if (message.type() == "player") {
		auto const player_id = message.next();
		if (message.next() == m_username) {
			m_player_id.emplace(player_id);
		}
		// message.remainder() == AVATAR
	} else if (message.type() == "poke") {
		std::cout << message.remainder() << '\n';
		// message.remainder() == PLAYER_ID|DETAILS|ITEM
	} else if (message.type() == "rated") {
		// Received if and only if the game is rated. We don't care about this
	} else if (message.type() == "rule") {
		std::cout << message.remainder() << '\n';
		// message.remainder() == RULE: DESCRIPTION
		// Received for each clause in effect
	} else if (message.type() == "start") {
		m_completed = true;
	} else if (message.type() == "teampreview") {
		// This appears to mean nothing
	} else if (message.type() == "tier") {
		m_tier.emplace(message.next());
	} else {
		std::cout << "Received battle message of unknown type: " << message.type() << ": " << message.remainder() << '\n';
	}
}

bounded::optional<Battle> BattleFactory::make() const {
	assert(m_completed);
	if (!m_player_id or !m_type or !m_tier or !m_generation) {
		return bounded::none;
	}
	if (*m_type != "singles") {
		return bounded::none;
	}
	if (*m_generation != 4) {
		return bounded::none;
	}
	return bounded::none;
}



VisibleFoeHP Battle::max_damage_precision() const {
	return 100_bi;
}

}	// namespace ps
}	// namespace technicalmachine
