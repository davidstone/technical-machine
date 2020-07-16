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

#include <tm/test/evaluate/expectiminimax.hpp>

#include <tm/evaluate/evaluate.hpp>
#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/end_of_turn.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/array/array.hpp>
#include <containers/append.hpp>

#include <iostream>
#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto make_depth(unsigned const depth) {
	return Depth(DepthValues{depth, 0U}, 0U);
}

auto make_shuffled_array(Generation const generation, auto & random_engine, auto... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto array = containers::array{Move(generation, ts)...};
	// gcc's stdlib does not support std::shuffle user defined integers
	std::shuffle(data(array), data(array) + size(array), random_engine);
	return array;
}

void ohko_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1U);

	Team team1(1_bi, true);
	{
		auto jolteon = Pokemon(generation, Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		containers::append(jolteon.regular_moves(), shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball));
		jolteon.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));
		
		team1.add_pokemon(jolteon);
		team1.pokemon().switch_in(generation, weather);
		team1.reset_start_of_turn();
	}

	Team team2(1_bi);
	{
		auto gyarados = Pokemon(generation, Species::Gyarados, Level(100_bi), Gender::male, Item::Leftovers, Ability::Intimidate, Nature::Adamant);
		containers::append(gyarados.regular_moves(), shuffled(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt));
		gyarados.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));

		team2.add_pokemon(gyarados);
		team2.pokemon().switch_in(generation, weather);
		team2.reset_start_of_turn();
	}

	{
		auto const best_move = expectiminimax(generation, team1, team2, weather, evaluate, depth, std::cout);
		BOUNDED_ASSERT(best_move.name == Moves::Thunderbolt);
		BOUNDED_ASSERT(best_move.score == double(victory));
	}
	
	Team team3(1_bi);
	{
		auto shedinja = Pokemon(generation, Species::Shedinja, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Wonder_Guard, Nature::Adamant);
		containers::append(shedinja.regular_moves(), shuffled(Moves::Swords_Dance, Moves::X_Scissor, Moves::Shadow_Sneak, Moves::Will_O_Wisp));
		shedinja.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));

		team3.add_pokemon(shedinja);
		team3.pokemon().switch_in(generation, weather);
		team3.reset_start_of_turn();
	}
	
	{
		auto const best_move = expectiminimax(generation, team1, team3, weather, evaluate, depth, std::cout);
		BOUNDED_ASSERT(best_move.name == Moves::Shadow_Ball);
		BOUNDED_ASSERT(best_move.score == double(victory));
	}
}

void one_turn_damage_tests(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1U);
	
	Team attacker(1_bi, true);
	{
		auto jolteon = Pokemon(generation, Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		containers::append(jolteon.regular_moves(), shuffled(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball));
		jolteon.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));

		attacker.add_pokemon(jolteon);
		attacker.pokemon().switch_in(generation, weather);
		attacker.reset_start_of_turn();
	}

	Team defender(1_bi);
	{
		auto swampert = Pokemon(generation, Species::Swampert, Level(100_bi), Gender::male, Item::Leftovers, Ability::Torrent, Nature::Bold);
		containers::append(swampert.regular_moves(), shuffled(Moves::Surf, Moves::Ice_Beam));
		swampert.set_ev(generation, PermanentStat::hp, IV(31_bi), EV(252_bi));
		swampert.set_ev(generation, PermanentStat::def, IV(31_bi), EV(252_bi));

		defender.add_pokemon(swampert);
		defender.pokemon().switch_in(generation, weather);
		defender.reset_start_of_turn();
	}

	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout);
	BOUNDED_ASSERT(best_move.name == Moves::Shadow_Ball);
}

