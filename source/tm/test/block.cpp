// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/block.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/string_conversions/move.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

constexpr auto generation = Generation::four;
constexpr auto moves(auto... move_names) {
	return RegularMoves({Move(generation, move_names, 0_bi)...});
}

TEST_CASE("block with all legal moves", "[block]") {
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			moves(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball)
		)
	});
	auto weather = Weather();
	user.pokemon().switch_in(weather);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Gyarados,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt)
		)
	});
	other.pokemon().switch_in(weather);

	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	CHECK(selections == LegalSelections({Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball}));
}

auto empty_pp(Move & move) {
	auto remaining_pp = move.pp().remaining();
	if (remaining_pp) {
		move.reduce_pp(*remaining_pp);
	}
};

TEST_CASE("Two moves with one out of pp", "[block]") {
	auto weather = Weather();

	auto user_moves = moves(Moves::Thunder, Moves::Thunderbolt);
	empty_pp(containers::front(user_moves));
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			user_moves
		)
	});
	user.pokemon().switch_in(weather);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(Moves::Thunder, Moves::Thunderbolt)
		)
	});
	other.pokemon().switch_in(weather);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	CHECK(selections == LegalSelections({Moves::Thunderbolt}));
}

TEST_CASE("Two moves with both out of pp", "[block]") {
	auto weather = Weather();

	auto user_moves = moves(Moves::Thunder, Moves::Thunderbolt);
	for (auto & move : user_moves) {
		empty_pp(move);
	}
	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			user_moves
		)
	});
	user.pokemon().switch_in(weather);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Pikachu,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Intimidate,
			default_combined_stats<generation>,
			moves(Moves::Thunder, Moves::Thunderbolt)
		)
	});
	other.pokemon().switch_in(weather);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	CHECK(selections == LegalSelections({Moves::Struggle}));
}

TEST_CASE("Replace fainted", "[block]") {
	auto weather = Weather{};

	auto team = Team<generation>({
		Pokemon<generation>(
			Species::Slugma,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Magma_Armor,
			default_combined_stats<generation>,
			moves(Moves::Flamethrower)
		),
		Pokemon<generation>(
			Species::Zapdos,
			Level(100_bi),
			Gender::genderless,
			Item::Choice_Specs,
			Ability::Pressure,
			default_combined_stats<generation>,
			moves(Moves::Thunderbolt)
		)
	});
	team.pokemon().switch_in(weather);
	team.reset_start_of_turn();

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			default_combined_stats<generation>,
			moves(Moves::Surf)
		)
	});
	other.pokemon().switch_in(weather);

	other.reset_start_of_turn();

	faint(team.pokemon());

	auto const expected = LegalSelections({Moves::Switch1});
	auto const selections = legal_selections(team, other, weather);
	CHECK(selections == expected);
}

} // namespace
} // namespace technicalmachine
