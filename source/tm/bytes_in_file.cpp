// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/bytes_in_file.hpp>

#include <tm/open_file.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/range_view.hpp>

#include <fstream>
#include <iterator>

namespace technicalmachine {

auto bytes_in_file(std::filesystem::path const & path) -> containers::vector<std::byte> {
	auto file = open_file(path, std::ios_base::binary);
	return containers::vector(containers::transform(
		containers::range_view(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()),
		[](char const c) { return static_cast<std::byte>(c); }
	));
}

} // namespace technicalmachine
