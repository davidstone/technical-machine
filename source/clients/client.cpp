// Connect to an arbitrary Pokemon sim
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

// What follows are generic functions and data for connection to any Pokemon
// sim. Specific functions and data are found in the respective sim's folder.

#include "client.hpp"

#include <iostream>

#include "battle.hpp"
#include "battle_settings.hpp"
#include "random_string.hpp"

#include "../settings_file.hpp"

namespace technicalmachine {

Client::Client(unsigned depth):
	random_engine(rd()),
	m_depth(depth) {
	load_settings(false);
}

Client::~Client() {
}

void Client::set_depth(unsigned const new_depth) {
	m_depth = new_depth;
}

Settings Client::load_settings(bool const reload) {
	if (reload) {
		m_evaluation_constants = Evaluate{};
	}
	Settings const settings;
	team_file = settings.team_file;
	return settings;
}

DetailedStats const & Client::detailed() const {
	return detailed_stats;
}

Evaluate const & Client::evaluation_constants() const {
	return m_evaluation_constants;
}

Team Client::generate_team() {
	std::cerr << team_file << '\n';
	return load_team_from_file(random_engine, team_file);
}

void Client::handle_battle_begin(uint32_t battle_id, std::string const & opponent, Party) {
	// TODO: Party?
	battles.handle_begin(battle_id, opponent);
}

void Client::handle_battle_end(uint32_t const battle_id, Result const result) {
	auto const & battle = battles.find(battle_id);
	battle.handle_end(*this, result);
	battles.handle_end(battle_id);
}

bool Client::challenges_are_queued() const {
	return battles.challenges_are_queued();
}

std::string const & Client::first_challenger() const {
	return battles.first_challenger();
}

std::string Client::random_string(size_t const size) {
	return ::technicalmachine::random_string(random_engine, size);
}

void Client::handle_incoming_challenge(std::string const & opponent, BattleSettings const & settings) {
	constexpr bool challenger = false;
	handle_finalize_challenge(opponent, settings.are_acceptable(), challenger);
}

}	// namespace technicalmachine
