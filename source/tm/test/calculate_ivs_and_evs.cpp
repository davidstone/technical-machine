// Test stat calculations
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/calculate_ivs_and_evs.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/calculate_ivs_and_evs.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

void test_low_attack() {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Tentacruel;
	constexpr auto level = Level(100_bi);
	constexpr auto stats = Stats<generation>(
		HP(364_bi),
		133_bi,
		177_bi,
		196_bi,
		352_bi,
		245_bi
	);
	constexpr auto hidden_power = bounded::optional<HiddenPower<generation>>();
	constexpr auto nature = Nature::Calm;

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(nature, nature));
	
	constexpr auto expected = CombinedStats<generation>{
		nature,
		IVs(
			IV(31_bi),
			IV(3_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		),
		EVs(
			EV(252_bi),
			EV(0_bi),
			EV(44_bi),
			EV(0_bi),
			EV(176_bi),
			EV(36_bi)
		)
	};
	if (calculated != expected) {
		throw std::runtime_error("Incorrect");
	}
}


void test_generation_two_with_hidden_power() {
	constexpr auto generation = Generation::two;
	constexpr auto species = Species::Sunflora;
	constexpr auto level = Level(80_bi);
	constexpr auto stats = Stats<generation>(
		HP(284_bi),
		199_bi,
		164_bi,
		247_bi,
		215_bi,
		127_bi
	);
	constexpr auto hidden_power = bounded::optional(HiddenPower<generation>(70_bi, Type::Ice));

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power);
	
	constexpr auto expected = CombinedStats<generation>{
		Nature::Hardy,
		DVs(
			DV(15_bi),
			DV(13_bi),
			DV(15_bi),
			DV(15_bi)
		),
		OldGenEVs(
			EV(252_bi),
			EV(252_bi),
			EV(252_bi),
			EV(252_bi),
			EV(252_bi)
		)
	};
	if (calculated != expected) {
		throw std::runtime_error("Incorrect");
	}
}

void test_generation_four_with_hidden_power() {
	constexpr auto generation = Generation::four;
	constexpr auto species = Species::Roserade;
	constexpr auto level = Level(75_bi);
	constexpr auto stats = Stats<generation>(
		HP(245_bi),
		100_bi,
		131_bi,
		236_bi,
		191_bi,
		185_bi
	);
	constexpr auto hidden_power = bounded::optional(HiddenPower<generation>(70_bi, Type::Dark));

	auto const calculated = calculate_ivs_and_evs(species, level, stats, hidden_power, containers::enum_range(Nature::Modest, Nature::Modest));
	
	constexpr auto expected = CombinedStats<generation>{
		Nature::Modest,
		IVs(
			IV(31_bi),
			IV(3_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi),
			IV(31_bi)
		),
		EVs(
			EV(252_bi),
			EV(0_bi),
			EV(108_bi),
			EV(0_bi),
			EV(28_bi),
			EV(116_bi)
		)
	};
	if (calculated != expected) {
		throw std::runtime_error("Incorrect");
	}
}

} // namespace

void calculate_ivs_and_evs_tests() {
	std::cout << "Running calculate_ivs_and_evs tests\n";
	test_low_attack();
	test_generation_two_with_hidden_power();
	test_generation_four_with_hidden_power();
}

} // namespace technicalmachine