void bellyzard_vs_defensive(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(2U);
	Team attacker(1_bi, true);
	{
		auto charizard = Pokemon(generation, Species::Charizard, Level(100_bi), Gender::male, Item::Salac_Berry, Ability::Blaze, Nature::Adamant);
		containers::append(charizard.regular_moves(), shuffled(Moves::Fire_Punch, Moves::Belly_Drum, Moves::Earthquake, Moves::Double_Edge));
		charizard.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));

		attacker.add_pokemon(charizard);
		attacker.pokemon().switch_in(generation, weather);
		attacker.reset_start_of_turn();
	}

	Team defender(1_bi);
	{
		auto mew = Pokemon(generation, Species::Mew, Level(100_bi), Gender::male, Item::Leftovers, Ability::Synchronize, Nature::Impish);
		containers::append(mew.regular_moves(), shuffled(Moves::Soft_Boiled));
		mew.set_ev(generation, PermanentStat::hp, IV(31_bi), EV(252_bi));
		
		defender.add_pokemon(mew);
		defender.pokemon().switch_in(generation, weather);
		defender.reset_start_of_turn();
	}

	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout);
	BOUNDED_ASSERT(best_move.name == Moves::Belly_Drum);
	BOUNDED_ASSERT(best_move.score == double(victory));
}

void hippopotas_vs_wobbuffet(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(11U);
	Team attacker(1_bi, true);
	{
		auto hippopotas = Pokemon(generation, Species::Hippopotas, Level(100_bi), Gender::male, Item::Leftovers, Ability::Sand_Stream, Nature::Adamant);
		containers::append(hippopotas.regular_moves(), containers::array{Move(generation, Moves::Curse, 0_bi), Move(generation, Moves::Crunch, 0_bi)});
		hippopotas.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));
		
		attacker.add_pokemon(hippopotas);
		attacker.pokemon().switch_in(generation, weather);
		attacker.reset_start_of_turn();
	}

	// TODO: Implement Encore's effect ending when PP runs out, then Wobbuffet
	// can have Encore

	Team defender(1_bi);
	{
		auto wobbuffet = Pokemon(generation, Species::Wobbuffet, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Shadow_Tag, Nature::Calm);
		containers::append(wobbuffet.regular_moves(), shuffled(Moves::Counter));
		wobbuffet.set_ev(generation, PermanentStat::def, IV(31_bi), EV(252_bi));

		defender.add_pokemon(wobbuffet);
		defender.pokemon().switch_in(generation, weather);
		defender.reset_start_of_turn();
	}

	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout);
	BOUNDED_ASSERT(best_move.name == Moves::Curse);
	BOUNDED_ASSERT(best_move.score == double(victory));
}


void baton_pass(Evaluate const & evaluate, Weather const weather, std::mt19937 & random_engine) {
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = Depth(DepthValues{4U, 0U}, 0U);
	Team attacker(2_bi, true);
	{
		auto smeargle = Pokemon(generation, Species::Smeargle, Level(100_bi), Gender::male, Item::Leftovers, Ability::Own_Tempo, Nature::Jolly);
		containers::append(smeargle.regular_moves(), shuffled(Moves::Baton_Pass, Moves::Belly_Drum));

		attacker.add_pokemon(smeargle);
		attacker.pokemon().switch_in(generation, weather);
	}

	{
		auto alakazam = Pokemon(generation, Species::Alakazam, Level(100_bi), Gender::male, Item::Lum_Berry, Ability::Synchronize, Nature::Jolly);
		containers::append(alakazam.regular_moves(), shuffled(Moves::Psycho_Cut, Moves::Recover));
		alakazam.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));
		
		attacker.add_pokemon(alakazam);
	}
	attacker.reset_start_of_turn();

	Team defender(2_bi);
	{
		auto gengar = Pokemon(generation, Species::Gengar, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		containers::append(gengar.regular_moves(), shuffled(Moves::Shadow_Ball));
		gengar.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));

		defender.add_pokemon(gengar);
		defender.pokemon().switch_in(generation, weather);
	}

	{
		auto misdreavus = Pokemon(generation, Species::Misdreavus, Level(100_bi), Gender::female, Item::Choice_Specs, Ability::Levitate, Nature::Modest);
		containers::append(misdreavus.regular_moves(), shuffled(Moves::Shadow_Ball));
		misdreavus.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));

		defender.add_pokemon(misdreavus);
	}
	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cerr);
	BOUNDED_ASSERT(best_move.name == Moves::Belly_Drum);
	BOUNDED_ASSERT(best_move.score == double(victory));
}


