// Handles challenges / current battles
// Copyright (C) 2013 David Stone
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

// What follows are generic functions and data for connection to any Pokemon
// sim. Specific functions and data are found in the respective sim's folder.

#include "battles.hpp"

#include <cassert>

#include "battle.hpp"
#include "no_pending_challenges.hpp"

namespace technicalmachine {

void Battles::handle_challenge_withdrawn() {
	if (challenges_are_queued()) {
		assert(challenges.size() == 1);
		challenges.erase(challenges.begin());
	}
}

void Battles::handle_challenge_withdrawn (std::string const & opponent) {
	challenges.erase(opponent);
}

Battle & Battles::handle_begin(uint32_t const battle_id, std::string const & opponent) {
	auto const it = challenges.find(opponent);
	if (it == challenges.end()) {
		throw NoPendingChallenges(opponent);
	}
	Battle & battle = *it->second;
	active.emplace(battle_id, std::move(it->second));
	challenges.erase(opponent);
	return battle;
}

void Battles::handle_end(uint32_t const battle_id) {
	active.erase(battle_id);
}

Battle const & Battles::find(uint32_t const battle_id) const {
	auto const it = active.find(battle_id);
	if (it == active.end())
		throw NoPendingChallenges(battle_id);
	return *it->second;
}

Battle & Battles::find(uint32_t const battle_id) {
	auto const it = active.find(battle_id);
	if (it == active.end())
		throw NoPendingChallenges(battle_id);
	return *it->second;
}

bool Battles::challenges_are_queued() const {
	return !challenges.empty();
}

std::string const & Battles::first_challenger() const {
	if (!challenges_are_queued())
		throw NoPendingChallenges();
	return challenges.begin()->first;
}

}	// namespace technicalmachine
