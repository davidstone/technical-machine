// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/ingame_id_to_nature.hpp>

#include <containers/integer_range.hpp>

namespace technicalmachine {
namespace {

static_assert([]{
	for (auto const original : containers::enum_range<Nature>()) {
		auto const id = ingame_nature_to_id(original);
		auto const result = ingame_id_to_nature(id);
		BOUNDED_ASSERT(original == result);
	}
	return true;
}());

} // namespace
} // namespace technicalmachine