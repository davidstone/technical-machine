// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/bytes_in_file.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/range_view.hpp>

#include <iterator>

namespace technicalmachine {

auto bytes_in_file(std::ifstream file) -> containers::vector<std::byte> {
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	return containers::vector(containers::transform(
		containers::range_view(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()),
		[](char const c) { return static_cast<std::byte>(c); }
	));
}

} // namespace technicalmachine
