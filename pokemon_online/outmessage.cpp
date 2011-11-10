// Pokemon Online outgoing messages
// Copyright (C) 2011 David Stone
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

#include "outmessage.h"
#include <cstdint>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "conversion.h"
#include "../team.h"
#include "../pokemon.h"

namespace technicalmachine {
namespace po {

OutMessage::OutMessage (uint8_t code):
	network::OutMessage::OutMessage (code) {
}

void OutMessage::write_string (std::string const & str) {
	uint32_t const number_of_utf16_bytes = 2 * str.length();
	write_int (number_of_utf16_bytes);
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
		write_short (*it);
}

void OutMessage::write_team (Team const & team, std::string const & username) {
	write_string (username);
	std::string const info = "http://doublewise.net/pokemon/";
	write_string (info);
	std::string const lose_message = "http://doublewise.net/pokemon/";
	write_string (lose_message);
	std::string const win_message = "http://doublewise.net/pokemon/";
	write_string (win_message);
	uint16_t const avatar = 117;	 // Mad scientist!
	write_short (avatar);
	std::string const tier = "All";
	write_string (tier);
	uint8_t const generation = 4;
	write_byte (generation);
	for (Pokemon const & pokemon : team.pokemon.set) {
		uint16_t const species = species_to_id (pokemon.name);
		write_short (species);
		uint8_t const forme = 0;
		write_byte (forme);
		write_string (pokemon.nickname);
		uint16_t const item = item_to_id (pokemon.item.name);
		write_short (item);
		uint16_t const ability = ability_to_id (pokemon.ability.name);
		write_short (ability);
		uint8_t const nature = nature_to_id (pokemon.nature.name);
		write_byte (nature);
		uint8_t const gender = pokemon.gender.to_simulator_int ();
		write_byte (gender);
		bool shiny = false;
		write_byte (shiny);
		write_byte (pokemon.happiness);
		write_byte (pokemon.level);
		unsigned number_of_moves = 0;
		for (std::vector<Move>::const_iterator move = pokemon.move.set.begin(); move->name != Move::STRUGGLE; ++move) {
			++number_of_moves;
			uint32_t const move_id = move_to_id (move->name);
			write_int (move_id);
		}
		while (number_of_moves < 4) {
			write_int (0);
			++number_of_moves;
		}
		write_byte (pokemon.hp.iv);
		write_byte (pokemon.atk.iv);
		write_byte (pokemon.def.iv);
		write_byte (pokemon.spe.iv);
		write_byte (pokemon.spa.iv);
		write_byte (pokemon.spd.iv);

		write_byte (pokemon.hp.ev * 4);
		write_byte (pokemon.atk.ev * 4);
		write_byte (pokemon.def.ev * 4);
		write_byte (pokemon.spe.ev * 4);
		write_byte (pokemon.spa.ev * 4);
		write_byte (pokemon.spd.ev * 4);
	}
	for (unsigned n = team.pokemon.set.size (); n <= 6; ++n) {
		write_short (0);
	}
	bool const ladder_enabled = true;
	write_byte (ladder_enabled);
	bool const show_team = true;
	write_byte (show_team);
}

enum Choice {
	CANCEL = 0,
	ATTACK = 1,
	SWITCH = 2,
	REARRANGE = 3,
	CENTER_MOVE = 4,
	DRAW = 5
};

void OutMessage::write_move (uint32_t battle_id, uint8_t move_index, uint8_t target) {
	write_int (battle_id);
	write_byte (1);
	write_byte (Choice::ATTACK);
	write_byte (move_index);
	write_byte (target);
}

void OutMessage::write_switch (uint32_t battle_id, uint8_t slot) {
	write_int (battle_id);
	write_byte (1);
	write_byte (Choice::SWITCH);
	write_byte (slot);
}

/*void OutMessage::write_challenge (std::string const & opponent, uint8_t generation, uint32_t party_size, uint32_t team_length, uint32_t metagame, std::vector <uint8_t> const & clauses, bool timing, uint32_t pool, uint8_t periods, uint32_t period_length) {
	write_string (opponent);
	write_byte (generation);
	write_int (party_size);
	write_int (team_length);
	write_int (metagame);
	if (metagame == -1u) {
		write_byte (clauses.size());
		for (std::vector <uint8_t>::const_iterator it = clauses.begin(); it != clauses.end(); ++it)
			write_byte (*it);
		write_byte (timing);
		if (timing) {
			write_int (pool);
			write_byte (periods);
			write_int (period_length);
		}
	}
}
*/
void OutMessage::send (boost::asio::ip::tcp::socket & socket) {
	uint16_t length = htons (buffer.size ());
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (unsigned n = 0; n != sizeof (uint16_t); ++n)
		buffer.insert (buffer.begin() + n, *(byte + n));
	boost::asio::write (socket, boost::asio::buffer (buffer));
}

}
}
