// Test function that determines the score functions
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

#include <iostream>

#undef NDEBUG

#include <cassert>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<typename RandomEngine, typename... Ts>
auto make_shuffled_array(RandomEngine & random_engine, Ts ... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto array = containers::array{Move(ts)...};
	// gcc's stdlib does not support std::shuffle user defined integers
	std::shuffle(data(array), data(array) + size(array), random_engine);
	return array;
}

void ohko_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;

	Team team1(1_bi, true);
	{
		team1.add_pokemon(Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		Pokemon & jolteon = team1.pokemon();
		containers::append(all_moves(jolteon), shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball));
		for (auto const stat : {StatNames::SPA, StatNames::SPE}) {
			set_stat_ev(jolteon, stat, EV(252_bi));
		}
	}

	Team team2(1_bi);
	{
		team2.add_pokemon(Species::Gyarados, Level(100_bi), Gender::male, Item::Leftovers, Ability::Intimidate, Nature::Adamant);
		Pokemon & gyarados = team2.pokemon();
		containers::append(all_moves(gyarados), shuffled(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt));
		for (auto const stat : {StatNames::ATK, StatNames::SPE}) {
			set_stat_ev(gyarados, stat, EV(252_bi));
		}
	}

	assert(expectiminimax(team1, team2, weather, depth, evaluate) == Moves::Thunderbolt);
	
	Team team3(1_bi);
	{
		team3.add_pokemon(Species::Shedinja, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Wonder_Guard, Nature::Adamant);
		Pokemon & shedinja = team3.pokemon();
		containers::append(all_moves(shedinja), shuffled(Moves::Swords_Dance, Moves::X_Scissor, Moves::Shadow_Sneak, Moves::Will_O_Wisp));
		for (auto const stat : {StatNames::ATK, StatNames::SPE}) {
			set_stat_ev(shedinja, stat, EV(252_bi));
		}
	}
	
	// TODO: implement Wonder_Guard
//	assert(expectiminimax(team1, team3, weather, depth, evaluate) == Moves::Shadow_Ball);
}

void one_turn_damage_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 1;
	
	Team attacker(1_bi, true);
	{
		attacker.add_pokemon(Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		Pokemon & jolteon = attacker.pokemon();
		containers::append(all_moves(jolteon), shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball));
		for (auto const stat : {StatNames::SPA, StatNames::SPE}) {
			set_stat_ev(jolteon, stat, EV(252_bi));
		}
	}

	Team defender(1_bi);
	{
		defender.add_pokemon(Species::Swampert, Level(100_bi), Gender::male, Item::Leftovers, Ability::Torrent, Nature::Bold);
		Pokemon & swampert = defender.pokemon();
		containers::append(all_moves(swampert), shuffled(Moves::Surf, Moves::Ice_Beam));
		set_hp_ev(swampert, EV(252_bi));
		set_stat_ev(swampert, StatNames::DEF, EV(252_bi));
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Shadow_Ball);
}

void bellyzard_vs_defensive(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 2;
	Team attacker(1_bi, true);
	{
		attacker.add_pokemon(Species::Charizard, Level(100_bi), Gender::male, Item::Salac_Berry, Ability::Blaze, Nature::Adamant);
		Pokemon & charizard = attacker.pokemon();
		containers::append(all_moves(charizard), shuffled(Moves::Fire_Punch, Moves::Belly_Drum, Moves::Earthquake, Moves::Double_Edge));
		for (auto const stat : {StatNames::ATK, StatNames::SPE}) {
			set_stat_ev(charizard, stat, EV(252_bi));
		}
	}

	Team defender(1_bi);
	{
		defender.add_pokemon(Species::Mew, Level(100_bi), Gender::male, Item::Leftovers, Ability::Synchronize, Nature::Impish);
		Pokemon & mew = defender.pokemon();
		containers::append(all_moves(mew), shuffled(Moves::Softboiled));
		set_hp_ev(mew, EV(252_bi));
		set_stat_ev(mew, StatNames::SPD, EV(64_bi));
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Belly_Drum);
}

void hippopotas_vs_wobbuffet(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 7;
	Team attacker(1_bi, true);
	{
		attacker.add_pokemon(Species::Hippopotas, Level(100_bi), Gender::male, Item::Leftovers, Ability::Sand_Stream, Nature::Adamant);
		Pokemon & hippopotas = attacker.pokemon();
		containers::append(all_moves(hippopotas), shuffled(Moves::Curse, Moves::Crunch));
		set_hp_ev(hippopotas, EV(252_bi));
		set_stat_ev(hippopotas, StatNames::ATK, EV(252_bi));
		set_stat_ev(hippopotas, StatNames::SPE, EV(4_bi));
	}

	Team defender(1_bi);
	{
		defender.add_pokemon(Species::Wobbuffet, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Shadow_Tag, Nature::Bold);
		Pokemon & wobbuffet = defender.pokemon();
		containers::append(all_moves(wobbuffet), shuffled(Moves::Counter, Moves::Safeguard));
		set_hp_ev(wobbuffet, EV(252_bi));
		set_stat_ev(wobbuffet, StatNames::DEF, EV(252_bi));
		set_stat_ev(wobbuffet, StatNames::SPE, EV(4_bi));
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Curse);
}


