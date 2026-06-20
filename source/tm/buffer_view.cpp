// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.buffer_view;

import tm.split_offsets;

import bounded;
import containers;
import std_module;

using namespace bounded::literal;
using namespace containers::string_literals;

namespace technicalmachine {

// TODO: Maybe something with iterators?
export template<typename View>
struct BufferView {
	static_assert(std::is_nothrow_copy_constructible_v<View>);

	explicit constexpr BufferView(View const buffer):
		m_buffer(buffer)
	{
	}
	
	constexpr auto pop(containers::range_size_t<View> const elements) -> View {
		auto result = View(containers::take(m_buffer, elements));
		m_buffer = View(containers::drop_exactly(m_buffer, elements));
		return result;
	}
	constexpr auto remainder() const -> View {
		return m_buffer;
	}

private:
	View m_buffer;
};

export template<typename View>
constexpr auto pop_to_delimiter(BufferView<View> & buffer_view, auto const delimiter) -> View {
	auto const view = buffer_view.remainder();
	auto const offsets = split_offsets(view, delimiter);
	auto result = buffer_view.pop(offsets.first);
	if (offsets.first != view.size()) {
		buffer_view.pop(offsets.discard);
	}
	return result;
}

export template<typename View, typename Delimiter = containers::range_value_t<View>>
struct DelimitedBufferView {
	static_assert(std::is_nothrow_copy_constructible_v<View>);

	constexpr DelimitedBufferView(View const buffer, Delimiter const delimiter):
		m_buffer(buffer),
		m_delimiter(delimiter)
	{
	}

	constexpr auto pop(auto const delimiter) -> View {
		return pop_to_delimiter(m_buffer, delimiter);
	}
	constexpr auto pop() -> View {
		return pop(m_delimiter);
	}
	constexpr auto remainder() const -> View {
		return m_buffer.remainder();
	}

private:
	BufferView<View> m_buffer;
	Delimiter m_delimiter;
};


namespace {

constexpr auto pop_zero_from_empty() -> bool {
	auto view = BufferView(""_sv);

	BOUNDED_ASSERT(view.remainder() == ""_s);

	auto const result = view.pop(0_bi);
	BOUNDED_ASSERT(result == ""_s);
	BOUNDED_ASSERT(view.remainder() == ""_s);
	return true;
}

static_assert(pop_zero_from_empty());


constexpr auto pop_zero_from_non_empty() -> bool {
	auto view = BufferView("technical"_sv);

	BOUNDED_ASSERT(view.remainder() == "technical"_s);

	auto const result = view.pop(0_bi);
	BOUNDED_ASSERT(result == ""_s);
	BOUNDED_ASSERT(view.remainder() == "technical"_s);
	return true;
}

static_assert(pop_zero_from_non_empty());

constexpr auto pop_some() -> bool {
	auto view = BufferView("machine"_sv);

	BOUNDED_ASSERT(view.remainder() == "machine"_s);

	auto const result = view.pop(3_bi);
	BOUNDED_ASSERT(result == "mac"_s);
	BOUNDED_ASSERT(view.remainder() == "hine"_s);
	return true;
}

static_assert(pop_some());

constexpr auto pop_char_delimiter_from_empty() -> bool {
	auto view = DelimitedBufferView(""_sv, 'c');

	BOUNDED_ASSERT(view.remainder() == ""_s);

	auto const result = view.pop();
	BOUNDED_ASSERT(result == ""_s);
	BOUNDED_ASSERT(view.remainder() == ""_s);
	return true;
}

static_assert(pop_char_delimiter_from_empty());

constexpr auto pop_to_found_char_delimiter() -> bool {
	auto view = BufferView("pokemon"_sv);

	BOUNDED_ASSERT(view.remainder() == "pokemon"_s);

	auto const result = pop_to_delimiter(view, 'e');
	BOUNDED_ASSERT(result == "pok"_s);
	BOUNDED_ASSERT(view.remainder() == "mon"_s);

	return true;
}

static_assert(pop_to_found_char_delimiter());

constexpr auto pop_to_not_found_char_delimiter() -> bool {
	auto view = BufferView("pokemon"_sv);

	BOUNDED_ASSERT(view.remainder() == "pokemon"_s);

	auto const result = pop_to_delimiter(view, 'q');
	BOUNDED_ASSERT(result == "pokemon"_s);
	BOUNDED_ASSERT(view.remainder() == ""_s);

	return true;
}

static_assert(pop_to_not_found_char_delimiter());

constexpr auto pop_to_found_string_delimiter() -> bool {
	auto view = BufferView("pokemon"_sv);

	BOUNDED_ASSERT(view.remainder() == "pokemon"_s);

	auto const result = pop_to_delimiter(view, "kem"_s);
	BOUNDED_ASSERT(result == "po"_s);
	BOUNDED_ASSERT(view.remainder() == "on"_s);

	return true;
}

static_assert(pop_to_found_string_delimiter());

constexpr auto pop_to_not_found_string_delimiter() -> bool {
	auto view = BufferView("pokemon"_sv);

	BOUNDED_ASSERT(view.remainder() == "pokemon"_s);

	auto const result = pop_to_delimiter(view, "pikachu"_s);
	BOUNDED_ASSERT(result == "pokemon"_s);
	BOUNDED_ASSERT(view.remainder() == ""_s);

	return true;
}

static_assert(pop_to_not_found_string_delimiter());

} // namespace
} // namespace technicalmachine