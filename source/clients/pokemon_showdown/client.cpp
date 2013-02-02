// Connect to Pokemon Showdown
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

#include "client.hpp"

#include <cstdint>
#include <string>

#include "battle.hpp"
#include "inmessage.hpp"
#include "outmessage.hpp"

#include "../../team.hpp"

namespace technicalmachine {
namespace ps {

Client::Client(unsigned set_depth):
	network::GenericClient(set_depth) {
	log_in();
}

void Client::log_in() {
}

void Client::run() {
	run_main_service<InMessage>();
}

void Client::send_keep_alive_message() {
}

void Client::handle_message(InMessage::Message code, InMessage & msg) {
	switch (code) {
	}
	read_header(msg);
}

void Client::send_battle_challenge(std::string const & opponent) {
}

void Client::handle_finalize_challenge(std::string const & opponent, bool accepted, bool unused) {
}

void Client::join_channel(std::string const & channel) {
}

void Client::part_channel(std::string const & channel) {
}

void Client::send_channel_message(std::string const & channel, std::string const & message) {
}

void Client::send_channel_message(uint32_t channel_id, std::string const & message) {
}

void Client::send_private_message(std::string const & user, std::string const & message) {
}

}	// namespace ps
}	// namespace technicalmachine
