// Detailed Pokemon stats
// Copyright (C) 2012 David Stone
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

#include "detailed_stats.hpp"

#include <cstddef>
#include <fstream>
#include <string>

#include "../ability.hpp"
#include "../item.hpp"
#include "../invalid_settings_file.hpp"

#include "../move/move.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../stat/nature.hpp"

namespace technicalmachine {

DetailedStats::DetailedStats() {
	static std::string const file_name = "settings/detailed.txt";
	std::ifstream file (file_name);
	std::string line;
	Species old_member = Species::END;
	bool ability_found = false;
	bool item_found = false;
	bool nature_found = false;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		constexpr char delimiter = '\t';
		size_t const x = line.find (delimiter);
		Species new_member = Pokemon::from_string (line.substr (0, x));
		if (new_member >= Species::END)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::invalid_data);
		if (old_member != new_member) {
			old_member = new_member;
			ability_found = false;
			item_found = false;
			nature_found = false;
		}
		size_t const y = line.find(delimiter, x + 1);
		size_t const z = line.find(delimiter, y + 1);
		std::string const sub = line.substr(x + 1, y - x - 1);
		if (sub == "Ability") {
			if (!ability_found) {
				ability_found = true;
				ability[static_cast<size_t>(new_member)] = Ability::from_string(line.substr(y + 1, z - y - 1));
			}
		}
		else if (sub == "Item") {
			if (!item_found) {
				item_found = true;
				item[static_cast<size_t>(new_member)] = Item::from_string(line.substr(y + 1, z - y - 1));
			}
		}
		else if (sub == "Nature") {
			if (!nature_found) {
				nature_found = true;
				nature[static_cast<size_t>(new_member)] = Nature::from_string(line.substr(y + 1, z - y - 1));
			}
		}
		else if (sub == "Move") {
			// When I get smarter move statistics, I won't want to cap this at 4
			if (move[static_cast<size_t>(new_member)].size() < 4)
				move[static_cast<size_t>(new_member)].emplace_back(Move::from_string(line.substr(y + 1, z - y - 1)));
		}
	}
}

}	// namespace technicalmachine
