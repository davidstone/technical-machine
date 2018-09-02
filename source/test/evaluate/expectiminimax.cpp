// Test function that determines the score functions
// Copyright (C) 2015 David Stone
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
#include "../../variable.hpp"
#include "../../weather.hpp"

#include "../../move/moves.hpp"
#include "../../move/use_move.hpp"

#include "../../pokemon/species.hpp"

#include <bounded/integer.hpp>

#include <containers/array/array.hpp>

#include <cassert>
#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<typename RandomEngine, typename... Ts>
auto make_shuffled_array(RandomEngine & random_engine, Ts ... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto array = containers::array{ts...};
	// gcc's stdlib does not support std::shuffle user defined intergers
	std::shuffle(data(array), data(array) + size(array), random_engine);
	return array;
}

void ohko_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;

	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
	for (auto const move : shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)) {
		all_moves(attacker.pokemon()).emplace_back(move);
	}
	set_stat_ev(attacker.pokemon(), StatNames::SPA, EV(252_bi));
	set_stat_ev(attacker.pokemon(), StatNames::SPE, EV(252_bi));

	Team defender(1_bi);
	defender.add_pokemon(Species::Gyarados, Level(100_bi), Gender::male, Item::Leftovers, Ability::Intimidate, Nature::Adamant);
	for (auto const move : shuffled(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_stat_ev(defender.pokemon(), StatNames::ATK, EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPE, EV(252_bi));

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Thunderbolt);
	
	Team team3(1_bi);
	team3.add_pokemon(Species::Shedinja, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Wonder_Guard, Nature::Adamant);
	for (auto const move : shuffled(Moves::Swords_Dance, Moves::X_Scissor, Moves::Shadow_Sneak, Moves::Will_O_Wisp)) {
		all_moves(team3.pokemon()).emplace_back(move);
	}
	set_stat_ev(team3.pokemon(), StatNames::ATK, EV(252_bi));
	set_stat_ev(team3.pokemon(), StatNames::SPE, EV(252_bi));
	
	// TODO: implement Wonder_Guard
//	assert(expectiminimax(attacker, team3, weather, depth, evaluate, random_engine) == Moves::Shadow_Ball);
}

void one_turn_damage_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;
	
	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
	for (auto const move : shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)) {
		all_moves(attacker.pokemon()).emplace_back(move);
	}
	set_stat_ev(attacker.pokemon(), StatNames::SPA, EV(252_bi));
	set_stat_ev(attacker.pokemon(), StatNames::SPE, EV(252_bi));

	Team defender(1_bi);
	defender.add_pokemon(Species::Swampert, Level(100_bi), Gender::male, Item::Leftovers, Ability::Torrent, Nature::Bold);
	for (auto const move : shuffled(Moves::Surf, Moves::Ice_Beam)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_hp_ev(defender.pokemon(), EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::DEF, EV(252_bi));

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Shadow_Ball);
}

void bellyzard_vs_defensive(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 2;
	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Charizard, Level(100_bi), Gender::male, Item::Salac_Berry, Ability::Blaze, Nature::Adamant);
	for (auto const move : shuffled(Moves::Fire_Punch, Moves::Belly_Drum, Moves::Earthquake, Moves::Double_Edge)) {
		all_moves(attacker.pokemon()).emplace_back(move);
	}
	set_stat_ev(attacker.pokemon(), StatNames::ATK, EV(252_bi));
	set_stat_ev(attacker.pokemon(), StatNames::SPE, EV(252_bi));

	Team defender(1_bi);
	defender.add_pokemon(Species::Mew, Level(100_bi), Gender::male, Item::Leftovers, Ability::Synchronize, Nature::Impish);
	for (auto const move : shuffled(Moves::Softboiled)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_hp_ev(defender.pokemon(), EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::DEF, EV(0_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPD, EV(64_bi));

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Belly_Drum);
}

void hippopotas_vs_wobbuffet(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 7;
	Team attacker(1_bi, true);
	attacker.add_pokemon(Species::Hippopotas, Level(100_bi), Gender::male, Item::Leftovers, Ability::Sand_Stream, Nature::Adamant);
	for (auto const move : shuffled(Moves::Curse, Moves::Crunch)) {
		all_moves(attacker.pokemon()).emplace_back(move);
	}
	set_hp_ev(attacker.pokemon(), EV(252_bi));
	set_stat_ev(attacker.pokemon(), StatNames::ATK, EV(252_bi));
	set_stat_ev(attacker.pokemon(), StatNames::SPE, EV(4_bi));

	Team defender(1_bi);
	defender.add_pokemon(Species::Wobbuffet, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Shadow_Tag, Nature::Bold);
	for (auto const move : shuffled(Moves::Counter, Moves::Safeguard)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_hp_ev(defender.pokemon(), EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::DEF, EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPE, EV(4_bi));

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Curse);
}


