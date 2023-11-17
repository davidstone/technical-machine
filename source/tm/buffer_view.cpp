// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

#include <bounded/assert.hpp>

export module tm.buffer_view;

import tm.split_offsets;

import containers;
import std_module;

namespace technicalmachine {

// TODO: Maybe something with iterators?
export template<typename View>
struct BufferView {
	static_assert(std::is_nothrow_copy_constructible_v<View>);

	explicit constexpr BufferView(View const buffer):
		m_buffer(buffer)
	{
	}
	
	constexpr auto pop(std::size_t const elements) {
		if (elements > m_buffer.size()) {
			throw std::runtime_error("Attempted to pop too many elements.");
		}
		auto result = View(m_buffer.data(), elements);
		m_buffer = View(m_buffer.data() + elements, m_buffer.size() - elements);
		return result;
	}
	constexpr auto remainder() const {
		return m_buffer;
	}

private:
	View m_buffer;
};

export template<typename View>
constexpr auto pop_to_delimiter(BufferView<View> & buffer_view, auto const delimiter) {
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

	constexpr auto pop(auto const delimiter) {
		return pop_to_delimiter(m_buffer, delimiter);
	}
	constexpr auto pop() {
		return pop(m_delimiter);
	}
	constexpr auto remainder() const {
		return m_buffer.remainder();
	}

private:
	BufferView<View> m_buffer;
	Delimiter m_delimiter;
};


namespace {

constexpr auto pop_zero_from_empty() {
	auto view = BufferView(std::string_view(""));

	BOUNDED_ASSERT(view.remainder() == "");

	auto const result = view.pop(0);
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "");
	return true;
}

static_assert(pop_zero_from_empty());


constexpr auto pop_zero_from_non_empty() {
	auto view = BufferView(std::string_view("technical"));

	BOUNDED_ASSERT(view.remainder() == "technical");

	auto const result = view.pop(0);
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "technical");
	return true;
}

static_assert(pop_zero_from_non_empty());

constexpr auto pop_some() {
	auto view = BufferView(std::string_view("machine"));

	BOUNDED_ASSERT(view.remainder() == "machine");

	auto const result = view.pop(3);
	BOUNDED_ASSERT(result == "mac");
	BOUNDED_ASSERT(view.remainder() == "hine");
	return true;
}

static_assert(pop_some());

constexpr auto pop_char_delimiter_from_empty() {
	auto view = DelimitedBufferView(std::string_view(""), 'c');

	BOUNDED_ASSERT(view.remainder() == "");

	auto const result = view.pop();
	BOUNDED_ASSERT(result == "");
	BOUNDED_ASSERT(view.remainder() == "");
	return true;
}

static_assert(pop_char_delimiter_from_empty());

constexpr auto pop_to_found_char_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, 'e');
	BOUNDED_ASSERT(result == "pok");
	BOUNDED_ASSERT(view.remainder() == "mon");

	return true;
}

static_assert(pop_to_found_char_delimiter());

constexpr auto pop_to_not_found_char_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, 'q');
	BOUNDED_ASSERT(result == "pokemon");
	BOUNDED_ASSERT(view.remainder() == "");

	return true;
}

static_assert(pop_to_not_found_char_delimiter());

constexpr auto pop_to_found_string_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, std::string_view("kem"));
	BOUNDED_ASSERT(result == "po");
	BOUNDED_ASSERT(view.remainder() == "on");

	return true;
}

static_assert(pop_to_found_string_delimiter());

constexpr auto pop_to_not_found_string_delimiter() {
	auto view = BufferView(std::string_view("pokemon"));

	BOUNDED_ASSERT(view.remainder() == "pokemon");

	auto const result = pop_to_delimiter(view, std::string_view("pikachu"));
	BOUNDED_ASSERT(result == "pokemon");
	BOUNDED_ASSERT(view.remainder() == "");

	return true;
}

static_assert(pop_to_not_found_string_delimiter());

} // namespace
} // namespace technicalmachine