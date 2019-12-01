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

#include <tm/test/call_move.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto damage = ActualDamage::Unknown{};	

constexpr auto move_array(auto... moves) {
	return containers::array{Move(generation, moves)...};
}

void test_baton_pass() {
	Team attacker(2_bi, true);
	{
		attacker.add_pokemon(generation, Species::Smeargle, Level(100_bi), Gender::male, Item::Leftovers, Ability::Own_Tempo, Nature::Jolly);
		auto & smeargle = back(attacker.all_pokemon());
		containers::append(regular_moves(smeargle), move_array(Moves::Baton_Pass, Moves::Belly_Drum));
	}

	{
		attacker.add_pokemon(generation, Species::Alakazam, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Synchronize, Nature::Jolly);
		auto & alakazam = back(attacker.all_pokemon());
		containers::append(regular_moves(alakazam), move_array(Moves::Psycho_Cut, Moves::Recover));
		set_stat_ev(alakazam, StatNames::ATK, EV(252_bi));
	}

	Team defender(2_bi);
	{
		defender.add_pokemon(generation, Species::Gengar, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & gengar = defender.pokemon();
		containers::append(regular_moves(gengar), move_array(Moves::Shadow_Ball));
		set_stat_ev(gengar, StatNames::SPA, EV(252_bi));
	}

	{
		defender.add_pokemon(generation, Species::Misdreavus, Level(100_bi), Gender::female, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & misdreavus = back(defender.all_pokemon());
		containers::append(regular_moves(misdreavus), move_array(Moves::Shadow_Ball));
		set_stat_ev(misdreavus, StatNames::SPA, EV(252_bi));
	}

	auto weather = Weather{};
	call_move(
		generation,
		attacker,
		UsedMove{Moves::Belly_Drum},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	if (attacker.pokemon().stage()[StatNames::ATK] != 6_bi) {
		std::cerr << "Belly Drum did not max out Attack\n";
		std::terminate();
	}
	call_move(
		generation,
		attacker,
		UsedMove{Moves::Baton_Pass},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	if (attacker.pokemon().stage()[StatNames::ATK] != 6_bi) {
		std::cerr << "Baton Pass immediately cleared stat boosts\n";
		std::terminate();
	}
	call_move(
		generation,
		attacker,
		UsedMove{Moves::Switch1},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	if (attacker.pokemon().stage()[StatNames::ATK] != 6_bi) {
		std::cerr << "Baton Pass cleared stat boosts after switching\n";
		std::terminate();
	}
	if (attacker.pokemon() != Species::Alakazam) {
		std::cerr << "Baton Pass did not bring out a new Pokemon\n";
		std::terminate();
	}
}

void sleep_talk() {
	auto weather = Weather{};
	auto attacker = Team(1_bi, true);
	{
		attacker.add_pokemon(generation, Species::Jolteon, Level(100_bi), Gender::female, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		auto jolteon = attacker.pokemon();
		jolteon.switch_in(generation);
		containers::append(regular_moves(jolteon), move_array(Moves::Sleep_Talk, Moves::Thunderbolt));
		set_hp_ev(generation, jolteon, EV(4_bi));
		set_stat_ev(jolteon, StatNames::SPA, EV(252_bi));
		set_stat_ev(jolteon, StatNames::SPE, EV(252_bi));
	}

	auto defender = Team(1_bi);
	{
		defender.add_pokemon(generation, Species::Gyarados, Level(100_bi), Gender::male, Item::Life_Orb, Ability::Intimidate, Nature::Adamant);
		auto gyarados = defender.pokemon();
		gyarados.switch_in(generation);
		push_back(regular_moves(gyarados), Move(generation, Moves::Earthquake));
		set_hp_ev(generation, gyarados, EV(4_bi));
		set_stat_ev(gyarados, StatNames::ATK, EV(252_bi));
		set_stat_ev(gyarados, StatNames::SPE, EV(252_bi));
	}

	auto jolteon = attacker.pokemon();
	constexpr auto uproar = false;
	apply_status_to_self(Statuses::sleep, jolteon, weather, uproar);

	call_move(
		generation,
		attacker,
		UsedMove{Moves::Sleep_Talk, Moves::Thunderbolt},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	BOUNDED_ASSERT(get_hp(defender.pokemon()).current() == 0_bi);
}

}	// namespace

void call_move_tests() {
	std::cout << "Running call_move tests.\n";
	test_baton_pass();
	sleep_talk();
	std::cout << "Use move tests passed.\n\n";
}

}	// namespace technicalmachine
