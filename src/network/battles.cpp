// Handles challenges / current battles
// Copyright (C) 2012 David Stone
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

#include "invalid_user.hpp"

#include "../battle.hpp"

namespace technicalmachine {
namespace network {

void Battles::add_pending_challenge(GenericBattle * battle) {
	challenges.insert(std::make_pair(battle->opponent, Pointer(battle)));
}

void Battles::handle_challenge_withdrawn() {
	challenges.erase(challenges.begin());
}

void Battles::handle_challenge_withdrawn (std::string const & opponent) {
	challenges.erase(opponent);
}

GenericBattle & Battles::handle_begin(uint32_t const battle_id, std::string const & opponent) {
	auto const it = challenges.find(opponent);
	if (it == challenges.end())
		throw InvalidUser(opponent);
	GenericBattle & battle = *it->second;
	active.insert(std::make_pair(battle_id, std::move(it->second)));
	challenges.erase(opponent);
	return battle;
}

void Battles::handle_end(uint32_t const battle_id) {
	active.erase(battle_id);
}

GenericBattle const & Battles::find(uint32_t const battle_id) const {
	auto const it = active.find(battle_id);
	if (it == active.end())
		throw InvalidUser(battle_id);
	return *it->second;
}

GenericBattle & Battles::find(uint32_t const battle_id) {
	auto const it = active.find(battle_id);
	if (it == active.end())
		throw InvalidUser(battle_id);
	return *it->second;
}

bool Battles::challenges_are_queued() const {
	return !challenges.empty();
}

std::string Battles::first_challenger() const {
	if (!challenges_are_queued())
		throw InvalidUser();
	return challenges.begin()->first;
}

}	// namespace network
}	// namespace technicalmachine
