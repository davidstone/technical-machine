// Pokemon Online outgoing messages
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

#include "outmessage.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#include "battle_settings.hpp"
#include "conversion.hpp"

#include "../../team.hpp"

#include "../../pokemon/pokemon.hpp"

#include <endian/endian.hpp>

namespace technicalmachine {
namespace po {

OutMessage::OutMessage (uint8_t code):
	network::OutMessage::OutMessage (code) {
}

void OutMessage::write_string (std::string const & str) {
	uint32_t const number_of_utf16_bytes = 2 * str.length();
	write_int (number_of_utf16_bytes);
	for (char const c : str)
		write_short (static_cast<uint16_t> (c));
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
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon (pokemon);
	}
	for (unsigned n = team.all_pokemon().size(); n <= 6; ++n) {
		write_short (0);
	}
}

void OutMessage::write_pokemon (Pokemon const & pokemon) {
	std::pair <uint16_t, uint8_t> const species = species_to_id(pokemon);
	write_short (species.first);
	write_byte (species.second);
	write_string (pokemon.get_nickname());
	uint16_t const item = item_to_id (get_item(pokemon).name);
	write_short (item);
	uint16_t const ability = ability_to_id (get_ability(pokemon).name());
	write_short (ability);
	uint8_t const nature = nature_to_id (get_nature(pokemon).name);
	write_byte (nature);
	uint8_t const gender = gender_to_id (get_gender(pokemon).gender);
	write_byte (gender);
	bool shiny = false;
	write_byte (shiny);
	write_byte(get_happiness(pokemon)());
	write_byte(get_level(pokemon)());
	unsigned number_of_moves = 0;
	pokemon.move.for_each_regular_move([&](Move const & move) {
		++number_of_moves;
		uint32_t const move_id = move_to_id(move);
		write_int (move_id);
	});
	while (number_of_moves < 4) {
		write_int (0);
		++number_of_moves;
	}
	static constexpr auto stats = {
		StatNames::ATK, StatNames::DEF, StatNames::SPE, StatNames::SPA, StatNames::SPD
	};
	write_byte(get_hp(pokemon).iv.value());
	for (auto const stat : stats) {
		write_byte(get_stat(pokemon, stat).iv.value());
	}
	write_byte(get_hp(pokemon).ev.value());
	for (auto const stat : stats) {
		write_byte(get_stat(pokemon, stat).ev.value());
	}
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

void OutMessage::write_challenge (uint32_t user_id, uint8_t generation, BattleSettings const & settings) {
	// What is this for?
	constexpr uint8_t send_var = 0;
	write_byte (send_var);
	write_int (user_id);
	write_int (settings.clauses);
	write_byte (settings.mode);
}

void OutMessage::write_color () {
	int8_t const spec = 0;
	uint16_t const alpha = 0;
	uint16_t const red = 0;
	uint16_t const green = 0;
	uint16_t const blue = 0;
	uint16_t const pad = 0;
	write_byte (spec);
	write_short (alpha);
	write_short (red);
	write_short (green);
	write_short (blue);
	write_short (pad);
}

void OutMessage::finalize() {
	typedef uint16_t length_type;
	length_type length = boost::endian::h_to_n(static_cast<length_type>(buffer.size()));
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (int n = 0; n != sizeof (length_type); ++n)
		buffer.insert (buffer.begin() + n, *(byte + n));
}

void OutMessage::reset_action_code () {
	// I store my chosen action to be sent to the server because the server
	// sends me one message for move legalities and then later sends me a
	// message saying that it's ready to receive my action decision. This means
	// that I have to be able to reset this action decision message to a "ready
	// to be filled with the action decision" state.
	buffer.resize (1);
	buffer [0] = BATTLE_MESSAGE;
}

} // namespace po
} // namespace technicalmachine
