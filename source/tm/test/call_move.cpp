// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/call_move.hpp>

#include <tm/test/incorrect_calculation.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

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
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		regular_moves(Moves::Baton_Pass, Moves::Belly_Drum)
	));
	attacker.add_pokemon(Pokemon<generation>(
		Species::Alakazam,
		Level(100_bi),
		Gender::male,
		Item::Lum_Berry,
		Ability::Synchronize,
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
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
		CombinedStats<generation>{
			Nature::Modest,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		regular_moves(Moves::Shadow_Ball)
	));
	defender.add_pokemon(Pokemon<generation>(
		Species::Misdreavus,
		Level(100_bi),
		Gender::female,
		Item::Choice_Specs,
		Ability::Levitate,
		CombinedStats<generation>{
			Nature::Modest,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(252_bi),
				EV(0_bi),
				EV(0_bi)
			)
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
		auto const side_effects = possible_side_effects(Moves::Belly_Drum, attacker.pokemon().as_const(), defender, weather);
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
	BOUNDED_ASSERT(!switch_decision_required(attacker));
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
	BOUNDED_ASSERT(switch_decision_required(attacker));
	BOUNDED_ASSERT(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(Moves::Switch1, attacker.pokemon().as_const(), defender, weather);
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
	BOUNDED_ASSERT(!switch_decision_required(attacker));
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
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
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
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
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

void fire_move_thaws_target() {
	auto weather = Weather();

	auto attacker = Team<generation>(1_bi, true);
	attacker.add_pokemon(Pokemon<generation>(
		Species::Charmander,
		Level(100_bi),
		Gender::female,
		Item::None,
		Ability::Blaze,
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		regular_moves(Moves::Ember)
	));
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>(1_bi);
	defender.add_pokemon(Pokemon<generation>(
		Species::Vaporeon,
		Level(100_bi),
		Gender::male,
		Item::None,
		Ability::Water_Absorb,
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi),
				IV(31_bi)
			),
			EVs(
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi),
				EV(0_bi)
			)
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	auto vaporeon = defender.pokemon();
	vaporeon.switch_in(weather);
	vaporeon.set_status(Statuses::freeze, weather);

	BOUNDED_ASSERT(vaporeon.status().name() == Statuses::freeze);

	constexpr auto move_name = Moves::Ember;
	auto const side_effects = possible_side_effects(move_name, attacker.pokemon().as_const(), defender, weather);
	BOUNDED_ASSERT(containers::size(side_effects) == 2_bi);

	{
		auto attacker_copy = attacker;
		auto defender_copy = defender;

		call_move(
			attacker_copy,
			UsedMove<generation>(move_name, side_effects[0_bi].function),
			defender_copy,
			FutureMove{false},
			weather,
			false,
			damage
		);

		BOUNDED_ASSERT(defender_copy.pokemon().status().name() == Statuses::clear);
	}

	call_move(
		attacker,
		UsedMove<generation>(move_name, side_effects[1_bi].function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);

	BOUNDED_ASSERT(vaporeon.status().name() == Statuses::burn);
}

} // namespace

void call_move_tests() {
	std::cout << "Running call_move tests.\n";
	test_baton_pass();
	wonder_guard();
	fire_move_thaws_target();
	std::cout << "Use move tests passed.\n\n";
}

}	// namespace technicalmachine
