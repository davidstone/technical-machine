// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <containers/algorithms/find.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace technicalmachine {
namespace detail {

struct split_offsets {
	template<typename View>
	constexpr split_offsets(View const buffer, typename View::value_type const delimiter):
		first(static_cast<std::size_t>(containers::find(buffer, delimiter) - buffer.begin())),
		discard(1U)
	{
	}

	template<typename View, typename Delimiter> requires std::is_same_v<typename View::value_type, typename Delimiter::value_type>
	constexpr split_offsets(View const buffer, Delimiter const delimiter):
		first(static_cast<std::size_t>(std::search(buffer.begin(), buffer.end(), delimiter.begin(), delimiter.end()) - buffer.begin())),
		discard(delimiter.size())
	{
	}

	std::size_t first;
	std::size_t discard;
};

template<typename View>
constexpr auto split_impl(View const buffer, split_offsets const offsets) {
	if (offsets.first == buffer.size()) {
		return std::pair(buffer, View());
	}
	return std::pair(
		View(buffer.data(), offsets.first),
		View(buffer.data() + offsets.first + offsets.discard, buffer.size() - offsets.discard - offsets.first)
	);
}

} // namespace detail

// TODO: Maybe something with iterators?
template<typename View>
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

constexpr auto split_view(auto const buffer, auto const delimiter) {
	return detail::split_impl(buffer, detail::split_offsets(buffer, delimiter));
}

template<typename View>
constexpr auto pop_to_delimiter(BufferView<View> & buffer_view, auto const delimiter) {
	auto const view = buffer_view.remainder();
	auto const offsets = detail::split_offsets(view, delimiter);
	auto result = buffer_view.pop(offsets.first);
	if (offsets.first != view.size()) {
		buffer_view.pop(offsets.discard);
	}
	return result;
}

template<typename View, typename Delimiter = typename View::value_type>
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

} // namespace technicalmachine
