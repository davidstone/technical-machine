// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/clients/pokemon_showdown/packed_team.hpp>

#include <tm/clients/pokemon_showdown/packed_format_to_team.hpp>
#include <tm/clients/pokemon_showdown/to_packed_format.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/begin_end.hpp>

#include <iostream>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {

void test_packed_team() {
	using namespace std::string_view_literals;
	std::cout << "Testing ps::packed_team\n";
	// TODO: test exact example string from PS documentation at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md#team-format
	constexpr auto initial = "Lumineon||Focus Sash|Honey Gather|Defog,Scald,Ice Beam,U-turn|Serious|84,84,84,84,80,84|Genderless|31,31,31,31,31,31||83|]Glaceon||Life Orb|Honey Gather|Toxic,Hidden Power,Shadow Ball,Ice Beam|Serious|80,0,84,80,80,84|Genderless|31,3,31,31,31,31||83|]Crabominable||Choice Band|Honey Gather|Close Combat,Earthquake,Stone Edge,Ice Hammer|Serious|84,84,80,84,84,84|Genderless|31,31,31,31,31,31||83|]Toxicroak||Life Orb|Honey Gather|Drain Punch,Sucker Punch,Gunk Shot,Substitute|Serious|84,84,84,80,84,84|Genderless|31,31,31,31,31,31||79|]Bouffalant||Choice Band|Honey Gather|Earthquake,Megahorn,Head Charge,Superpower|Serious|80,84,80,84,80,84|Genderless|31,31,31,31,31,31||83|]Qwilfish||Black Sludge|Honey Gather|Thunder Wave,Destiny Bond,Liquidation,Pain Split|Serious|84,80,84,84,84,84|Genderless|31,31,31,31,31,31||83|"sv;
	constexpr auto generation = Generation::seven;
	auto const team = packed_format_to_team<generation>(initial);
	auto const output = to_packed_format(team);
	if (output != initial) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected:\n\n"sv, initial, " but got\n\n"sv, output));
	}
}

} // namespace ps
} // namespace technicalmachine
