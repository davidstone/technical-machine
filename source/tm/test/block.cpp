// Copyright (C) 2019 David Stone
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

#include <tm/test/block.hpp>

#include <tm/move/moves.hpp>
#include <tm/pokemon/species.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/block.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

constexpr auto generation = Generation::four;

void basic() {
	auto user = Team(1_bi, true);
	{
		auto & jolteon = user.add_pokemon(generation, Species::Jolteon, Level(100_bi), Gender::male, Item::Leftovers, Ability::Volt_Absorb, Nature::Timid);
		containers::append(
			regular_moves(jolteon),
			containers::array{
				Move(generation, Moves::Thunderbolt),
				Move(generation, Moves::Charm),
				Move(generation, Moves::Thunder),
				Move(generation, Moves::Shadow_Ball)
			}
		);
	}
	auto weather = Weather();
	user.pokemon().switch_in(generation, weather);

	auto other = Team(1_bi, false);
	{
		auto & gyarados = other.add_pokemon(generation, Species::Gyarados, Level(100_bi), Gender::male, Item::Leftovers, Ability::Intimidate, Nature::Adamant);
		containers::append(
			regular_moves(gyarados),
			containers::array{
				Move(generation, Moves::Dragon_Dance),
				Move(generation, Moves::Waterfall),
				Move(generation, Moves::Stone_Edge),
				Move(generation, Moves::Taunt)
			}
		);
	}
	other.pokemon().switch_in(generation, weather);

	auto const selections = legal_selections(generation, user, other.pokemon(), weather);
	if (size(selections) != 4_bi) {
		throw std::runtime_error("Invalid number of legal selections");
	}
}

void test_two_moves_with_one_out_of_pp() {
	auto user = Team(1_bi, true);
	auto & pokemon = user.add_pokemon(generation, Species::Pikachu, Level(100_bi), Gender::female);
	auto empty_pp = [](Move & move) {
		while (!move.pp().is_empty()) {
			move.decrement_pp(Ability::Static);
		}
	};
	auto weather = Weather();
	user.pokemon().switch_in(generation, weather);
	auto & thunder = all_moves(pokemon).add(Move(generation, Moves::Thunder, 0_bi));
	empty_pp(thunder);
	all_moves(pokemon).add(Move(generation, Moves::Thunderbolt, 0_bi));

	auto other = Team(1_bi, false);
	other.add_pokemon(generation, Species::Pikachu, Level(100_bi), Gender::female);
	other.pokemon().switch_in(generation, weather);
	
	auto const selections = legal_selections(generation, user, other.pokemon(), weather);
	if (size(selections) != 1_bi) {
		throw std::runtime_error("Incorrect number of selections with one of two moves out of PP. Expected 1, got " + to_string(size(selections)));
	}
	if (front(selections) != Moves::Thunderbolt) {
		throw std::runtime_error("Incorrect legal selection with one of two moves out of PP. Expected Thunderbolt, got " + std::string(to_string(front(selections))));
	}
}

void test_two_moves_with_both_out_of_pp() {
	auto user = Team(1_bi, true);
	auto & pokemon = user.add_pokemon(generation, Species::Pikachu, Level(100_bi), Gender::female);
	auto empty_pp = [](Move & move) {
		while (!move.pp().is_empty()) {
			move.decrement_pp(Ability::Static);
		}
	};
	auto weather = Weather();
	user.pokemon().switch_in(generation, weather);
	auto & thunder = all_moves(pokemon).add(Move(generation, Moves::Thunder, 0_bi));
	empty_pp(thunder);
	auto & thunderbolt = all_moves(pokemon).add(Move(generation, Moves::Thunderbolt, 0_bi));
	empty_pp(thunderbolt);

	auto other = Team(1_bi, false);
	other.add_pokemon(generation, Species::Pikachu, Level(100_bi), Gender::female);
	other.pokemon().switch_in(generation, weather);
	
	auto const selections = legal_selections(generation, user, other.pokemon(), Weather{});
	if (size(selections) != 1_bi) {
		throw std::runtime_error("Incorrect number of selections with two of two moves out of PP. Expected 1, got " + to_string(size(selections)));
	}
	if (front(selections) != Moves::Struggle) {
		throw std::runtime_error("Incorrect legal selection with two of two moves out of PP. Expected Struggle, got " + std::string(to_string(front(selections))));
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
