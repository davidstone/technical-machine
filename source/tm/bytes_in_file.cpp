// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.bytes_in_file;

import tm.open_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

export auto bytes_in_file(std::filesystem::path const & path) -> containers::vector<std::byte> {
	auto file = open_binary_file_for_reading(path);
	return containers::vector(containers::transform(
		containers::subrange(
			std::istreambuf_iterator<char>(file),
			std::default_sentinel
		),
		bounded::construct<std::byte>
	));
}

} // namespace technicalmachine
