// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.binary_file_reader;

import tm.weight;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

static_assert(std::endian::native == std::endian::little);

export auto read_bytes(std::istream & stream, auto const size) -> containers::array<std::byte, size> {
	auto buffer = containers::array<std::byte, size>();
	stream.read(
		reinterpret_cast<char *>(std::addressof(buffer)),
		static_cast<std::streamsize>(sizeof(buffer))
	);
	return buffer;
}

export template<bounded::bounded_integer T>
auto read(std::istream & stream) -> T {
	return bounded::checked_bit_cast<T>(read_bytes(stream, bounded::size_of<T>));
}

export template<typename T> requires std::is_enum_v<T>
auto read(std::istream & stream) -> T {
	return T(read<decltype(bounded::integer(T()))>(stream));
}

export auto read_weight(std::istream & stream) -> Weight<double> {
	return Weight(std::bit_cast<double>(read_bytes(stream, bounded::size_of<double>)));
}

export template<typename Key>
auto map_reader(std::istream & stream, auto const read_mapped) {
	using Count = bounded::integer<0, bounded::normalize<bounded::number_of<Key>>>;
	return containers::generate_n(read<Count>(stream), [&stream, read_mapped] {
		auto key = read<Key>(stream);
		auto mapped = [&] {
			if constexpr (requires { read_mapped(key); }) {
				return read_mapped(key);
			} else {
				return read_mapped();
			}
		}();
		return containers::map_value_type(
			std::move(key),
			std::move(mapped)
		);
	});
}

} // namespace technicalmachine
