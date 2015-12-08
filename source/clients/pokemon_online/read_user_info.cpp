// Read "User" message from PO
// Copyright (C) 2015 David Stone
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

#include "read_user_info.hpp"

#include "inmessage.hpp"

#include <bounded_integer/integer_range.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {
namespace po {
namespace {

using namespace bounded::literal;

auto load_team_vector(InMessage & msg) {
	PokemonStaticVector team;
	for (auto const n : bounded::integer_range(max_pokemon_per_team)) {
		static_cast<void>(n);
		uint16_t const species = msg.read_short();
		uint8_t const forme = msg.read_byte();
		team.emplace_back(species, forme);
	}
	return team;
}

}	// namespace

User::User(InMessage & msg):
	id(msg.read_int()),
	name(msg.read_string()),
	info(msg.read_string()),
	authority(static_cast<int8_t>(msg.read_byte())),
	flags(msg.read_byte()),
	logged_in(flags % 2 >= 1),
	battling(flags % 4 >= 2),
	away(flags % 8 >= 4),
	rating(static_cast<int16_t>(msg.read_short())),
	team(load_team_vector(msg)),
	avatar(msg.read_short()),
	tier(msg.read_string()),

	color_spec(msg.read_byte()),
	alpha(msg.read_short()),
	red(msg.read_short()),
	green(msg.read_short()),
	blue(msg.read_short()),
	padding(msg.read_short()),

	gen(msg.read_byte()) {
}

}	// namespace po
}	// namespace technicalmachine
