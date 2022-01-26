// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <containers/at.hpp>
#include <containers/data.hpp>
#include <containers/maximum_array_size.hpp>
#include <containers/size.hpp>

#include <cstddef>
#include <span>
#include <string_view>

namespace technicalmachine {

using namespace bounded::literal;

struct BitView {
	explicit constexpr BitView(std::span<std::byte const> const view):
		m_view(view)
	{
	}
	
	constexpr auto remaining_bits() const {
		return bounded::increase_min<0>(::bounded::assume_in_range<containers::array_size_type<std::byte>>(m_view.size()) * bounded::char_bit - m_consumed_bits_in_initial_byte);
	}

	constexpr auto skip(auto const bits) {
		pop_bits<not_an_integer>(bits);
	}

	constexpr auto pop_integer(auto const bits) {
		return pop_bits<bounded::integer<0, bounded::normalize<(1_bi << bits) - 1_bi>>>(bits);
	}
	constexpr auto pop_string(auto const size_in_bytes) -> std::string_view {
		BOUNDED_ASSERT(m_consumed_bits_in_initial_byte == 0_bi);
		BOUNDED_ASSERT(size_in_bytes <= m_view.size());
		auto const bytes = pop(static_cast<std::size_t>(size_in_bytes));
		return std::string_view(reinterpret_cast<char const *>(bytes.data()), bytes.size());
	}

private:
	struct not_an_integer {
		constexpr explicit not_an_integer(auto) {}
		constexpr void operator<<=(auto) const {}
		constexpr void operator+=(auto) const {}
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

} // namespace technicalmachine