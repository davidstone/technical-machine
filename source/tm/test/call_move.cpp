// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto damage = ActualDamage::Unknown{};	

constexpr auto regular_moves(auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

TEST_CASE("Baton Pass", "[call_move]") {
	auto weather = Weather();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Smeargle,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Own_Tempo,
			default_combined_stats<generation>,
			regular_moves(Moves::Baton_Pass, Moves::Belly_Drum)
		),
		Pokemon<generation>(
			Species::Alakazam,
			Level(100_bi),
			Gender::male,
			Item::Lum_Berry,
			Ability::Synchronize,
			CombinedStats<generation>{
				Nature::Hardy,
				max_dvs_or_ivs<generation>,
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
		)
	});
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Gengar,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
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
		),
		Pokemon<generation>(
			Species::Misdreavus,
			Level(100_bi),
			Gender::female,
			Item::Choice_Specs,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
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
		)
	});
	defender.pokemon().switch_in(weather);

	attacker.reset_start_of_turn();

	CHECK(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(Moves::Belly_Drum, attacker.pokemon().as_const(), defender, weather);
		CHECK(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		CHECK(side_effect.probability == 1.0);

		call_move(
			attacker,
			UsedMove<Team<generation>>(Moves::Belly_Drum, side_effect.function),
			defender,
			FutureMove{false},
			weather,
			false,
			damage
		);
	}
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);

	attacker.reset_start_of_turn();

	CHECK(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Baton_Pass, Moves::Belly_Drum, Moves::Switch1})
	);

	call_move(
		attacker,
		UsedMove<Team<generation>>(Moves::Baton_Pass, no_effect_function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(Moves::Switch1, attacker.pokemon().as_const(), defender, weather);
		CHECK(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		CHECK(side_effect.probability == 1.0);
		call_move(
			attacker,
			UsedMove<Team<generation>>(Moves::Switch1, side_effect.function),
			defender,
			FutureMove{false},
			weather,
			false,
			damage
		);
	}
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(attacker.pokemon().species() == Species::Alakazam);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, weather) ==
		StaticVectorMove({Moves::Pass})
	);
}

TEST_CASE("Wonder Guard", "[call_move]") {
	auto weather = Weather();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(Moves::Shadow_Ball, Moves::Thunderbolt)
		)
	});
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Shedinja,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Wonder_Guard,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Tackle)})
		)
	});
	auto shedinja = defender.pokemon();
	shedinja.switch_in(weather);

	CHECK(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<Team<generation>>(Moves::Thunderbolt, no_effect_function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	CHECK(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<Team<generation>>(Moves::Shadow_Ball, no_effect_function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);
	CHECK(shedinja.hp().current() == 0_bi);
}

TEST_CASE("Fire move thaws target", "[call_move]") {
	auto weather = Weather();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Charmander,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Blaze,
			default_combined_stats<generation>,
			regular_moves(Moves::Ember)
		)
	});
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Vaporeon,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Water_Absorb,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Tackle)})
		)
	});
	auto vaporeon = defender.pokemon();
	vaporeon.switch_in(weather);
	vaporeon.set_status(Statuses::freeze, weather);

	CHECK(vaporeon.status().name() == Statuses::freeze);

	constexpr auto move_name = Moves::Ember;
	auto const side_effects = possible_side_effects(move_name, attacker.pokemon().as_const(), defender, weather);
	CHECK(containers::size(side_effects) == 2_bi);

	{
		auto attacker_copy = attacker;
		auto defender_copy = defender;

		call_move(
			attacker_copy,
			UsedMove<Team<generation>>(move_name, side_effects[0_bi].function),
			defender_copy,
			FutureMove{false},
			weather,
			false,
			damage
		);

		CHECK(defender_copy.pokemon().status().name() == Statuses::clear);
	}

	call_move(
		attacker,
		UsedMove<Team<generation>>(move_name, side_effects[1_bi].function),
		defender,
		FutureMove{false},
		weather,
		false,
		damage
	);

	CHECK(vaporeon.status().name() == Statuses::burn);
}

TEST_CASE("Sleep Talk Substitute", "[call_move]") {
	auto weather = Weather();

	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Registeel,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Sleep_Talk), Move(generation, Moves::Substitute)})
		)
	});
	user.pokemon().switch_in(weather);
	user.pokemon().set_hp(weather, 5_bi);
	user.pokemon().rest(weather, false);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Tyranitar,
			Level(100_bi),
			Gender::male,
			Item::Choice_Band,
			Ability::Sand_Stream,
			default_combined_stats<generation>,
			regular_moves(Moves::Rock_Slide)
		)
	});
	other.pokemon().switch_in(weather);

	CHECK(user.pokemon().substitute().hp() == 0_bi);

	auto const side_effects = possible_side_effects(Moves::Substitute, user.pokemon().as_const(), other, weather);
	CHECK(containers::size(side_effects) == 1_bi);
	call_move(
		user,
		UsedMove<Team<generation>>(Moves::Sleep_Talk, Moves::Substitute, false, false, containers::front(side_effects).function),
		other,
		FutureMove{false},
		weather,
		false,
		damage
	);

	CHECK(user.pokemon().substitute().hp() == user.pokemon().hp().max() / 4_bi);
}

} // namespace
} // namespace technicalmachine
