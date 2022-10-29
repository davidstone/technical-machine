// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/evaluate/evaluate.hpp>
#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/move_name.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/end_of_turn.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/front_back.hpp>
#include <containers/legacy_iterator.hpp>
#include <containers/size.hpp>

#include <boost/iostreams/stream.hpp>

#include <random>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto make_depth(DepthInt const depth) {
	return Depth(depth, 0_bi);
}

auto shuffled_regular_moves(Generation const generation, auto & random_engine, auto... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto moves = RegularMoves{Move(generation, ts)...};
	std::shuffle(containers::legacy_iterator(containers::begin(moves)), containers::legacy_iterator(containers::end(moves)), random_engine);
	return moves;
}

TEST_CASE("expectiminimax OHKO", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1_bi);

	auto team1 = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(MoveName::Thunderbolt, MoveName::Charm, MoveName::Thunder, MoveName::Shadow_Ball)
		)
	}, true);
	team1.pokemon().switch_in(weather);
	team1.reset_start_of_turn();

	auto team2 = Team<generation>({
		Pokemon<generation>(
			Species::Gyarados,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Dragon_Dance, MoveName::Waterfall, MoveName::Stone_Edge, MoveName::Taunt)
		)
	});
	team2.pokemon().switch_in(weather);
	team2.reset_start_of_turn();

	{
		auto const best_move = expectiminimax(team1, team2, weather, evaluate, depth);
		CHECK(best_move.name == MoveName::Thunderbolt);
		CHECK(best_move.score == victory<generation>);
	}
	
	auto team3 = Team<generation>({
		Pokemon<generation>(
			Species::Shedinja,
			Level(100_bi),
			Gender::male,
			Item::Lum_Berry,
			Ability::Wonder_Guard,
			default_combined_stats<generation>,
			regular_moves(MoveName::Swords_Dance, MoveName::X_Scissor, MoveName::Shadow_Sneak, MoveName::Will_O_Wisp)
		)
	});
	team3.pokemon().switch_in(weather);
	team3.reset_start_of_turn();
	
	{
		auto const best_move = expectiminimax(team1, team3, weather, evaluate, depth);
		CHECK(best_move.name == MoveName::Shadow_Ball);
		CHECK(best_move.score == victory<generation>);
	}
}

TEST_CASE("expectiminimax one-turn damage", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1_bi);
	
	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Volt_Absorb,
			CombinedStats<generation>{
				Nature::Hardy,
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
			regular_moves(MoveName::Thunderbolt, MoveName::Charm, MoveName::Thunder, MoveName::Shadow_Ball)
		)
	}, true);
	attacker.pokemon().switch_in(weather);
	attacker.reset_start_of_turn();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Swampert,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Torrent,
			default_combined_stats<generation>,
			regular_moves(MoveName::Surf, MoveName::Ice_Beam)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Shadow_Ball);
}

TEST_CASE("expectiminimax BellyZard", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(2_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Charizard,
			Level(100_bi),
			Gender::male,
			Item::Salac_Berry,
			Ability::Blaze,
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
			regular_moves(MoveName::Fire_Punch, MoveName::Belly_Drum, MoveName::Earthquake, MoveName::Double_Edge)
		)
	}, true);
	attacker.pokemon().switch_in(weather);
	attacker.reset_start_of_turn();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Mew,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Synchronize,
			CombinedStats<generation>{
				Nature::Hardy,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Soft_Boiled)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Belly_Drum);
	CHECK(best_move.score == victory<generation>);
}

TEST_CASE("expectiminimax Hippopotas vs Wobbuffet", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(11_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Hippopotas,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Sand_Stream,
			CombinedStats<generation>{
				Nature::Adamant,
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
			regular_moves(MoveName::Curse, MoveName::Crunch)
		)
	}, true);
	attacker.pokemon().switch_in(weather);
	attacker.reset_start_of_turn();

	// TODO: Implement Encore's effect ending when PP runs out, then Wobbuffet
	// can have Encore

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Wobbuffet,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Shadow_Tag,
			CombinedStats<generation>{
				Nature::Hardy,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Counter)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Curse);
	CHECK(best_move.score == victory<generation>);
}


TEST_CASE("expectiminimax Baton Pass", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = Depth(4_bi, 0_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Smeargle,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Own_Tempo,
			default_combined_stats<generation>,
			regular_moves(MoveName::Baton_Pass, MoveName::Belly_Drum)
		),
		Pokemon<generation>(
			Species::Alakazam,
			Level(100_bi),
			Gender::male,
			Item::Lum_Berry,
			Ability::Synchronize,
			CombinedStats<generation>{
				Nature::Adamant,
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
			regular_moves(MoveName::Psycho_Cut, MoveName::Recover)
		)
	}, true);
	attacker.pokemon().switch_in(weather);
	attacker.reset_start_of_turn();

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
			regular_moves(MoveName::Shadow_Ball)
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
			regular_moves(MoveName::Shadow_Ball)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Belly_Drum);
	CHECK(best_move.score == victory<generation>);
}


