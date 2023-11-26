// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.ps.slot_memory;

import tm.pokemon.max_pokemon_per_team;

import bounded;
import containers;
import numeric_traits;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

void validate_indexes(ps::SlotMemory const & slot_memory, std::initializer_list<ps::SlotMemory::Index> test) {
	auto integer = bounded::integer<0, bounded::normalize<numeric_traits::max_value<TeamIndex> + 1_bi>>(0_bi);
	for (auto const & expected : test) {
		REQUIRE(slot_memory[::bounded::check_in_range<TeamIndex>(integer)] == expected);
		++integer;
	}
	REQUIRE_THROWS(slot_memory[::bounded::check_in_range<TeamIndex>(integer)]);
}

template<typename T>
auto make_distribution() {
	return std::uniform_int_distribution<int>(
		numeric_traits::min_value<T>.value(),
		numeric_traits::max_value<T>.value()
	);
}

template<typename T>
auto positive_distribution() {
	return std::uniform_int_distribution<int>(
		1,
		numeric_traits::max_value<T>.value()
	);
}

template<typename T>
constexpr auto enumerate = containers::inclusive_integer_range(
	numeric_traits::min_value<T>,
	numeric_traits::max_value<T>
);

auto test_indexes_consistent(ps::SlotMemory memory, auto const size) -> void {
	for (auto const index : enumerate<TeamIndex>) {
		if (index < size) {
			CHECK(memory.reverse_lookup(memory[index]) == index);
		} else {
			CHECK_THROWS(memory[index]);
		}
	}
	auto const slot_indexes = containers::transform(
		containers::integer_range(size),
		[&](TeamIndex const index) { return memory[index]; }
	);
	for (auto const index : enumerate<ps::SlotMemory::Index>) {
		if (containers::any_equal(slot_indexes, index)) {
			CHECK(memory[memory.reverse_lookup(index)] == index);
		} else {
			CHECK_THROWS(memory.reverse_lookup(index));
		}
	}
}

TEST_CASE("Pokemon Showdown SlotMemory initial state consistent", "[Pokemon Showdown]") {
	for (auto const size : enumerate<TeamSize>) {
		if (size == 0_bi) {
			CHECK_THROWS(ps::SlotMemory(size));
		} else {
			INFO(size);
			test_indexes_consistent(ps::SlotMemory(size), size);
		}
	}
}

TEST_CASE("Pokemon Showdown SlotMemory Construct", "[Pokemon Showdown]") {
	validate_indexes(ps::SlotMemory(3_bi), {1_bi, 2_bi, 3_bi});
}

TEST_CASE("Pokemon Showdown SlotMemory Switch", "[Pokemon Showdown]") {
	auto memory = ps::SlotMemory(4_bi);
	validate_indexes(memory, {1_bi, 2_bi, 3_bi, 4_bi});
	memory.switch_to(1_bi);
	validate_indexes(memory, {2_bi, 1_bi, 3_bi, 4_bi});
	memory.switch_to(2_bi);
	validate_indexes(memory, {2_bi, 3_bi, 1_bi, 4_bi});
	memory.switch_to(3_bi);
	validate_indexes(memory, {2_bi, 3_bi, 4_bi, 1_bi});
}

TEST_CASE("Pokemon Showdown SlotMemory Switch to Self", "[Pokemon Showdown]") {
	auto memory = ps::SlotMemory(3_bi);
	memory.switch_to(0_bi);
	validate_indexes(memory, {1_bi, 2_bi, 3_bi});
}

TEST_CASE("Pokemon Showdown SlotMemory Faint", "[Pokemon Showdown]") {
	auto memory = ps::SlotMemory(6_bi);
	validate_indexes(memory, {1_bi, 2_bi, 3_bi, 4_bi, 5_bi, 6_bi});
	memory.replace_fainted(3_bi);
	validate_indexes(memory, {2_bi, 3_bi, 1_bi, 5_bi, 6_bi});
	memory.replace_fainted(1_bi);
	validate_indexes(memory, {2_bi, 1_bi, 5_bi, 6_bi});
}

TEST_CASE("Pokemon Showdown SlotMemory mixed operations", "[Pokemon Showdown]") {
	auto memory = ps::SlotMemory(6_bi);
	validate_indexes(memory, {1_bi, 2_bi, 3_bi, 4_bi, 5_bi, 6_bi});
	memory.switch_to(memory.reverse_lookup(2_bi));
	validate_indexes(memory, {2_bi, 1_bi, 3_bi, 4_bi, 5_bi, 6_bi});
	memory.switch_to(memory.reverse_lookup(5_bi));
	validate_indexes(memory, {2_bi, 5_bi, 3_bi, 4_bi, 1_bi, 6_bi});
	memory.replace_fainted(memory.reverse_lookup(4_bi));
	validate_indexes(memory, {2_bi, 5_bi, 3_bi, 1_bi, 6_bi});
	memory.replace_fainted(memory.reverse_lookup(6_bi));
	validate_indexes(memory, {2_bi, 5_bi, 3_bi, 1_bi});
	memory.replace_fainted(memory.reverse_lookup(3_bi));
	validate_indexes(memory, {2_bi, 5_bi, 1_bi});
}

TEST_CASE("Pokemon Showdown Slot Memory Fuzz", "[Pokemon Showdown]") {
	auto const iterations = 10000_bi;
	auto random_engine = std::mt19937(std::random_device{}());
	auto action_distribution = std::uniform_int_distribution<int>(0, 4);
	auto random_size = [&, distribution = positive_distribution<TeamSize>()]() mutable {
		return ::bounded::assume_in_range<TeamSize>(distribution(random_engine));
	};
	auto random_index = [&, distribution = make_distribution<TeamIndex>()]() mutable {
		return ::bounded::assume_in_range<TeamIndex>(distribution(random_engine));
	};

	auto size = random_size();
	auto slot_memory = ps::SlotMemory(size);

	for ([[maybe_unused]] auto const _ : containers::integer_range(iterations)) {
		try {
			switch (action_distribution(random_engine)) {
				case 0:
					size = random_size();
					slot_memory = ps::SlotMemory(size);
					break;
				case 1:
				case 2:
					slot_memory.switch_to(random_index());
					test_indexes_consistent(slot_memory, size);
					break;
				case 3:
				case 4:
					slot_memory.replace_fainted(random_index());
					--size;
					test_indexes_consistent(slot_memory, size);
					break;
			}
		} catch (std::exception const &) {
		}
	}
}

} // namespace
} // namespace technicalmachine
