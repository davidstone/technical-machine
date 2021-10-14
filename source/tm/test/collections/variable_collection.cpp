// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/collection.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <tm/empty_team.hpp>
#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/array/make_array.hpp>
#include <containers/integer_range.hpp>
#include <containers/size.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

using bounded::to_string;

constexpr auto generation = Generation::four;

void add_pokemon(Team<generation> & team, Species const species) {
	auto const level = Level(100_bi);
	team.add_pokemon(Pokemon<generation>(species, level, Gender::male));
	team.pokemon().set_ability_to_base_ability();
}

using EffectIndex = bounded::integer<0, 4>;
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
	CHECK(expected == calculated);
}

TEST_CASE("Side effects", "[side effects]") {
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
			auto const side_effects = possible_side_effects(Moves::Whirlwind, user.pokemon().as_const(), team, weather);
			auto const expected_size = foe_size - 1_bi;
			CHECK(containers::size(side_effects) == expected_size);
			for (auto const effect_index : containers::integer_range(expected_size)) {
				auto const & side_effect = side_effects[effect_index];
				CHECK(side_effect.probability == 1.0 / double(foe_size - 1_bi));
				team.all_pokemon().set_index(current_index);
				side_effect.function(user, team, weather, 0_bi);
				validate(team, EffectIndex(effect_index), current_index);
			}
		}
	}
}

} // namespace
} // namespace technicalmachine
