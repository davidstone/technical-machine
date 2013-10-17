// Pokemon Lab outgoing messages
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

#include "outmessage.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include "conversion.hpp"
#include "id.hpp"

#include "../../ability.hpp"
#include "../../team.hpp"

#include "../../cryptography/byte_order.hpp"

#include "../../pokemon/pokemon.hpp"

#include "../../string_conversions/ability.hpp"
#include "../../string_conversions/item.hpp"
#include "../../string_conversions/pokemon.hpp"

namespace technicalmachine {
namespace pl {

OutMessage::OutMessage (uint8_t code) :
	network::OutMessage::OutMessage (code) {
}

void OutMessage::write_string (std::string const & str) {
	write_short (str.length());
	for (char const c : str)
		write_byte (static_cast<uint8_t> (c));
}

void OutMessage::write_team (Team const & team, std::string const &) {
	write_int(team.all_pokemon().size());
	for (auto const & pokemon : team.all_pokemon()) {
		write_pokemon (pokemon);
	}
}

void OutMessage::write_pokemon (Pokemon const & pokemon) {
	write_int(simulator_cast(pokemon).value());
	write_string (pokemon.get_nickname());
	constexpr bool shiny = false;
	write_byte (shiny);

	auto const gender = simulator_cast(get_gender(pokemon).gender);
	write_byte(gender.value());

	write_byte (pokemon.happiness());
	write_int(get_level(pokemon));
	write_string(to_string(get_item(pokemon).name));
	write_string(to_string(get_ability(pokemon).name()));
	write_int(get_nature(pokemon).name);
	write_int (pokemon.move.number_of_regular_moves());
	pokemon.move.for_each_regular_move([&](Move const & move) {
		write_int(simulator_cast(move).value());
		write_int (3);		// Replace this with real PP-ups logic later
	});
	static constexpr auto stats = {
		Stat::HP, Stat::ATK, Stat::DEF, Stat::SPE, Stat::SPA, Stat::SPD
	};
	for (auto const stat : stats) {
		write_int(get_stat(pokemon, stat).iv);
		write_int(get_stat(pokemon, stat).ev.value());
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
		for (uint8_t const clause : clauses)
			write_byte (clause);
		write_byte (timing);
		if (timing) {
			write_int (pool);
			write_byte (periods);
			write_int (period_length);
		}
	}
}

void OutMessage::finalize() {
	uint32_t const length = boost::endian::h_to_n(static_cast<uint32_t>(buffer.size() - 1));
	uint8_t const * byte = reinterpret_cast <uint8_t const *> (&length);
	for (int n = 0; n != sizeof (uint32_t); ++n)
		buffer.insert (buffer.begin() + n + 1, *(byte + n));
}

} // namespace pl
} // namespace technicalmachine
