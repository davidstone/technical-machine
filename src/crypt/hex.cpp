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

#include <string>
#include "hex.h"

namespace technicalmachine {

static char const * HEX_TABLE = "0123456789ABCDEF";

std::string getHexString (unsigned char const * digest, int const length) {
	std::string hex (length * 2, ' ');
	for (int n = 0; n < length; ++n) {
		int const high = (digest [n] & 0xf0) >> 4;
		int const low = (digest [n] &  0x0f);
		hex [n * 2] = HEX_TABLE [high];
		hex [n * 2 + 1] = HEX_TABLE [low];
	}
	return hex;
}

}
