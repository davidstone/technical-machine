// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/call_move.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>

#include <containers/front_back.hpp>
#include <containers/size.hpp>

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

	auto attacker = Team<generation>(2_bi, true);
	attacker.add_pokemon(Pokemon<generation>(
		Species::Smeargle,
		Level(100_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Own_Tempo,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		regular_moves(Moves::Baton_Pass, Moves::Belly_Drum)
	));
	attacker.add_pokemon(Pokemon<generation>(
		Species::Alakazam,
		Level(100_bi),
		Gender::male,
		Item::Lum_Berry,
		Ability::Synchronize,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(252_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		regular_moves(Moves::Psycho_Cut, Moves::Recover)
	));
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>(2_bi);
	defender.add_pokemon(Pokemon<generation>(
		Species::Gengar,
		Level(100_bi),
		Gender::male,
		Item::Choice_Specs,
		Ability::Levitate,
		CombinedStats<IVAndEV>{
			Nature::Modest,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(252_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		regular_moves(Moves::Shadow_Ball)
	));
	defender.add_pokemon(Pokemon<generation>(
		Species::Misdreavus,
		Level(100_bi),
		Gender::female,
		Item::Choice_Specs,
		Ability::Levitate,
		CombinedStats<IVAndEV>{
			Nature::Modest,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(252_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		regular_moves(Moves::Shadow_Ball)
	));
	defender.pokemon().switch_in(weather);

	attacker.reset_start_of_turn();

	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(Moves::Belly_Drum, as_const(attacker.pokemon()), defender, weather);
		BOUNDED_ASSERT(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		BOUNDED_ASSERT(side_effect.probability == 1.0);

		call_move(
			attacker,
			UsedMove<generation>(Moves::Belly_Drum, side_effect.function),
			defender,
			FutureMove{false},
			weather,
			false,
			damage
		);
	}
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(!attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);

	attacker.reset_start_of_turn();

	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

	call_move(
		attacker,
		UsedMove<generation>(Moves::Baton_Pass, no_effect_function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(Moves::Switch1, as_const(attacker.pokemon()), defender, weather);
		BOUNDED_ASSERT(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		BOUNDED_ASSERT(side_effect.probability == 1.0);
		call_move(
			attacker,
			UsedMove<generation>(Moves::Switch1, side_effect.function),
			defender,
			FutureMove{false},
			weather,
			false,
			damage
		);
	}
	BOUNDED_ASSERT(attacker.pokemon().stage()[BoostableStat::atk] == 6_bi);
	BOUNDED_ASSERT(attacker.pokemon().species() == Species::Alakazam);
	BOUNDED_ASSERT(!attacker.pokemon().switch_decision_required());
	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);
}

void wonder_guard() {
	auto weather = Weather();

	auto attacker = Team<generation>(1_bi, true);
	attacker.add_pokemon(Pokemon<generation>(
		Species::Jolteon,
		Level(100_bi),
		Gender::female,
		Item::None,
		Ability::Volt_Absorb,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		regular_moves(Moves::Shadow_Ball, Moves::Thunderbolt)
	));
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>(1_bi);
	defender.add_pokemon(Pokemon<generation>(
		Species::Shedinja,
		Level(100_bi),
		Gender::male,
		Item::None,
		Ability::Wonder_Guard,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto shedinja = defender.pokemon();
	shedinja.switch_in(weather);

	BOUNDED_ASSERT(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<generation>(Moves::Thunderbolt, no_effect_function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	BOUNDED_ASSERT(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<generation>(Moves::Shadow_Ball, no_effect_function),
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
