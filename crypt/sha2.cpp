/*
 * This file is a part of Shoddy Battle.
 * Copyright (C) 2009  Catherine Fitzpatrick and Benjamin Gwin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, visit the Free Software Foundation, Inc.
 * online at http://gnu.org.
 */

#include <iostream>
#include <string>
#include "sha2.hpp"
#include "hex.h"
#include "sha2.h"

namespace technicalmachine {

std::string getSHA256Hash (std::string const & message) {
	unsigned char digest [32];
	sha256 (reinterpret_cast<unsigned char const *>(message.c_str()), message.length(), digest);
	std::string str = "";
	for (unsigned n = 0; n != 32; ++n)
		str += digest [n];
	return str;
}

}
