// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/collection.hpp>

#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <tm/phazing_in_same_pokemon.hpp>
#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/array.hpp>
#include <containers/integer_range.hpp>
#include <containers/size.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

using bounded::to_string;

constexpr auto generation = Generation::four;

auto make_team(auto const... species) {
	return Team<generation>({
		Pokemon<generation>(
			species,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, Moves::Tackle)})
		)...
	});
}

using EffectIndex = bounded::integer<0, 4>;
template<Generation generation>
void validate(Team<generation> const & team, EffectIndex const effect_index, TeamIndex const current_index) {
	static constexpr auto expected_index = containers::array<bounded::integer<0, 5>, 6_bi, 5_bi>{{
		{1_bi, 2_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 2_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 3_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 3_bi, 4_bi}
	}};

	auto const expected = expected_index[current_index][effect_index];
	auto const calculated = team.all_pokemon().index();
	CHECK(expected == calculated);
}

auto test_phaze(Team<generation> user, Team<generation> team) {
	auto weather = Weather();
	for (auto const current_index : containers::integer_range(team.size())) {
		team.all_pokemon().set_index(current_index);
		auto const side_effects = possible_side_effects(Moves::Whirlwind, user.pokemon().as_const(), team, weather);
		auto const expected_size = bounded::increase_min<0>(team.size() - 1_bi);
		CHECK(containers::size(side_effects) == expected_size);
		for (auto const effect_index : containers::integer_range(expected_size)) {
			auto const & side_effect = side_effects[effect_index];
			CHECK(side_effect.probability == 1.0 / double(team.size() - 1_bi));
			team.all_pokemon().set_index(current_index);
			side_effect.function(user, team, weather, 0_bi);
			validate(team, EffectIndex(effect_index), current_index);
		}
	}
}

TEST_CASE("Phaze 2 Pokemon", "[side effects]") {
	test_phaze(
		make_team(Species::Lugia),
		make_team(Species::Bulbasaur, Species::Ivysaur)
	);
}
TEST_CASE("Phaze 3 Pokemon", "[side effects]") {
	test_phaze(
		make_team(Species::Lugia),
		make_team(Species::Bulbasaur, Species::Ivysaur, Species::Venusaur)
	);
}
TEST_CASE("Phaze 4 Pokemon", "[side effects]") {
	test_phaze(
		make_team(Species::Lugia),
		make_team(Species::Bulbasaur, Species::Ivysaur, Species::Venusaur, Species::Charmander)
	);
}
TEST_CASE("Phaze 5 Pokemon", "[side effects]") {
	test_phaze(
		make_team(Species::Lugia),
		make_team(Species::Bulbasaur, Species::Ivysaur, Species::Venusaur, Species::Charmander, Species::Charmeleon)
	);
}
TEST_CASE("Phaze 6 Pokemon", "[side effects]") {
	test_phaze(
		make_team(Species::Lugia),
		make_team(Species::Bulbasaur, Species::Ivysaur, Species::Venusaur, Species::Charmander, Species::Charmeleon, Species::Charizard)
	);
}

} // namespace
} // namespace technicalmachine
