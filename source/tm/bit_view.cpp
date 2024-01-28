// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.bit_view;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

export struct BitView {
	explicit constexpr BitView(std::span<std::byte const> const view):
		m_view(view)
	{
	}

	constexpr auto remaining_bits() const {
		return bounded::increase_min<0>(::bounded::assume_in_range<containers::array_size_type<std::byte>>(m_view.size()) * bounded::char_bit - m_consumed_bits_in_initial_byte);
	}

	constexpr auto skip_bits(auto const bits) -> void {
		pop_bits<not_an_integer>(bits);
	}
	constexpr auto skip_bytes(auto const bytes) -> void {
		pop(static_cast<std::size_t>(bytes));
	}

	constexpr auto pop_integer(auto const bits) {
		return pop_bits<bounded::integer<0, bounded::normalize<(1_bi << bits) - 1_bi>>>(bits);
	}
	constexpr auto pop_string(auto const size_in_bytes) {
		BOUNDED_ASSERT(m_consumed_bits_in_initial_byte == 0_bi);
		BOUNDED_ASSERT(size_in_bytes <= m_view.size());
		auto const bytes = pop(static_cast<std::size_t>(size_in_bytes));
		return containers::string(containers::transform(bytes, bounded::construct<char>));
	}

private:
	struct not_an_integer {
		constexpr explicit not_an_integer(auto) {
		}
		constexpr void operator<<=(auto) const {
		}
		constexpr void operator+=(auto) const {
		}
	};

	constexpr auto pop(std::size_t const bytes) -> std::span<std::byte const> {
		auto const result = m_view.subspan(0, bytes);
		m_view = m_view.subspan(bytes);
		return result;
	}

	template<typename result_t>
	constexpr auto pop_bits(auto const bits) {
		BOUNDED_ASSERT(bits <= remaining_bits());
		auto result = result_t(0_bi);
		auto remaining_bits = bounded::integer<0, bounded::normalize<bits>>(bits);
		while (remaining_bits > 0_bi) {
			auto const bits_remaining_in_initial_byte = bounded::char_bit - m_consumed_bits_in_initial_byte;
			auto const bits_to_consume = bounded::min(remaining_bits, bits_remaining_in_initial_byte);
			auto const low_bits_to_ignore = bounded::increase_min<0>(bits_remaining_in_initial_byte - bits_to_consume);
			result <<= bits_to_consume;
			result += bounded::integer(without_upper_bits() >> low_bits_to_ignore);
			m_consumed_bits_in_initial_byte += bits_to_consume;
			if (m_consumed_bits_in_initial_byte == bounded::char_bit) {
				pop(1);
				m_consumed_bits_in_initial_byte = 0_bi;
			}
			remaining_bits -= bits_to_consume;
		}
		return result;
	}

	constexpr auto without_upper_bits() const -> std::byte {
		return (containers::at(m_view, 0_bi) << m_consumed_bits_in_initial_byte) >> m_consumed_bits_in_initial_byte;
	}

	std::span<std::byte const> m_view;
	bounded::integer<0, bounded::normalize<bounded::char_bit>> m_consumed_bits_in_initial_byte = 0_bi;
};


namespace {

static_assert([] {
	constexpr auto all_zeros = containers::array{std::byte(0x00), std::byte(0x00)};
	auto view = BitView(all_zeros);
	for (auto const _ : containers::integer_range(16_bi)) {
		BOUNDED_ASSERT(view.pop_integer(1_bi) == 0_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

static_assert([] {
	constexpr auto all_ones = containers::array{std::byte(0xFF), std::byte(0xFF)};
	auto view = BitView(all_ones);
	for (auto const _ : containers::integer_range(16_bi)) {
		BOUNDED_ASSERT(view.pop_integer(1_bi) == 1_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

static_assert([] {
	constexpr auto alternating_two_bit = containers::array{std::byte(0b10011001), std::byte(0b10011001)};
	auto view = BitView(alternating_two_bit);
	for (auto const _ : containers::integer_range(4_bi)) {
		BOUNDED_ASSERT(view.pop_integer(2_bi) == 2_bi);
		BOUNDED_ASSERT(view.pop_integer(2_bi) == 1_bi);
	}
	BOUNDED_ASSERT(view.remaining_bits() == 0_bi);
	return true;
}());

} // namespace
} // namespace technicalmachine