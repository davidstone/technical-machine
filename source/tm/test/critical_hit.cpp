// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/critical_hit.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/team.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

template<Generation generation>
auto individual_test(Species const species, MoveName const move_name, Item const item, bool const focus_energy, double const rate) {
	auto weather = Weather();
	constexpr auto ability = Ability::Honey_Gather;
	auto attacker = Team<generation>({
		Pokemon<generation>(
			species,
			Level(100_bi),
			Gender::genderless,
			item,
			ability,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	attacker.pokemon().switch_in(weather);
	if (focus_energy) {
		attacker.pokemon().focus_energy();
	}
	attacker.reset_start_of_turn();

	auto const ch_rate = critical_hit_probability(attacker.pokemon().as_const(), move_name, ability, weather);
	CHECK(ch_rate == rate);
}

TEST_CASE("Generation 1 Slowpoke Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Slowpoke, MoveName::Water_Gun, Item::None, true, 1.0 / 256.0);
}
TEST_CASE("Generation 1 Tauros Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Tauros, MoveName::Body_Slam, Item::None, false, 55.0 / 256.0);
}
TEST_CASE("Generation 1 Electrode Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Electrode, MoveName::Thunderbolt, Item::None, false, 70.0 / 256.0);
}
TEST_CASE("Generation 1 Electrode Slash Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Electrode, MoveName::Slash, Item::None, false, 255.0 / 256.0);
}
TEST_CASE("Generation 1 Sandslash Slash Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Sandslash, MoveName::Slash, Item::None, false, 255.0 / 256.0);
}
TEST_CASE("Generation 1 Goldeen Slash Critical Hit", "[Critical Hit]") {
	individual_test<Generation::one>(Species::Goldeen, MoveName::Slash, Item::None, false, 248.0 / 256.0);
}
TEST_CASE("Generation 2 Critical Hit", "[Critical Hit]") {
	individual_test<Generation::two>(Species::Pikachu, MoveName::Thunder, Item::None, false, 1.0 / 16.0);
}
TEST_CASE("Generation 2 Slash Critical Hit", "[Critical Hit]") {
	individual_test<Generation::two>(Species::Smeargle, MoveName::Slash, Item::None, false, 1.0 / 4.0);
}
TEST_CASE("Generation 2 Slash Focus Energy Critical Hit", "[Critical Hit]") {
	individual_test<Generation::two>(Species::Sandshrew, MoveName::Slash, Item::None, true, 1.0 / 3.0);
}
TEST_CASE("Generation 2 Slash Focus Energy Stick Critical Hit", "[Critical Hit]") {
	individual_test<Generation::two>(Species::Farfetchd, MoveName::Slash, Item::Stick, true, 1.0 / 2.0);
}
TEST_CASE("Generation 3 Slash Critical Hit", "[Critical Hit]") {
	individual_test<Generation::three>(Species::Smeargle, MoveName::Slash, Item::None, false, 1.0 / 8.0);
}
TEST_CASE("Generation 3 Slash Focus Energy Critical Hit", "[Critical Hit]") {
	individual_test<Generation::three>(Species::Sandshrew, MoveName::Slash, Item::None, true, 1.0 / 3.0);
}
TEST_CASE("Generation 3 Slash Focus Energy Stick Critical Hit", "[Critical Hit]") {
	individual_test<Generation::three>(Species::Farfetchd, MoveName::Slash, Item::Stick, true, 1.0 / 2.0);
}

} // namespace
} // namespace technicalmachine