void replace_fainted(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto weather = Weather{};
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(2U);
	auto attacker = Team(3_bi, true);

	{
		auto magikarp = Pokemon(generation, Species::Magikarp, Level(5_bi), Gender::male, Item::Leftovers, Ability::Swift_Swim, Nature::Jolly);
		attacker.add_pokemon(magikarp);
		attacker.pokemon().switch_in(generation, weather);
	}
	{
		auto slugma = Pokemon(generation, Species::Slugma, Level(100_bi), Gender::male, Item::Choice_Specs, Ability::Magma_Armor, Nature::Jolly);
		containers::append(slugma.regular_moves(), shuffled(Moves::Flamethrower, Moves::Earth_Power));
		attacker.add_pokemon(slugma);
	}
	{
		auto zapdos = Pokemon(generation, Species::Zapdos, Level(100_bi), Gender::genderless, Item::Choice_Specs, Ability::Pressure, Nature::Modest);
		zapdos.regular_moves().push_back(Move(generation, Moves::Thunderbolt));
		zapdos.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));
		attacker.add_pokemon(zapdos);
	}
	attacker.reset_start_of_turn();

	auto defender = Team(1_bi);
	{
		auto suicune = Pokemon(generation, Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Bold);
		containers::append(suicune.regular_moves(), shuffled(Moves::Calm_Mind, Moves::Surf, Moves::Ice_Beam));
		suicune.set_ev(generation, PermanentStat::hp, IV(31_bi), EV(252_bi));
		suicune.set_ev(generation, PermanentStat::def, IV(31_bi), EV(252_bi));
		
		defender.add_pokemon(suicune);
		defender.pokemon().switch_in(generation, weather);
	}
	defender.reset_start_of_turn();

	call_move(
		generation,
		defender,
		UsedMove{Moves::Surf},
		attacker,
		FutureMove{false},
		weather,
		false,
		ActualDamage::Unknown{}
	);
	
	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout);
	BOUNDED_ASSERT(best_move.name == Moves::Switch2);
}


void latias_vs_suicune(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto const weather = Weather{};
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(3U);
	auto attacker = Team(1_bi, true);
	{
		auto latias = Pokemon(generation, Species::Latias, Level(100_bi), Gender::female, Item::Leftovers, Ability::Levitate, Nature::Calm);
		containers::append(latias.regular_moves(), shuffled(Moves::Calm_Mind, Moves::Dragon_Pulse, Moves::Recover));
		latias.set_ev(generation, PermanentStat::hp, IV(31_bi), EV(252_bi));
		latias.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(120_bi));
		latias.set_ev(generation, PermanentStat::spd, IV(31_bi), EV(136_bi));

		attacker.add_pokemon(latias);
		attacker.pokemon().switch_in(generation, weather);
	}
	attacker.reset_start_of_turn();

	auto defender = Team(1_bi);
	{
		auto suicune = Pokemon(generation, Species::Suicune, Level(100_bi), Gender::genderless, Item::Leftovers, Ability::Pressure, Nature::Calm);
		containers::append(suicune.regular_moves(), shuffled(Moves::Ice_Beam, Moves::Rest));
		suicune.set_ev(generation, PermanentStat::hp, IV(31_bi), EV(252_bi));
		suicune.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(120_bi));
		suicune.set_ev(generation, PermanentStat::spd, IV(31_bi), EV(136_bi));

		defender.add_pokemon(suicune);
		defender.pokemon().switch_in(generation, weather);
	}
	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout);
	BOUNDED_ASSERT(best_move.name == Moves::Calm_Mind);
}

