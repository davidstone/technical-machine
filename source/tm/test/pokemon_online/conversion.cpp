// Test PO conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/pokemon_online/conversion.hpp>

#include <tm/test/invalid_simulator_conversion.hpp>

#include <tm/ability.hpp>

#include <tm/move/is_switch.hpp>

#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/stat/nature.hpp>

#include <tm/clients/pokemon_online/conversion.hpp>

#include <containers/integer_range.hpp>

#include <iostream>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace po {
namespace {

void test_enum(std::string_view const name, auto && to_id, auto && from_id, auto max) {
	std::cout << "\t\tVerifying correct " << name << ".\n";
	for (auto const original : containers::enum_range(max)) {
		auto const id = to_id(original);
		auto const result = from_id(id);
		if (original != result) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_item() {
	std::cout << "\t\tVerifying correct item.\n";
	for (auto const original : containers::enum_range<Item>()) {
		auto const id = item_to_id(original);
		auto const result = id_to_item(id);
		if (original != result and id != 0) {
			throw InvalidSimulatorConversion(original, result);
		}
	}
}

void test_move() {
	std::cout << "\t\tVerifying correct move.\n";
	for(auto const original : containers::enum_range<Moves>()) {
		if (is_regular(original)) {
			auto const id = move_to_id(original);
			auto const result = id_to_move(id);
			if (original != result) {
				throw InvalidSimulatorConversion(original, result);
			}
		}
	}
}

}	// namespace

void test_conversions () {
	std::cout << "\tRunning Pokemon Online conversion tests.\n";
	test_enum("Ability", ability_to_id, id_to_ability, Ability::Bad_Dreams);
	test_enum("Gender", gender_to_id, id_to_gender, bounded::max_value<Gender>);
	test_item();
	test_move();
	test_enum("Nature", nature_to_id, id_to_nature, bounded::max_value<Nature>);
	test_enum("Species", species_to_id, id_to_species, Species::Arceus);
}

}	// namespace po
}	// namespace technicalmachine
