// Copyright (C) 2018 David Stone
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

#include <tm/test/clients/pokemon_showdown/packed_team.hpp>

#include <tm/clients/pokemon_showdown/packed_team.hpp>

#include <iostream>
#include <string_view>

namespace technicalmachine {
namespace ps {

void test_packed_team() {
	std::cout << "Testing ps::packed_team\n";
	// TODO: test exact example string from PS documentation at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md#team-format
	constexpr auto initial = std::string_view("Lumineon||Focus Sash|Honey Gather|Defog,Scald,Ice Beam,U-turn|Serious|84,84,84,84,80,84|Genderless|31,31,31,31,31,31||83|]Glaceon||Life Orb|Honey Gather|Toxic,Hidden Power,Shadow Ball,Ice Beam|Serious|80,0,84,80,80,84|Genderless|31,3,31,31,31,31||83|]Crabominable||Choice Band|Honey Gather|Close Combat,Earthquake,Stone Edge,Ice Hammer|Serious|84,84,80,84,84,84|Genderless|31,31,31,31,31,31||83|]Toxicroak||Life Orb|Honey Gather|Drain Punch,Sucker Punch,Gunk Shot,Substitute|Serious|84,84,84,80,84,84|Genderless|31,31,31,31,31,31||79|]Bouffalant||Choice Band|Honey Gather|Earthquake,Megahorn,Head Charge,Superpower|Serious|80,84,80,84,80,84|Genderless|31,31,31,31,31,31||83|]Qwilfish||Black Sludge|Honey Gather|Thunder Wave,Destiny Bond,Liquidation,Pain Split|Serious|84,80,84,84,84,84|Genderless|31,31,31,31,31,31||83|");
	constexpr auto generation = Generation::seven;
	auto const team = packed_format_to_team(initial, generation);
	auto const output = to_packed_format(generation, team);
	if (output != initial) {
		throw std::runtime_error("Expected:\n\n" + std::string(initial) + " but got\n\n" + std::string(begin(output), end(output)));
	}
}

} // namespace ps
} // namespace technicalmachine
