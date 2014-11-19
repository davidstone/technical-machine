// Test function that determines the score functions
// Copyright (C) 2014 David Stone
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

#include "expectiminimax.hpp"

#include "../../evaluate/evaluate.hpp"
#include "../../evaluate/expectiminimax.hpp"
#include "../../team.hpp"
#include "../../weather.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/array.hpp>

#include <cassert>
#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<typename RandomEngine, typename... Ts>
auto make_shuffled_array(RandomEngine & random_engine, Ts ... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto array = bounded::make_array(ts...);
	// bounded::array::iterator does not support addition by integer literals
	std::shuffle(array.data(), array.data() + array.size(), random_engine);
	return array;
}

void ohko_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;

	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Jolteon, Level(100_bi), Gender::MALE, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
	for (auto const move : shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)) {
		all_moves(attacker.pokemon()).add(move);
	}
	get_stat(attacker.pokemon(), StatNames::SPA).ev = EV(252_bi);
	get_stat(attacker.pokemon(), StatNames::SPE).ev = EV(252_bi);

	Team defender(1_bi);
	defender.add_pokemon(Species::Gyarados, Level(100_bi), Gender::MALE, Item::Leftovers, Ability::Intimidate, Nature::Adamant);
	for (auto const move : shuffled(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt)) {
		all_moves(defender.pokemon()).add(move);
	}
	get_stat(defender.pokemon(), StatNames::ATK).ev = EV(252_bi);
	get_stat(defender.pokemon(), StatNames::SPE).ev = EV(252_bi);

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Thunderbolt);
	
	Team team3(1_bi);
	team3.add_pokemon(Species::Shedinja, Level(100_bi), Gender::MALE, Item::Lum_Berry, Ability::Wonder_Guard, Nature::Adamant);
	for (auto const move : shuffled(Moves::Swords_Dance, Moves::X_Scissor, Moves::Shadow_Sneak, Moves::Will_O_Wisp)) {
		all_moves(team3.pokemon()).add(move);
	}
	get_stat(team3.pokemon(), StatNames::ATK).ev = EV(252_bi);
	get_stat(team3.pokemon(), StatNames::SPE).ev = EV(252_bi);
	
	// TODO: implement Wonder_Guard
//	assert(expectiminimax(attacker, team3, weather, depth, evaluate, random_engine) == Moves::Shadow_Ball);
}

void one_turn_damage_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;
	
	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Jolteon, Level(100_bi), Gender::MALE, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
	for (auto const move : shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)) {
		all_moves(attacker.pokemon()).add(move);
	}
	get_stat(attacker.pokemon(), StatNames::SPA).ev = EV(252_bi);
	get_stat(attacker.pokemon(), StatNames::SPE).ev = EV(252_bi);

	Team defender(1_bi);
	defender.add_pokemon(Species::Swampert, Level(100_bi), Gender::MALE, Item::Leftovers, Ability::Torrent, Nature::Bold);
	for (auto const move : shuffled(Moves::Surf, Moves::Ice_Beam)) {
		all_moves(defender.pokemon()).add(move);
	}
	get_hp(defender.pokemon()).ev = EV(252_bi);
	get_stat(defender.pokemon(), StatNames::DEF).ev = EV(252_bi);

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Shadow_Ball);
}

void bellyzard_vs_defensive(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 2;
	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Charizard, Level(100_bi), Gender::MALE, Item::Salac_Berry, Ability::Blaze, Nature::Adamant);
	for (auto const move : shuffled(Moves::Fire_Punch, Moves::Belly_Drum, Moves::Earthquake, Moves::Double_Edge)) {
		all_moves(attacker.pokemon()).add(move);
	}
	get_stat(attacker.pokemon(), StatNames::ATK).ev = EV(252_bi);
	get_stat(attacker.pokemon(), StatNames::SPE).ev = EV(252_bi);

	Team defender(1_bi);
	defender.add_pokemon(Species::Mew, Level(100_bi), Gender::MALE, Item::Leftovers, Ability::Synchronize, Nature::Impish);
	for (auto const move : shuffled(Moves::Softboiled)) {
		all_moves(defender.pokemon()).add(move);
	}
	get_hp(defender.pokemon()).ev = EV(252_bi);
	get_stat(defender.pokemon(), StatNames::DEF).ev = EV(0_bi);
	get_stat(defender.pokemon(), StatNames::SPD).ev = EV(64_bi);

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Belly_Drum);
}

}	// namespace

void expectiminimax_tests() {
	std::cout << "Running expectiminimax tests.\n";

	Evaluate const evaluate;
	Weather const weather;
	std::random_device rd;
	std::mt19937 random_engine(rd());
	
	ohko_tests(evaluate, weather, random_engine);
	one_turn_damage_tests(evaluate, weather, random_engine);
	bellyzard_vs_defensive(evaluate, weather, random_engine);
	
	std::cout << "Evaluate tests passed.\n\n";
}

}	// namespace technicalmachine
