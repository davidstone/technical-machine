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

#include "use_move.hpp"

#include "incorrect_calculation.hpp"

#include "../move/use_move.hpp"

#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

void test_baton_pass() {
	Team attacker(2_bi, true);
	{
		attacker.add_pokemon(Species::Smeargle, Level(100_bi), Gender::male, Item::Leftovers, Ability::Own_Tempo, Nature::Jolly);
		auto & smeargle = back(attacker.all_pokemon());
		containers::append(all_moves(smeargle), containers::array{Move(Moves::Baton_Pass), Move(Moves::Belly_Drum)});
	}

	{
		attacker.add_pokemon(Species::Alakazam, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Synchronize, Nature::Jolly);
		auto & alakazam = back(attacker.all_pokemon());
		containers::append(all_moves(alakazam), containers::array{Move(Moves::Psycho_Cut), Move(Moves::Recover)});
		set_stat_ev(alakazam, StatNames::ATK, EV(252_bi));
	}

	Team defender(2_bi);
	{
		defender.add_pokemon(Species::Gengar, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & gengar = defender.pokemon();
		containers::append(all_moves(gengar), containers::array{Move(Moves::Shadow_Ball)});
		set_stat_ev(gengar, StatNames::SPA, EV(252_bi));
	}

	{
		defender.add_pokemon(Species::Misdreavus, Level(100_bi), Gender::female, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & misdreavus = back(defender.all_pokemon());
		containers::append(all_moves(misdreavus), containers::array{Move(Moves::Shadow_Ball)});
		set_stat_ev(misdreavus, StatNames::SPA, EV(252_bi));
	}
	
	auto weather = Weather{};
	call_move(attacker, Move(Moves::Belly_Drum), false, defender, bounded::none, false, weather, Variable{}, false, false, false, bounded::none);
	if (stage(attacker.pokemon())[StatNames::ATK] != 6_bi) {
		std::cerr << "Belly Drum did not max out Attack\n";
		std::terminate();
	}
	call_move(attacker, Move(Moves::Baton_Pass), false, defender, bounded::none, false, weather, Variable{}, false, false, false, bounded::none);
	if (stage(attacker.pokemon())[StatNames::ATK] != 6_bi) {
		std::cerr << "Baton Pass immediately cleared stat boosts\n";
		std::terminate();
	}
	call_move(attacker, Move(Moves::Switch1), false, defender, bounded::none, false, weather, Variable{}, false, false, false, bounded::none);
	if (stage(attacker.pokemon())[StatNames::ATK] != 6_bi) {
		std::cerr << "Baton Pass cleared stat boosts after switching\n";
		std::terminate();
	}
	if (attacker.pokemon() != Species::Alakazam) {
		std::cerr << "Baton Pass did not bring out a new Pokemon\n";
		std::terminate();
	}
}

}	// namespace

void use_move_tests() {
	std::cout << "Running use_move tests.\n";
	test_baton_pass();
	std::cout << "Use move tests passed.\n\n";
}

}	// namespace technicalmachine
