// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/slot_memory.hpp>

#include <containers/front_back.hpp>
#include <containers/integer_range.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <initializer_list>
#include <random>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

void validate_indexes(ps::SlotMemory const & slot_memory, std::initializer_list<ps::SlotMemory::Index> test) {
	auto integer = bounded::integer<0, bounded::normalize<numeric_traits::max_value<TeamIndex> + 1_bi>>(0_bi);
	for (auto const & expected : test) {
		REQUIRE(slot_memory[::bounded::assume_in_range<TeamIndex>(integer)] == expected);
		++integer;
	}
	REQUIRE_THROWS(slot_memory[::bounded::assume_in_range<TeamIndex>(integer)]);
}

template<typename T>
auto make_distribution() {
	return std::uniform_int_distribution<int>(numeric_traits::min_value<T>.value(), numeric_traits::max_value<T>.value());
}

TEST_CASE("Pokemon Showdown Slot Memory Construct", "[Pokemon Showdown]") {
	validate_indexes(ps::SlotMemory(3_bi), {1_bi, 2_bi, 3_bi});
}

TEST_CASE("Pokemon Showdown Slot Memory Switch", "[Pokemon Showdown]") {
	auto switch_to_all = ps::SlotMemory(4_bi);
	// {1, 2, 3, 4}
	switch_to_all.switch_to(1_bi);
	// {2, 1, 3, 4}
	switch_to_all.switch_to(2_bi);
	// {2, 3, 1, 4}
	switch_to_all.switch_to(3_bi);
	// {2, 3, 4, 1}
	validate_indexes(switch_to_all, {2_bi, 3_bi, 4_bi, 1_bi});
}

TEST_CASE("Pokemon Showdown Slot Memory Switch to Self", "[Pokemon Showdown]") {
	auto switch_to_self = ps::SlotMemory(3_bi);
	switch_to_self.switch_to(0_bi);
	validate_indexes(switch_to_self, {1_bi, 2_bi, 3_bi});
}

TEST_CASE("Pokemon Showdown Slot Memory Faint", "[Pokemon Showdown]") {
	auto faint_two = ps::SlotMemory(6_bi);
	// {1, 2, 3, 4, 5, 6}
	faint_two.replace_fainted(3_bi);
	// {2, 3, 1, 5, 6}
	faint_two.replace_fainted(1_bi);
	// {2, 1, 5, 6}
	validate_indexes(faint_two, {2_bi, 1_bi, 5_bi, 6_bi});
}

TEST_CASE("Pokemon Showdown Slot Memory Fuzz", "[Pokemon Showdown]") {
	auto const iterations = 100000_bi;
	auto random_engine = std::mt19937(std::random_device{}());
	auto action_distribution = std::uniform_int_distribution<int>(0, 3);
	auto random_size = [&, distribution = make_distribution<TeamSize>()]() mutable {
		return ::bounded::assume_in_range<TeamSize>(distribution(random_engine));
	};
	auto random_index = [&, distribution = make_distribution<TeamIndex>()]() mutable {
		return ::bounded::assume_in_range<TeamIndex>(distribution(random_engine));
	};

	auto slot_memory = ps::SlotMemory(6_bi);

	for (auto const n [[maybe_unused]] : containers::integer_range(iterations)) {
		try {
			switch (action_distribution(random_engine)) {
				case 0:
					slot_memory = ps::SlotMemory(random_size());
					break;
				case 1:
					slot_memory[random_index()];
					break;
				case 2:
					slot_memory.switch_to(random_index());
					break;
				case 3:
					slot_memory.replace_fainted(random_index());
					break;
			}
		} catch (std::exception const &) {
		}
	}
}

} // namespace
} // namespace technicalmachine
