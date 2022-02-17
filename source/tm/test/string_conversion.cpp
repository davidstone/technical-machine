// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/type.hpp>
#include <tm/string_conversions/weather.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>
#include <tm/move/moves.hpp>
#include <tm/status.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

template<typename Enum>
void test_generic() {
	for (auto const original : containers::enum_range<Enum>()) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		CHECK(original == result);
	}
}

TEST_CASE("ability", "[string_conversion]") {
	test_generic<Ability>();
}
TEST_CASE("gender", "[string_conversion]") {
	test_generic<Gender>();
}
TEST_CASE("item", "[string_conversion]") {
	test_generic<Item>();
}
TEST_CASE("move", "[string_conversion]") {
	test_generic<Moves>();
}
TEST_CASE("nature", "[string_conversion]") {
	test_generic<Nature>();
}
TEST_CASE("species", "[string_conversion]") {
	test_generic<Species>();
}
TEST_CASE("status", "[string_conversion]") {
	test_generic<Statuses>();
}
TEST_CASE("type", "[string_conversion]") {
	test_generic<Type>();
}
TEST_CASE("weather", "[string_conversion]") {
	test_generic<NormalWeather>();
}

TEST_CASE("pokemon", "[string_conversion]") {
	constexpr auto generation = Generation::three;

	auto make_pokemon = [](RegularMoves const moves, Statuses const status = Statuses::clear) {
		auto pokemon = Pokemon<generation>(
			Species::Mewtwo,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(4_bi),
					EV(12_bi),
					EV(24_bi),
					EV(0_bi),
					EV(32_bi),
					EV(100_bi)
				)
			},
			moves
		);
		pokemon.set_status(status);
		return pokemon;
	};

	auto check = [](Pokemon<generation> const pokemon) {
		auto const str = to_string(pokemon);
		auto const result = pokemon_from_string<generation>(str);

		CHECK(pokemon == result);
	};

	auto moves = RegularMoves({Move(generation, Moves::Psychic)});
	check(make_pokemon(moves));
	for (auto const move : {Moves::Recover, Moves::Calm_Mind, Moves::Taunt}) {
		moves.push_back(Move(generation, move));
		check(make_pokemon(moves));
	}
	check(make_pokemon(moves, Statuses::burn));
}

} // namespace
} // namespace technicalmachine