void sleep_talk(Evaluate const & evaluate, std::mt19937 & random_engine) {
	auto weather = Weather();
	constexpr auto generation = Generation::four;
	auto const shuffled = [&](auto... args) {
		return make_shuffled_array(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1U);
	auto attacker = Team(1_bi, true);
	{
		auto jolteon = Pokemon(generation, Species::Jolteon, Level(100_bi), Gender::female, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		containers::append(jolteon.regular_moves(), shuffled(Moves::Sleep_Talk, Moves::Thunderbolt));
		jolteon.set_ev(generation, PermanentStat::spa, IV(31_bi), EV(252_bi));
		jolteon.set_ev(generation, PermanentStat::spe, IV(31_bi), EV(252_bi));

		attacker.add_pokemon(jolteon);
		attacker.pokemon().switch_in(generation, weather);
		attacker.reset_start_of_turn();
	}

	auto defender = Team(1_bi);
	{
		auto gyarados = Pokemon(generation, Species::Gyarados, Level(100_bi), Gender::male, Item::Life_Orb, Ability::Intimidate, Nature::Adamant);
		containers::append(gyarados.regular_moves(), shuffled(Moves::Earthquake));
		gyarados.set_ev(generation, PermanentStat::atk, IV(31_bi), EV(252_bi));
		
		defender.add_pokemon(gyarados);
		defender.pokemon().switch_in(generation, weather);
	}
	defender.reset_start_of_turn();
	
	constexpr auto keep_status = false;
	constexpr auto unknown_damage = ActualDamage::Unknown{};
	constexpr auto sleep_talk = UsedMove{Moves::Sleep_Talk};
	constexpr auto thunderbolt = UsedMove{Moves::Thunderbolt};
	constexpr auto other_move = FutureMove{false};
	
	auto next_turn = [&]{
		constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false);
		end_of_turn(generation, attacker, end_of_turn_flags, defender, end_of_turn_flags, weather);
		attacker.reset_start_of_turn();
		defender.reset_start_of_turn();
	};

	auto jolteon = attacker.pokemon();
	
	// TODO: Validate score, too

	BOUNDED_ASSERT(jolteon.status().name() == Statuses::clear);
	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout).name == Moves::Thunderbolt);

	call_move(generation, attacker, sleep_talk, defender, other_move, weather, keep_status, unknown_damage);
	apply_status_to_self(generation, Statuses::sleep, jolteon, weather);
	next_turn();
	BOUNDED_ASSERT(jolteon.status().name() == Statuses::sleep);
	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cerr).name == Moves::Sleep_Talk);

	call_move(generation, attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
	next_turn();
	BOUNDED_ASSERT(jolteon.status().name() == Statuses::sleep);
	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout).name == Moves::Sleep_Talk);

	call_move(generation, attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
	next_turn();
	BOUNDED_ASSERT(jolteon.status().name() == Statuses::sleep);
	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout).name == Moves::Sleep_Talk);

	#if 0
		// Same probability of either move
		call_move(generation, attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
		next_turn();
		BOUNDED_ASSERT(jolteon.status().name() == Statuses::sleep);
		BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, depth, std::cout).name == ?);
	#endif
}

void generation_one_frozen_last_pokemon(Evaluate const & evaluate) {
	constexpr auto generation = Generation::one;
	auto weather = Weather();
	auto moves = [](auto const ... name) {
		return containers::array{Move(generation, name)...};
	};

	auto attacker = Team(1_bi, true);
	{
		auto alakazam = Pokemon(generation, Species::Alakazam, Level(100_bi), Gender::genderless, Item::None, Ability::Honey_Gather, Nature::Hardy);
		containers::append(alakazam.regular_moves(), moves(Moves::Psychic, Moves::Recover, Moves::Thunder_Wave, Moves::Seismic_Toss));

		attacker.add_pokemon(alakazam);
		attacker.pokemon().switch_in(generation, weather);
	}

	auto defender = Team(1_bi);
	{
		auto gengar = Pokemon(generation, Species::Gengar, Level(100_bi), Gender::genderless, Item::None, Ability::Honey_Gather, Nature::Hardy);
		containers::append(gengar.regular_moves(), moves(Moves::Explosion, Moves::Hypnosis, Moves::Thunderbolt, Moves::Night_Shade));
		gengar.set_status(Statuses::freeze);
		gengar.set_hp(12_bi);

		defender.add_pokemon(gengar);
		defender.pokemon().switch_in(generation, weather);
	}
	
	attacker.reset_start_of_turn();
	defender.reset_start_of_turn();

	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, make_depth(1U), std::cout).name == Moves::Psychic);
	BOUNDED_ASSERT(expectiminimax(generation, attacker, defender, weather, evaluate, make_depth(2U), std::cout).name == Moves::Psychic);
}

} // namespace

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
	sleep_talk(evaluate, random_engine);
	generation_one_frozen_last_pokemon(evaluate);
	
	std::cout << "Evaluate tests passed.\n\n";
}

}	// namespace technicalmachine
