// Sha-2 implementation header
// Copyright (C) 2012 David Stone
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

#ifndef CRYPTOGRAPHY_SHA2_HPP_
#define CRYPTOGRAPHY_SHA2_HPP_

#include <string>

namespace technicalmachine {
namespace cryptography {

std::string sha224 (std::string const & message);
std::string sha256 (std::string const & message);
std::string sha384 (std::string const & message);
std::string sha512 (std::string const & message);

}	// namespace cryptography
}	// namespace technicalmachine
#endif // CRYPTOGRAPHY_SHA2_HPP_
