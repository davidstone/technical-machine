// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.write_bytes;

import std_module;

namespace technicalmachine {

export template<typename T>
auto write_bytes(std::ostream & stream, T const & value, auto const expected_size) {
	static_assert(!std::is_empty_v<T>);
	static_assert(std::is_trivially_copyable_v<T>);
	static_assert(sizeof(value) == expected_size);
	static_assert(std::endian::native == std::endian::little);
	stream.write(reinterpret_cast<char const *>(std::addressof(value)), sizeof(value));
}

} // namespace technicalmachine
