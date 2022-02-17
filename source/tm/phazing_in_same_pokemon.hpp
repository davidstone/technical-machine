// Exception class if phazing attempts to bring in the currently active Pokemon
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/collection.hpp>

#include <stdexcept>

namespace technicalmachine {

struct PhazingInSamePokemon : std::logic_error {
	explicit PhazingInSamePokemon(TeamIndex index);
};

} // namespace technicalmachine
