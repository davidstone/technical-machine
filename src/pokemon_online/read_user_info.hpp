// Read "User" message from PO
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

#ifndef POKEMON_ONLINE_READ_USER_INFO_H_
#define POKEMON_ONLINE_READ_USER_INFO_H_

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace technicalmachine {
namespace po {

class InMessage;

class User {
	public:
		uint32_t const id;
		std::string const name;
		std::string const info;
		int8_t const authority;
		uint8_t const flags;
		bool const logged_in;
		bool const battling;
		bool const away;
		int16_t const rating;
		std::vector <std::pair <uint16_t, uint8_t>> const team;
		uint16_t avatar;
		std::string const tier;
		uint8_t const color_spec;
		uint16_t const alpha;
		uint16_t const red;
		uint16_t const green;
		uint16_t const blue;
		uint16_t const padding;
		uint8_t const gen;
		explicit User (InMessage & msg);
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE_READ_USER_INFO_HPP_
