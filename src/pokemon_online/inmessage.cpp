// Pokemon Online incoming messages
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

#include "inmessage.hpp"

#include <cstdint>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>

#include "connect.hpp"
#include "../network/invalid_packet.hpp"

namespace technicalmachine {
namespace po {

InMessage::InMessage (): network::InMessage::InMessage () {
}

std::string InMessage::read_string () {
	std::string data = "";
	uint32_t const bytes = read_int ();
	// QString reports a size of 0xFFFFFFFF if the string is null. I'll just call it empty.
	if (bytes != 0xFFFFFFFF) {
		uint32_t const number_of_utf16_characters = bytes / 2;
		for (uint32_t n = 0; n != number_of_utf16_characters; ++n)
			data += static_cast <char> (read_short ());
	}
	return data;
}

void InMessage::read_header (boost::asio::ip::tcp::socket & socket, Client * client) {
	reset (3);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& InMessage::read_body, this, boost::ref (socket), client));
}

void InMessage::read_body (boost::asio::ip::tcp::socket & socket, Client * client) {
	// extract the message type and length components
	uint16_t const bytes = read_short ();
	// Don't do an invalid call to new if the server says the message has a length of 0
	if (bytes > 0) {
		Message code = static_cast <Message> (read_byte ());
		reset (bytes - 1u);
		boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& Client::handle_message, client, code, boost::ref (*this)));
	}
	else {
		throw technicalmachine::network::InvalidPacket ("Server sent message of length 0.");
	}
}

#if 0
void OutMessage::read_team (Team const & team) {
	std::string const username = read_string ();
	std::string const info = read_string ();
	std::string const lose_message = read_string ();
	std::string const win_message = read_string ();
	uint16_t const avatar = read_short ();
	std::string const tier = read_string ();
	uint8_t const generation = read_byte ();
	for (Pokemon const & pokemon : team.pokemon.set) {
		uint16_t const species = species_to_id (pokemon.name);
		read_short (species);
		uint8_t const forme = 0;
		read_byte (forme);
		read_string (pokemon.nickname);
		uint16_t const item = item_to_id (pokemon.item.name);
		read_short (item);
		uint16_t const ability = ability_to_id (pokemon.ability.name);
		read_short (ability);
		uint8_t const nature = nature_to_id (pokemon.nature.name);
		read_byte (nature);
		uint8_t const gender = pokemon.gender.to_simulator_int ();
		read_byte (gender);
		bool shiny = false;
		read_byte (shiny);
		read_byte (pokemon.happiness);
		read_byte (pokemon.level);
		unsigned number_of_moves = 0;
		for (std::vector<Move>::const_iterator move = pokemon.move.set.begin(); move->name != Move::STRUGGLE; ++move) {
			++number_of_moves;
			uint32_t const move_id = move_to_id (move->name);
			read_int (move_id);
		}
		while (number_of_moves < 4) {
			read_int (0);
			++number_of_moves;
		}
		read_byte (pokemon.hp.iv);
		read_byte (pokemon.atk.iv);
		read_byte (pokemon.def.iv);
		read_byte (pokemon.spe.iv);
		read_byte (pokemon.spa.iv);
		read_byte (pokemon.spd.iv);

		read_byte (pokemon.hp.ev * 4);
		read_byte (pokemon.atk.ev * 4);
		read_byte (pokemon.def.ev * 4);
		read_byte (pokemon.spe.ev * 4);
		read_byte (pokemon.spa.ev * 4);
		read_byte (pokemon.spd.ev * 4);
	}
	for (unsigned n = team.pokemon.set.size (); n <= 6; ++n) {
		read_short (0);
	}
}
#endif

}	// namespace po
}	// namespace technicalmachine
