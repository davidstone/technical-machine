// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/bit_view.hpp>

#include <containers/array/array.hpp>
#include <containers/integer_range.hpp>

namespace technicalmachine {
namespace {

static_assert([] {
	constexpr auto all_zeros = containers::array{std::byte(0x00), std::byte(0x00)};
	auto view = BitView(all_zeros);
	for ([[maybe_unused]] auto const n : containers::integer_range(16_bi)) {
		BOUNDED_ASSERT(view.pop_integer(1_bi) == 0_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

static_assert([] {
	constexpr auto all_ones = containers::array{std::byte(0xFF), std::byte(0xFF)};
	auto view = BitView(all_ones);
	for ([[maybe_unused]] auto const n : containers::integer_range(16_bi)) {
		BOUNDED_ASSERT(view.pop_integer(1_bi) == 1_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

static_assert([] {
	constexpr auto alternating_two_bit = containers::array{std::byte(0b10011001), std::byte(0b10011001)};
	auto view = BitView(alternating_two_bit);
	for ([[maybe_unused]] auto const n : containers::integer_range(4_bi)) {
		BOUNDED_ASSERT(view.pop_integer(2_bi) == 2_bi);
		BOUNDED_ASSERT(view.pop_integer(2_bi) == 1_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

} // namespace
} // namespace technicalmachine