// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/block.hpp>

#include <tm/move/moves.hpp>
#include <tm/pokemon/species.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <containers/front_back.hpp>
#include <containers/size.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

constexpr auto generation = Generation::four;
constexpr auto moves(auto... move_names) {
	return RegularMoves({Move(generation, move_names, 0_bi)...});
}

void basic() {
	auto const expected = moves(Moves::Thunderbolt, Moves::Charm, Moves::Thunder, Moves::Shadow_Ball);
	auto user = Team<generation>(1_bi, true);
	user.add_pokemon(Pokemon<generation>(
		Species::Jolteon,
		Level(100_bi),
		Gender::male,
		Item::Leftovers,
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
		expected
	));
	auto weather = Weather();
	user.pokemon().switch_in(weather);

	auto other = Team<generation>(1_bi, false);
	other.add_pokemon(Pokemon<generation>(
		Species::Gyarados,
		Level(100_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Intimidate,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		moves(Moves::Dragon_Dance, Moves::Waterfall, Moves::Stone_Edge, Moves::Taunt)
	));
	other.pokemon().switch_in(weather);

	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	if (!containers::equal(selections, expected)) {
		for (auto const & move : selections) {
			std::cerr << to_string(move) << '\n';
		}
		throw std::runtime_error("Invalid legal selections");
	}
}

auto empty_pp(Move & move) {
	while (!move.pp().is_empty()) {
		move.decrement_pp(Ability::Static);
	}
};

void test_two_moves_with_one_out_of_pp() {
	auto weather = Weather();

	auto user = Team<generation>(1_bi, true);
	auto & pokemon = user.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Intimidate,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		moves(Moves::Thunder, Moves::Thunderbolt)
	));
	empty_pp(containers::front(pokemon.regular_moves()));
	user.pokemon().switch_in(weather);

	auto other = Team<generation>(1_bi, false);
	other.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Intimidate,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		moves(Moves::Thunder, Moves::Thunderbolt)
	));
	other.pokemon().switch_in(weather);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	if (containers::size(selections) != 1_bi) {
		throw std::runtime_error("Incorrect number of selections with one of two moves out of PP. Expected 1, got " + to_string(containers::size(selections)));
	}
	if (containers::front(selections) != Moves::Thunderbolt) {
		throw std::runtime_error("Incorrect legal selection with one of two moves out of PP. Expected Thunderbolt, got " + std::string(to_string(containers::front(selections))));
	}
}

void test_two_moves_with_both_out_of_pp() {
	auto weather = Weather();

	auto user = Team<generation>(1_bi, true);
	auto & pokemon = user.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Intimidate,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		moves(Moves::Thunder, Moves::Thunderbolt)
	));
	user.pokemon().switch_in(weather);
	for (auto & move : pokemon.regular_moves()) {
		empty_pp(move);
	}

	auto other = Team<generation>(1_bi, false);
	other.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Intimidate,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		moves(Moves::Thunder, Moves::Thunderbolt)
	));
	other.pokemon().switch_in(weather);
	
	user.reset_start_of_turn();

	auto const selections = legal_selections(user, other, weather);
	if (containers::size(selections) != 1_bi) {
		throw std::runtime_error("Incorrect number of selections with two of two moves out of PP. Expected 1, got " + to_string(containers::size(selections)));
	}
	if (containers::front(selections) != Moves::Struggle) {
		throw std::runtime_error("Incorrect legal selection with two of two moves out of PP. Expected Struggle, got " + std::string(to_string(containers::front(selections))));
	}
}

}	// namespace

void block_tests() {
	std::cout << "Running block tests.\n";
	basic();
	test_two_moves_with_one_out_of_pp();
	test_two_moves_with_both_out_of_pp();
}

}	// namespace technicalmachine
