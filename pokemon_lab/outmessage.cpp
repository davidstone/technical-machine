// Pokemon Lab outgoing messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>
#include <vector>
#include "../pokemon.h"
#include "../team.h"
#include <boost/asio.hpp>
#include "outmessage.h"
#undef SING

namespace technicalmachine {
namespace pl {

OutMessage::OutMessage (uint8_t code) {
	buffer.push_back (code);
}

void OutMessage::write_byte (uint8_t byte) {
	buffer.push_back (byte);
}

void OutMessage::write_short (uint16_t bytes) {
	uint16_t network_byte = htons (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint16_t); ++n)
		buffer.push_back (*(byte + n));
}

void OutMessage::write_int (uint32_t bytes) {
	uint32_t network_byte = htonl (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.push_back (*(byte + n));
}

void OutMessage::write_string (std::string const & string) {
	write_short (string.length());
	for (std::string::const_iterator it = string.begin(); it != string.end(); ++it)
		buffer.push_back (*it);
}

void OutMessage::write_team (Team const & team) {
	write_int (team.size);
	for (std::vector <Pokemon>::const_iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		write_int (tm_to_pl_species (pokemon->name));
		write_string (pokemon->nickname);
		write_byte (0);		// Don't care about shininess

		uint8_t gender;
		if (pokemon->gender == MALE)
			gender = 1;
		else if (pokemon->gender == FEMALE)
			gender = 2;
		else
			gender = 0;
		write_byte (gender);

		write_byte (pokemon->happiness);
		write_int (pokemon->level);
		write_string (item_name [pokemon->item]);
		write_string (ability_name [pokemon->ability]);
		write_int (pokemon->nature);
		unsigned number_of_moves = 0;
		while (pokemon->move.set [number_of_moves].name != STRUGGLE)
			++number_of_moves;
		write_int (number_of_moves);
		for (std::vector<Move>::const_iterator move = pokemon->move.set.begin(); move->name != STRUGGLE; ++move) {
			int name = move->name;
			if (name >= SWITCH0)
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

void OutMessage::send (boost::asio::ip::tcp::socket & socket) {
	uint32_t length = htonl (buffer.size() - 1);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.insert (buffer.begin() + n + 1, *(byte + n));
	boost::asio::write (socket, boost::asio::buffer (buffer));
}

int OutMessage::tm_to_pl_species (species id) {
	static int const technical_machine_to_pokemon_lab_species [] = { 464, 62, 358, 141, 305, 189, 64, 333, 428, 180, 346, 23, 58, 497, 167, 347, 303, 143, 486, 183, 297, 370, 342, 353, 338, 413, 152, 266, 14, 373, 181, 68, 402, 401, 8, 256, 241, 442, 285, 441, 440, 408, 422, 0, 431, 414, 11, 330, 331, 322, 459, 317, 267, 350, 9, 250, 112, 5, 3, 4, 445, 425, 424, 151, 392, 357, 169, 437, 365, 343, 35, 34, 172, 90, 419, 255, 340, 221, 345, 410, 341, 492, 457, 168, 158, 103, 154, 495, 300, 224, 385, 386, 387, 388, 86, 487, 49, 131, 84, 83, 231, 147, 148, 456, 146, 430, 429, 95, 50, 205, 355, 481, 354, 268, 132, 22, 124, 470, 308, 100, 238, 397, 243, 195, 101, 102, 294, 82, 21, 348, 159, 460, 179, 135, 423, 329, 204, 482, 161, 448, 479, 449, 281, 91, 427, 93, 73, 447, 202, 491, 503, 475, 361, 435, 206, 476, 43, 41, 117, 54, 75, 367, 209, 74, 87, 390, 382, 252, 57, 325, 315, 129, 444, 296, 92, 489, 213, 453, 454, 106, 105, 236, 249, 434, 162, 186, 115, 228, 227, 366, 96, 173, 313, 394, 1, 38, 384, 134, 188, 123, 139, 140, 63, 13, 114, 351, 229, 98, 280, 108, 97, 403, 404, 381, 304, 170, 130, 245, 379, 380, 474, 165, 164, 467, 107, 344, 263, 270, 432, 269, 293, 452, 271, 248, 461, 336, 369, 406, 407, 67, 66, 65, 239, 218, 128, 125, 471, 80, 81, 466, 295, 477, 494, 309, 55, 225, 462, 178, 182, 104, 258, 283, 302, 307, 306, 153, 51, 485, 375, 374, 10, 150, 149, 261, 349, 240, 443, 311, 199, 433, 145, 393, 418, 121, 257, 88, 450, 197, 176, 33, 30, 28, 31, 29, 32, 289, 37, 290, 163, 298, 321, 273, 223, 42, 137, 138, 94, 421, 488, 45, 46, 278, 52, 230, 493, 171, 17, 16, 15, 24, 220, 203, 126, 395, 310, 185, 59, 60, 61, 76, 260, 136, 478, 232, 56, 396, 480, 53, 246, 436, 194, 155, 210, 25, 242, 279, 411, 77, 19, 18, 383, 377, 490, 376, 378, 368, 222, 111, 110, 468, 451, 314, 409, 483, 499, 498, 500, 501, 502, 301, 372, 26, 27, 253, 211, 122, 116, 118, 363, 272, 85, 160, 335, 318, 496, 504, 291, 371, 89, 426, 412, 274, 405, 284, 212, 352, 265, 226, 187, 299, 455, 439, 288, 286, 79, 198, 78, 217, 234, 237, 214, 142, 360, 463, 208, 337, 20, 362, 166, 326, 446, 324, 6, 233, 400, 399, 398, 120, 119, 207, 438, 184, 244, 191, 190, 282, 332, 316, 259, 276, 219, 275, 113, 469, 127, 215, 71, 72, 472, 174, 175, 254, 323, 391, 157, 458, 327, 251, 356, 389, 156, 247, 235, 196, 200, 216, 484, 133, 48, 47, 2, 420, 328, 70, 287, 44, 312, 99, 36, 319, 320, 364, 7, 465, 12, 69, 109, 339, 292, 39, 277, 201, 193, 415, 416, 417, 264, 359, 177, 192, 473, 334, 144, 262, 40 };
	return technical_machine_to_pokemon_lab_species [id];
}

}
}