TEST_CASE("expectiminimax replace fainted", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(2_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Magikarp,
			Level(5_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Swift_Swim,
			default_combined_stats<generation>,
			regular_moves(MoveName::Tackle)
		),
		Pokemon<generation>(
			Species::Slugma,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Magma_Armor,
			default_combined_stats<generation>,
			regular_moves(MoveName::Flamethrower, MoveName::Earth_Power)
		),
		Pokemon<generation>(
			Species::Zapdos,
			Level(100_bi),
			Gender::genderless,
			Item::Choice_Specs,
			Ability::Pressure,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				default_evs<generation>
			},
			regular_moves(MoveName::Thunderbolt)
		)
	}, true);
	attacker.pokemon().switch_in(weather);

	attacker.reset_start_of_turn();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			default_combined_stats<generation>,
			regular_moves(MoveName::Calm_Mind, MoveName::Surf, MoveName::Ice_Beam)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	{
		constexpr auto move_name = MoveName::Surf;
		auto const side_effects = possible_side_effects(move_name, defender.pokemon().as_const(), attacker, weather);
		REQUIRE(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		call_move(
			defender,
			UsedMove<Team<generation>>(
				move_name,
				side_effect.function
			),
			attacker,
			FutureMove{false},
			weather,
			false,
			ActualDamage::Unknown{},
			false
		);
	}

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Switch2);
}


TEST_CASE("expectiminimax Latias vs Suicune", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto const weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(3_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Latias,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Calm,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(120_bi),
					EV(136_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Calm_Mind, MoveName::Dragon_Pulse, MoveName::Recover)
		)
	}, true);
	attacker.pokemon().switch_in(weather);

	attacker.reset_start_of_turn();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			CombinedStats<generation>{
				Nature::Calm,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(120_bi),
					EV(136_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Ice_Beam, MoveName::Rest)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	auto const best_move = expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == MoveName::Calm_Mind);
}

TEST_CASE("expectiminimax Sleep Talk", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	auto const evaluate = Evaluate<generation>();
	auto weather = Weather();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};
	constexpr auto depth = make_depth(1_bi);

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(MoveName::Sleep_Talk, MoveName::Thunderbolt)
		)
	}, true);
	attacker.pokemon().switch_in(weather);
	attacker.reset_start_of_turn();

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Gyarados,
			Level(100_bi),
			Gender::male,
			Item::Life_Orb,
			Ability::Intimidate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Earthquake)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	constexpr auto keep_status = false;
	constexpr auto unknown_damage = ActualDamage::Unknown{};
	constexpr auto sleep_talk = UsedMove<Team<generation>>(
		MoveName::Sleep_Talk,
		no_effect_function
	);
	constexpr auto thunderbolt = UsedMove<Team<generation>>(
		MoveName::Thunderbolt,
		no_effect_function
	);
	constexpr auto other_move = FutureMove{false};

	auto next_turn = [&] {
		constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);
		end_of_turn(attacker, end_of_turn_flags, defender, end_of_turn_flags, weather);
		attacker.reset_start_of_turn();
		defender.reset_start_of_turn();
	};

	auto jolteon = attacker.pokemon();

	// TODO: Validate score, too

	CHECK(jolteon.status().name() == StatusName::clear);
	CHECK(expectiminimax(attacker, defender, weather, evaluate, depth).name == MoveName::Thunderbolt);

	call_move(
		attacker,
		sleep_talk,
		defender,
		other_move,
		weather,
		keep_status,
		unknown_damage,
		false
	);
	jolteon.set_status(StatusName::sleep, weather);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(expectiminimax(attacker, defender, weather, evaluate, depth).name == MoveName::Sleep_Talk);

	call_move(
		attacker,
		thunderbolt,
		defender,
		other_move,
		weather,
		keep_status,
		unknown_damage,
		false
	);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(expectiminimax(attacker, defender, weather, evaluate, depth).name == MoveName::Sleep_Talk);

	call_move(
		attacker,
		thunderbolt,
		defender,
		other_move,
		weather,
		keep_status,
		unknown_damage,
		false
	);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(expectiminimax(attacker, defender, weather, evaluate, depth).name == MoveName::Sleep_Talk);

	#if 0
		// Same probability of either move
		call_move(
			attacker,
			thunderbolt,
			defender,
			other_move,
			weather,
			keep_status,
			unknown_damage,
			false
		);
		next_turn();
		CHECK(jolteon.status().name() == StatusName::sleep);
		CHECK(expectiminimax(attacker, defender, weather, evaluate, depth).name == ?);
	#endif
}

TEST_CASE("Generation 1 frozen last Pokemon", "[expectiminimax]") {
	constexpr auto generation = Generation::one;
	auto const evaluate = Evaluate<generation>();
	auto weather = Weather();
	auto regular_moves = [](auto const ... name) {
		return RegularMoves{Move(generation, name)...};
	};

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Alakazam,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			regular_moves(MoveName::Psychic, MoveName::Recover, MoveName::Thunder_Wave, MoveName::Seismic_Toss)
		)
	}, true);
	attacker.pokemon().switch_in(weather);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Gengar,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			regular_moves(MoveName::Explosion, MoveName::Hypnosis, MoveName::Thunderbolt, MoveName::Night_Shade)
		)
	});
	defender.pokemon().set_status(StatusName::freeze, weather);
	defender.pokemon().set_hp(weather, 12_bi);
	defender.pokemon().switch_in(weather);

	attacker.reset_start_of_turn();
	defender.reset_start_of_turn();

	CHECK(expectiminimax(attacker, defender, weather, evaluate, make_depth(1_bi)).name == MoveName::Psychic);
	CHECK(expectiminimax(attacker, defender, weather, evaluate, make_depth(2_bi)).name == MoveName::Psychic);
}

} // namespace
} // namespace technicalmachine
