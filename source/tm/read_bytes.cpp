// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.read_bytes;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

export template<typename T>
auto read_bytes(std::istream & stream) -> T {
	static_assert(!std::is_empty_v<T>);
	static_assert(std::is_trivially_copyable_v<T>);
	static_assert(std::endian::native == std::endian::little);
	auto buffer = containers::array<std::byte, bounded::size_of<T>>();
	stream.read(
		reinterpret_cast<char *>(std::addressof(buffer)),
		static_cast<std::streamsize>(sizeof(buffer))
	);
	return std::bit_cast<T>(buffer);
}

} // namespace technicalmachine
