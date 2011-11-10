// Pokemon Lab outgoing messages
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
#include "../ability.h"
#include "../pokemon.h"
#include "../team.h"

namespace technicalmachine {
namespace pl {

OutMessage::OutMessage (uint8_t code) : network::OutMessage::OutMessage (code) {
}

void OutMessage::write_string (std::string const & str) {
	write_short (str.length());
	for (char const c : str)
		write_byte (c);
}

void OutMessage::write_team (Team const & team) {
	write_int (team.size);
	for (Pokemon const & pokemon : team.pokemon.set) {
		write_int (species_to_id (pokemon.name));
		write_string (pokemon.nickname);
		constexpr bool shiny = false;
		write_byte (shiny);

		int8_t const gender = pokemon.gender.to_simulator_int ();
		write_byte (gender);

		write_byte (pokemon.happiness);
		write_int (pokemon.level);
		write_string (pokemon.item.get_name ());
		write_string (pokemon.ability.get_name ());
		write_int (pokemon.nature.name);
		unsigned number_of_moves = 0;
		while (pokemon.move.set [number_of_moves].name != Move::STRUGGLE)
			++number_of_moves;
		write_int (number_of_moves);
		for (std::vector<Move>::const_iterator move = pokemon.move.set.begin(); move->name != Move::STRUGGLE; ++move) {
			write_int (move_to_id (move->name));
			write_int (3);		// Replace this with real PP-ups logic later
		}
		write_int (pokemon.hp.iv);
		write_int (pokemon.hp.ev * 4);
		write_int (pokemon.atk.iv);
		write_int (pokemon.atk.ev * 4);
		write_int (pokemon.def.iv);
		write_int (pokemon.def.ev * 4);
		write_int (pokemon.spe.iv);
		write_int (pokemon.spe.ev * 4);
		write_int (pokemon.spa.iv);
		write_int (pokemon.spa.ev * 4);
		write_int (pokemon.spd.iv);
		write_int (pokemon.spd.ev * 4);
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
		for (uint8_t clause : clauses)
			write_byte (clause);
		write_byte (timing);
		if (timing) {
			write_int (pool);
			write_byte (periods);
			write_int (period_length);
		}
	}
}

void OutMessage::send (boost::asio::ip::tcp::socket & socket) {
	uint32_t length = htonl (buffer.size() - 1);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.insert (buffer.begin() + n + 1, *(byte + n));
	boost::asio::write (socket, boost::asio::buffer (buffer));
}

} // namespace pl
} // namespace technicalmachine
