// Test checked collections of random move effects
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

#include <tm/test/collections/variable_collection.hpp>

#include <tm/test/collections/invalid_collection.hpp>

#include <tm/empty_team.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <bounded/assert.hpp>

#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

using bounded::to_string;

constexpr auto generation = Generation::four;

void add_pokemon(Team & team, Species const species) {
	auto const level = Level(100_bi);
	team.add_pokemon(Pokemon(generation, species, level, Gender::male));
	team.pokemon().set_ability_to_base_ability();
}

void phaze_in_same_pokemon(Team const & team) {
	try {
		auto variable = Variable{};
		variable.set_phaze_index(team, team.pokemon().species());
		throw InvalidCollection("Can phaze in the same Pokemon.");
	} catch (PhazingInSamePokemon const &) {
		// Do nothing; the above operation should throw.
	}
}

void phaze_in_different_pokemon(Team const & team, containers::index_type<PokemonCollection> const new_index, containers::index_type<PokemonCollection> const current_index, TeamSize foe_size) {
	static constexpr auto expected_index = containers::make_explicit_array<6, 6>(
		bounded::none, 1_bi, 2_bi, 3_bi, 4_bi, 5_bi,
		0_bi, bounded::none, 2_bi, 3_bi, 4_bi, 5_bi,
		0_bi, 1_bi, bounded::none, 3_bi, 4_bi, 5_bi,
		0_bi, 1_bi, 2_bi, bounded::none, 4_bi, 5_bi,
		0_bi, 1_bi, 2_bi, 3_bi, bounded::none, 5_bi,
		0_bi, 1_bi, 2_bi, 3_bi, 4_bi, bounded::none
	);
	auto variable = Variable{};
	variable.set_phaze_index(team, team.pokemon(new_index).species());
	auto const expected = expected_index[current_index][new_index];
	BOUNDED_ASSERT(expected);
	auto const calculated = variable.phaze_index();
	if (calculated != *expected) {
		throw InvalidCollection("Offsets for phazing are incorrect. Expected " + to_string(*expected) + " but got a result of " + to_string(calculated) + ".");
	}
	if (new_index == foe_size) {
		throw InvalidCollection("Phazing supports too many elements when the foe's size is " + to_string(foe_size) + ".");
	}
}

void test_combinations(Team team) {
	for (auto const foe_size : containers::integer_range(2_bi, max_pokemon_per_team)) {
		add_pokemon(team, static_cast<Species>(foe_size));
		auto collection = all_probabilities(generation, Moves::Whirlwind, foe_size);
		auto const expected = foe_size - 1_bi;
		if (size(collection) != expected) {
			throw InvalidCollection("Phazing size is incorrect. Expected: " + to_string(expected) + " but got " + to_string(size(collection)));
		}
		for (auto const current_index : containers::integer_range(foe_size)) {
			team.all_pokemon().set_index(current_index);
			for (auto const new_index : containers::integer_range(foe_size)) {
				if (current_index == new_index) {
					phaze_in_same_pokemon(team);
				} else {
					phaze_in_different_pokemon(team, new_index, current_index, foe_size);
				}
			}
		}
	}
}

}	// namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	Team team(max_pokemon_per_team);
	add_pokemon(team, static_cast<Species>(1));
	test_combinations(std::move(team));
}

}	// namespace technicalmachine