void baton_pass(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 5;
	Team attacker(1_bi, true);

	attacker.add_pokemon(Species::Smeargle, Level(100_bi), Gender::male, Item::Leftovers, Ability::Own_Tempo, Nature::Jolly);
	auto & smeargle = back(attacker.all_pokemon());
	for (auto const move : shuffled(Moves::Baton_Pass, Moves::Belly_Drum)) {
		all_moves(smeargle).emplace_back(move);
	}

	attacker.add_pokemon(Species::Alakazam, Level(100_bi), Gender::male, Item::Choice_Band, Ability::Synchronize, Nature::Jolly);
	auto & alakazam = back(attacker.all_pokemon());
	for (auto const move : shuffled(Moves::Bite, Moves::Recover)) {
		all_moves(alakazam).emplace_back(move);
	}


	Team defender(2_bi);
	defender.add_pokemon(Species::Gengar, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
	for (auto const move : shuffled(Moves::Shadow_Ball)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_stat_ev(defender.pokemon(), StatNames::SPA, EV(252_bi));

	defender.add_pokemon(Species::Misdreavus, Level(100_bi), Gender::female, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
	for (auto const move : shuffled(Moves::Shadow_Ball)) {
		all_moves(defender.pokemon(1_bi)).emplace_back(move);
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Belly_Drum);
}


void replace_fainted(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto weather = Weather{};
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 2;
	Team attacker(3_bi, true);

	attacker.add_pokemon(Species::Magikarp, Level(5_bi), Gender::male, Item::Leftovers, Ability::Swift_Swim, Nature::Jolly);

	attacker.add_pokemon(Species::Slugma, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Magma_Armor, Nature::Jolly);
	for (auto const move : shuffled(Moves::Flamethrower, Moves::Earth_Power)) {
		all_moves(back(attacker.all_pokemon())).emplace_back(move);
	}

	attacker.add_pokemon(Species::Zapdos, Level(100_bi), Gender::genderless, Item::Choice_Specs, Ability::Pressure, Nature::Modest);
	all_moves(back(attacker.all_pokemon())).emplace_back(Moves::Thunderbolt);
	set_hp_ev(back(attacker.all_pokemon()), EV(4_bi));
	set_stat_ev(back(attacker.all_pokemon()), StatNames::SPA, EV(252_bi));
	set_stat_ev(back(attacker.all_pokemon()), StatNames::SPE, EV(252_bi));


	Team defender(1_bi);
	defender.add_pokemon(Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Bold);
	for (auto const move : shuffled(Moves::Calm_Mind, Moves::Surf, Moves::Ice_Beam)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_hp_ev(defender.pokemon(), EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::DEF, EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPD, EV(4_bi));

	auto const variable = Variable{};
	call_move(defender, Move(Moves::Surf), false, attacker, bounded::none, false, weather, variable, false, false, false, bounded::none);
	

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Switch2);
}


void latias_vs_suicune(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto const weather = Weather{};
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 3;
	Team attacker(1_bi, true);

	attacker.add_pokemon(Species::Latias, Level(100_bi), Gender::female, Item::Leftovers, Ability::Levitate, Nature::Calm);
	for (auto const move : shuffled(Moves::Calm_Mind, Moves::Dragon_Pulse, Moves::Recover)) {
		all_moves(back(attacker.all_pokemon())).emplace_back(move);
	}

	set_hp_ev(back(attacker.all_pokemon()), EV(252_bi));
	set_stat_ev(back(attacker.all_pokemon()), StatNames::SPA, EV(120_bi));
	set_stat_ev(back(attacker.all_pokemon()), StatNames::SPD, EV(136_bi));


	Team defender(1_bi);
	defender.add_pokemon(Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Calm);
	for (auto const move : shuffled(Moves::Ice_Beam, Moves::Rest)) {
		all_moves(defender.pokemon()).emplace_back(move);
	}
	set_hp_ev(defender.pokemon(), EV(252_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPA, EV(120_bi));
	set_stat_ev(defender.pokemon(), StatNames::SPD, EV(136_bi));

	assert(expectiminimax(attacker, defender, weather, depth, evaluate, random_engine) == Moves::Calm_Mind);
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
	hippopotas_vs_wobbuffet(evaluate, weather, random_engine);
	baton_pass(evaluate, weather, random_engine);
	replace_fainted(evaluate, random_engine);
	latias_vs_suicune(evaluate, random_engine);
	
	std::cout << "Evaluate tests passed.\n\n";
}

}	// namespace technicalmachine
