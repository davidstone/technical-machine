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

#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <tm/empty_team.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

using bounded::to_string;

constexpr auto generation = Generation::four;

void add_pokemon(Team<generation> & team, Species const species) {
	auto const level = Level(100_bi);
	team.add_pokemon(Pokemon<generation>(species, level, Gender::male));
	team.pokemon().set_ability_to_base_ability();
}

using EffectIndex = bounded::checked_integer<0, 4>;
template<Generation generation>
void validate(Team<generation> const & team, EffectIndex const effect_index, TeamIndex const current_index) {
	static constexpr auto expected_index = containers::make_explicit_array<6, 5>(
		1_bi, 2_bi, 3_bi, 4_bi, 5_bi,
		0_bi, 2_bi, 3_bi, 4_bi, 5_bi,
		0_bi, 1_bi, 3_bi, 4_bi, 5_bi,
		0_bi, 1_bi, 2_bi, 4_bi, 5_bi,
		0_bi, 1_bi, 2_bi, 3_bi, 5_bi,
		0_bi, 1_bi, 2_bi, 3_bi, 4_bi
	);

	auto const expected = expected_index[current_index][effect_index];
	auto const calculated = team.all_pokemon().index();
	if (expected != calculated) {
		throw InvalidCollection("Offsets for phazing are incorrect. Expected " + to_string(expected) + " but got a result of " + to_string(calculated) + ".");
	}
}

void test_combinations() {
	auto user = Team<generation>(1_bi);
	add_pokemon(user, Species::Lugia);
	auto weather = Weather();
	for (auto const foe_size : containers::integer_range(2_bi, max_pokemon_per_team)) {
		auto team = Team<generation>(foe_size);
		for (auto const species : containers::integer_range(foe_size)) {
			add_pokemon(team, static_cast<Species>(species));
		}
		for (auto const current_index : containers::integer_range(foe_size)) {
			team.all_pokemon().set_index(current_index);
			auto const side_effects = possible_side_effects(Moves::Whirlwind, as_const(user.pokemon()), team, weather);
			auto const expected_size = foe_size - 1_bi;
			if (size(side_effects) != expected_size) {
				throw InvalidCollection("Phazing size is incorrect. Expected: " + to_string(expected_size) + " but got " + to_string(size(side_effects)));
			}
			for (auto const effect_index : containers::integer_range(expected_size)) {
				auto const & side_effect = side_effects[effect_index];
				if (side_effect.probability != 1.0 / double(foe_size - 1_bi)) {
					throw std::runtime_error("");
				}
				team.all_pokemon().set_index(current_index);
				side_effect.function(user, team, weather, 0_bi);
				validate(team, EffectIndex(effect_index), current_index);
			}
		}
	}
}

}	// namespace

void variable_collection_tests() {
	std::cout << "\tRunning variable collection tests.\n";
	test_combinations();
}

}	// namespace technicalmachine
