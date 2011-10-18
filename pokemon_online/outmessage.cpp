// Pokemon Online outgoing messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "outmessage.h"
#include <cstdint>
#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace technicalmachine {
namespace po {

OutMessage::OutMessage (uint8_t code) : network::OutMessage::OutMessage (code) {
}

void OutMessage::write_string (std::string const & string) {
	uint32_t const number_of_utf16_bytes = 2 * string.length();
	write_int (number_of_utf16_bytes);
	for (std::string::const_iterator it = string.begin(); it != string.end(); ++it)
		write_short (*it);
}

/*void OutMessage::write_team (Team const & team) {
	write_string (username);
	std::string const info = "";
	write_string (info);
	std::string const lose_message = "";
	write_string (lose_message);
	std::string const win_message = "";
	write_string (win_message);
	uint16_t const avatar = 117;		// Mad scientist
	write_short (avatar);
	std::string const default_tier = "";
	write_string (default_tier);
	uint8_t const generation = 4;
	write_byte (generation);
	for (std::vector <Pokemon>::const_iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		uint16_t species = 0;
		write_short (species);
		uint16_t forme = 0;
		write_short (forme);
		write_string (pokemon->nickname);
		uint16_t item = 0;
		write_short (item);
		uint16_t ability = 0;
		write_short (ability);
		uint8_t nature = 0;
		write_byte (nature);
		uint8_t gender = 0;
		write_byte (gender);
		bool shiny = false;
		write_byte (shiny);
		write_byte (it->happiness);
		write_byte (level);
		unsigned number_of_moves = 0;
		while (pokemon->move.set [number_of_moves].name != Move::STRUGGLE)
			++number_of_moves;
		write_int (number_of_moves);
		for (std::vector<Move>::const_iterator move = pokemon->move.set.begin(); move->name != Move::STRUGGLE; ++move) {
			int name = move->name;
			if (name >= Move::SWITCH0)
				name -= 6;
			write_int (name);
			write_int (3);		// Replace this with real PP-ups logic later
		}
		write_int (pokemon->hp.iv);
		write_int (pokemon->hp.ev * 4);
		write_int (pokemon->atk.iv);
		write_int (pokemon->atk.ev * 4);
		write_int (pokemon->def.iv);
		write_int (pokemon->def.ev * 4);
		write_int (pokemon->spe.iv);
		write_int (pokemon->spe.ev * 4);
		write_int (pokemon->spa.iv);
		write_int (pokemon->spa.ev * 4);
		write_int (pokemon->spd.iv);
		write_int (pokemon->spd.ev * 4);
	}
	for (unsigned n = team.pokemon.set.size (); n <= 6; ++n) {
		// add empty Pokemon?
	}
}

void OutMessage::write_move (uint32_t field_id, uint8_t move_index, uint8_t target) {
	write_int (field_id);
	write_byte (0);
	write_byte (move_index);
	write_byte (target);
}

void OutMessage::write_switch (uint32_t field_id, uint8_t slot) {
	write_int (field_id);
	write_byte (1);
	write_byte (slot);
	write_byte (0);
}

void OutMessage::write_challenge (std::string const & opponent, uint8_t generation, uint32_t party_size, uint32_t team_length, uint32_t metagame, std::vector <uint8_t> const & clauses, bool timing, uint32_t pool, uint8_t periods, uint32_t period_length) {
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
