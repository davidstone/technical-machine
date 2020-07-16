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

#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto damage = ActualDamage::Unknown{};	

constexpr auto regular_moves(auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

void test_baton_pass() {
	auto weather = Weather();

	auto attacker = Team(2_bi, true);
	attacker.add_pokemon(Pokemon(
		generation,
		Species::Smeargle,
		Level(100_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Own_Tempo,
		Nature::Jolly,
		regular_moves(Moves::Baton_Pass, Moves::Belly_Drum)
	));

	{
		auto & alakazam = attacker.add_pokemon(Pokemon(
			generation,
			Species::Alakazam,
			Level(100_bi),
			Gender::male,
			Item::Lum_Berry,
			Ability::Synchronize,
			Nature::Jolly,
			regular_moves(Moves::Psycho_Cut, Moves::Recover)
		));
		alakazam.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));
	}
	attacker.pokemon().switch_in(generation, weather);

	auto defender = Team(2_bi);
	{
		auto & gengar = defender.add_pokemon(Pokemon(
			generation,
			Species::Gengar,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Levitate,
			Nature::Modest,
			regular_moves(Moves::Shadow_Ball)
		));
		gengar.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));
	}
	{
		auto & misdreavus = defender.add_pokemon(Pokemon(
			generation,
			Species::Misdreavus,
			Level(100_bi),
			Gender::female,
			Item::Choice_Specs,
			Ability::Levitate,
			Nature::Modest,
			regular_moves(Moves::Shadow_Ball)
		));
		misdreavus.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));
	}
	defender.pokemon().switch_in(generation, weather);
	attacker.reset_start_of_turn();

	BOUNDED_ASSERT(
		legal_selections(generation, attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

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
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(!attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(generation, attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);

	attacker.reset_start_of_turn();

	BOUNDED_ASSERT(
		legal_selections(generation, attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

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
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(generation, attacker, defender, weather) ==
		StaticVectorMove({Moves::Switch1})
	);

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
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(attacker.pokemon().species() == Species::Alakazam);
	BOUNDED_ASSERT(!attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(generation, attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);
}

void wonder_guard() {
	auto weather = Weather();

	auto attacker = Team(1_bi, true);
	attacker.add_pokemon(Pokemon(
		generation,
		Species::Jolteon,
		Level(100_bi),
		Gender::female,
		Item::None,
		Ability::Volt_Absorb,
		Nature::Timid,
		regular_moves(Moves::Shadow_Ball, Moves::Thunderbolt)
	));
	attacker.pokemon().switch_in(generation, weather);

	auto defender = Team(1_bi);
	defender.add_pokemon(Pokemon(
		generation,
		Species::Shedinja,
		Level(100_bi),
		Gender::male,
		Item::None,
		Ability::Wonder_Guard,
		Nature::Adamant,
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto shedinja = defender.pokemon();
	shedinja.switch_in(generation, weather);

	BOUNDED_ASSERT(shedinja.hp().current() == 1_bi);

	call_move(
		generation,
		attacker,
		UsedMove{Moves::Thunderbolt},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	BOUNDED_ASSERT(shedinja.hp().current() == 1_bi);

	call_move(
		generation,
		attacker,
		UsedMove{Moves::Shadow_Ball},
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	BOUNDED_ASSERT(shedinja.hp().current() == 0_bi);
}

}	// namespace

void call_move_tests() {
	std::cout << "Running call_move tests.\n";
	test_baton_pass();
	wonder_guard();
	std::cout << "Use move tests passed.\n\n";
}

}	// namespace technicalmachine