void baton_pass(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 5;
	Team attacker(1_bi, true);
	{
		attacker.add_pokemon(Species::Smeargle, Level(100_bi), Gender::male, Item::Leftovers, Ability::Own_Tempo, Nature::Jolly);
		auto & smeargle = back(attacker.all_pokemon());
		containers::append(all_moves(smeargle), shuffled(Moves::Baton_Pass, Moves::Belly_Drum));
	}

	{
		attacker.add_pokemon(Species::Alakazam, Level(100_bi), Gender::male, Item::Choice_Band, Ability::Synchronize, Nature::Jolly);
		auto & alakazam = back(attacker.all_pokemon());
		containers::append(all_moves(alakazam), shuffled(Moves::Bite, Moves::Recover));
	}

	Team defender(2_bi);
	{
		defender.add_pokemon(Species::Gengar, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & gengar = defender.pokemon();
		containers::append(all_moves(gengar), shuffled(Moves::Shadow_Ball));
		set_stat_ev(gengar, StatNames::SPA, EV(252_bi));
	}

	{
		defender.add_pokemon(Species::Misdreavus, Level(100_bi), Gender::female, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		Pokemon & misdreavus = back(defender.all_pokemon());
		containers::append(all_moves(misdreavus), shuffled(Moves::Shadow_Ball));
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Belly_Drum);
}


void replace_fainted(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto weather = Weather{};
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 2;
	Team attacker(3_bi, true);

	attacker.add_pokemon(Species::Magikarp, Level(5_bi), Gender::male, Item::Leftovers, Ability::Swift_Swim, Nature::Jolly);

	{
		attacker.add_pokemon(Species::Slugma, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Magma_Armor, Nature::Jolly);
		auto & slugma = back(attacker.all_pokemon());
		containers::append(all_moves(slugma), shuffled(Moves::Flamethrower, Moves::Earth_Power));
	}

	{
		attacker.add_pokemon(Species::Zapdos, Level(100_bi), Gender::genderless, Item::Choice_Specs, Ability::Pressure, Nature::Modest);
		auto & zapdos = back(attacker.all_pokemon());
		all_moves(zapdos).emplace_back(Moves::Thunderbolt);
		set_hp_ev(zapdos, EV(4_bi));
		for (auto const stat : { StatNames::SPA, StatNames::SPE }) {
			set_stat_ev(zapdos, stat, EV(252_bi));
		}
	}

	Team defender(1_bi);
	{
		defender.add_pokemon(Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Bold);
		Pokemon & suicune = defender.pokemon();
		containers::append(all_moves(suicune), shuffled(Moves::Calm_Mind, Moves::Surf, Moves::Ice_Beam));
		set_hp_ev(suicune, EV(252_bi));
		set_stat_ev(suicune, StatNames::DEF, EV(252_bi));
		set_stat_ev(suicune, StatNames::SPD, EV(4_bi));
	}

	auto const variable = Variable{};
	call_move(defender, Move(Moves::Surf), false, attacker, bounded::none, false, weather, variable, false, false, false, bounded::none);
	
	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Switch2);
}


void latias_vs_suicune(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto const weather = Weather{};
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(random_engine, args...);
	};
	constexpr auto depth = 3;
	Team attacker(1_bi, true);
	{
		attacker.add_pokemon(Species::Latias, Level(100_bi), Gender::female, Item::Leftovers, Ability::Levitate, Nature::Calm);
		Pokemon & latias = attacker.pokemon();
		containers::append(all_moves(latias), shuffled(Moves::Calm_Mind, Moves::Dragon_Pulse, Moves::Recover));
		set_hp_ev(latias, EV(252_bi));
		set_stat_ev(latias, StatNames::SPA, EV(120_bi));
		set_stat_ev(latias, StatNames::SPD, EV(136_bi));
	}

	Team defender(1_bi);
	{
		defender.add_pokemon(Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Calm);
		Pokemon & suicune = defender.pokemon();
		containers::append(all_moves(suicune), shuffled(Moves::Ice_Beam, Moves::Rest));
		set_hp_ev(suicune, EV(252_bi));
		set_stat_ev(suicune, StatNames::SPA, EV(120_bi));
		set_stat_ev(suicune, StatNames::SPD, EV(136_bi));
	}

	assert(expectiminimax(attacker, defender, weather, depth, evaluate) == Moves::Calm_Mind);
}

void performance(Evaluate const & evaluate) {
	auto const weather = Weather{};
	constexpr auto depth = 2;
	auto add_pokemon = [&](Team & team, Species const species, auto... moves) {
		team.add_pokemon(species, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pickup, Nature::Hardy);
		Pokemon & pokemon = back(team.all_pokemon());
		containers::append(all_moves(pokemon), containers::array{Move(moves)...});
		set_hp_ev(pokemon, EV(252_bi));
		set_stat_ev(pokemon, StatNames::DEF, EV(120_bi));
		set_stat_ev(pokemon, StatNames::SPD, EV(136_bi));
	};
	Team ai(6_bi, true);
	Team foe(6_bi, false);
	for (auto const species : {Species::Latias, Species::Latios, Species::Tyranitar, Species::Hippowdon, Species::Salamence, Species::Slugma}) {
		for (auto * team : {&ai, &foe}) {
			add_pokemon(*team, species, Moves::Recover, Moves::Dragon_Claw, Moves::Earthquake, Moves::Dragon_Dance);
		}
	}

	expectiminimax(ai, foe, weather, depth, evaluate);
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
	performance(evaluate);
	
	std::cout << "Evaluate tests passed.\n\n";
}

}	// namespace technicalmachine
