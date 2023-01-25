// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.clients.po.conversion;

import tm.move.move_name;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.ability;
import tm.gender;
import tm.item;

import bounded;
import containers;
import numeric_traits;

namespace technicalmachine {
namespace {

void test_enum(auto && to_id, auto && from_id, auto max) {
	for (auto const original : containers::enum_range(max)) {
		auto const id = to_id(original);
		auto const result = from_id(id);
		CHECK(original == result);
	}
}

TEST_CASE("ability conversion", "[pokemon online]") {
	test_enum(po::ability_to_id, po::id_to_ability, Ability::Bad_Dreams);
}

TEST_CASE("gender conversion", "[pokemon online]") {
	test_enum(po::gender_to_id, po::id_to_gender, numeric_traits::max_value<Gender>);
}

TEST_CASE("species conversion", "[pokemon online]") {
	test_enum(po::species_to_id, po::id_to_species, Species::Arceus);
}

TEST_CASE("item conversion", "[pokemon online]") {
	for (auto const original : containers::enum_range<Item>()) {
		auto const id = po::item_to_id(original);
		auto const result = po::id_to_item(id);
		CHECK((original == result or id == 0));
	}
}

TEST_CASE("move conversion", "[pokemon online]") {
	for (auto const original : containers::enum_range<MoveName>()) {
		if (is_regular(original)) {
			auto const id = po::move_to_id(original);
			auto const result = po::id_to_move(id);
			CHECK(original == result);
		}
	}
}

} // namespace
} // namespace technicalmachine
