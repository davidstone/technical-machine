// Convert string to hex representation
// Copyright (C) 2011 David Stone
//
// This program is free software: you can redistribute it and / or modify
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

#include "hex.hpp"
#include <string>

namespace technicalmachine {
namespace cryptography {

std::string get_hex_string (std::string const & digest) {
	static constexpr char hex_table [] = "0123456789ABCDEF";
	std::string output;
	output.resize (2 * digest.size ());
	for (unsigned n = 0; n < digest.size (); ++n) {
		unsigned const high = (static_cast <uint8_t const> (digest [n]) / 16);
		unsigned const low = (static_cast <uint8_t const> (digest [n]) % 16);
		output [n * 2] = hex_table [high];
		output [n * 2 + 1] = hex_table [low];
	}
	return output;
}

}	// namespace cryptography
}	// namespace technicalmachine
