// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/evaluate/evaluate.hpp>
#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/moves.hpp>
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

struct log_out {
	using char_type = char;
	using category = boost::iostreams::sink_tag;

	auto write(char const * const ptr, std::streamsize const size) -> std::streamsize {
		INFO(std::string_view(ptr, static_cast<std::size_t>(size)));
		return size;
	}
};

template<Generation generation>
auto test_expectiminimax(Team<generation> const & ai, Team<generation> const & foe, Weather const weather, Evaluate<generation> const evaluate, Depth const depth) -> BestMove {
	auto log_buffer = log_out();
	auto log = boost::iostreams::stream<log_out>(log_buffer);
	return expectiminimax(ai, foe, weather, evaluate, depth, log);
}

constexpr auto make_depth(DepthInt const depth) {
	return Depth(DepthValues{depth, 0_bi}, 0_bi);
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
			regular_moves(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)
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
			regular_moves(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt)
		)
	});
	team2.pokemon().switch_in(weather);
	team2.reset_start_of_turn();

	{
		auto const best_move = test_expectiminimax(team1, team2, weather, evaluate, depth);
		CHECK(best_move.name == Moves::Thunderbolt);
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
			regular_moves(Moves::Swords_Dance, Moves::X_Scissor, Moves::Shadow_Sneak, Moves::Will_O_Wisp)
		)
	});
	team3.pokemon().switch_in(weather);
	team3.reset_start_of_turn();
	
	{
		auto const best_move = test_expectiminimax(team1, team3, weather, evaluate, depth);
		CHECK(best_move.name == Moves::Shadow_Ball);
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
			regular_moves(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)
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
			regular_moves(Moves::Surf, Moves::Ice_Beam)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Shadow_Ball);
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
			regular_moves(Moves::Fire_Punch, Moves::Belly_Drum, Moves::Earthquake, Moves::Double_Edge)
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
			regular_moves(Moves::Soft_Boiled)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Belly_Drum);
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
			regular_moves(Moves::Curse, Moves::Crunch)
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
			regular_moves(Moves::Counter)
		)
	});
	defender.pokemon().switch_in(weather);
	defender.reset_start_of_turn();

	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Curse);
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
	constexpr auto depth = Depth(DepthValues{4_bi, 0_bi}, 0_bi);

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
			regular_moves(Moves::Psycho_Cut, Moves::Recover)
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

	defender.reset_start_of_turn();

	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Belly_Drum);
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
			regular_moves(Moves::Tackle)
		),
		Pokemon<generation>(
			Species::Slugma,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Magma_Armor,
			default_combined_stats<generation>,
			regular_moves(Moves::Flamethrower, Moves::Earth_Power)
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
			regular_moves(Moves::Thunderbolt)
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
			regular_moves(Moves::Calm_Mind, Moves::Surf, Moves::Ice_Beam)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	{
		constexpr auto move_name = Moves::Surf;
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
			ActualDamage::Unknown{}
		);
	}
	
	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Switch2);
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
			regular_moves(Moves::Calm_Mind, Moves::Dragon_Pulse, Moves::Recover)
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
			regular_moves(Moves::Ice_Beam, Moves::Rest)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();

	auto const best_move = test_expectiminimax(attacker, defender, weather, evaluate, depth);
	CHECK(best_move.name == Moves::Calm_Mind);
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
			regular_moves(Moves::Sleep_Talk, Moves::Thunderbolt)
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
			regular_moves(Moves::Earthquake)
		)
	});
	defender.pokemon().switch_in(weather);

	defender.reset_start_of_turn();
	
	constexpr auto keep_status = false;
	constexpr auto unknown_damage = ActualDamage::Unknown{};
	constexpr auto sleep_talk = UsedMove<Team<generation>>(
		Moves::Sleep_Talk,
		no_effect_function
	);
	constexpr auto thunderbolt = UsedMove<Team<generation>>(
		Moves::Thunderbolt,
		no_effect_function
	);
	constexpr auto other_move = FutureMove{false};
	
	auto next_turn = [&]{
		constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false);
		end_of_turn(attacker, end_of_turn_flags, defender, end_of_turn_flags, weather);
		attacker.reset_start_of_turn();
		defender.reset_start_of_turn();
	};

	auto jolteon = attacker.pokemon();
	
	// TODO: Validate score, too

	CHECK(jolteon.status().name() == Statuses::clear);
	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, depth).name == Moves::Thunderbolt);

	call_move(attacker, sleep_talk, defender, other_move, weather, keep_status, unknown_damage);
	jolteon.set_status(Statuses::sleep, weather);
	next_turn();
	CHECK(jolteon.status().name() == Statuses::sleep);
	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, depth).name == Moves::Sleep_Talk);

	call_move(attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
	next_turn();
	CHECK(jolteon.status().name() == Statuses::sleep);
	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, depth).name == Moves::Sleep_Talk);

	call_move(attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
	next_turn();
	CHECK(jolteon.status().name() == Statuses::sleep);
	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, depth).name == Moves::Sleep_Talk);

	#if 0
		// Same probability of either move
		call_move(attacker, thunderbolt, defender, other_move, weather, keep_status, unknown_damage);
		next_turn();
		CHECK(jolteon.status().name() == Statuses::sleep);
		CHECK(test_expectiminimax(attacker, defender, weather, evaluate, depth).name == ?);
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
			regular_moves(Moves::Psychic, Moves::Recover, Moves::Thunder_Wave, Moves::Seismic_Toss)
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
			regular_moves(Moves::Explosion, Moves::Hypnosis, Moves::Thunderbolt, Moves::Night_Shade)
		)
	});
	defender.pokemon().set_status(Statuses::freeze, weather);
	defender.pokemon().set_hp(weather, 12_bi);
	defender.pokemon().switch_in(weather);
	
	attacker.reset_start_of_turn();
	defender.reset_start_of_turn();

	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, make_depth(1_bi)).name == Moves::Psychic);
	CHECK(test_expectiminimax(attacker, defender, weather, evaluate, make_depth(2_bi)).name == Moves::Psychic);
}

} // namespace
} // namespace technicalmachine
