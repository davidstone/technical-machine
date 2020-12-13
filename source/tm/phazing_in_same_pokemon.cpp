// Exception class if phazing attempts to bring in the currently active Pokemon
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/phazing_in_same_pokemon.hpp>

namespace technicalmachine {

PhazingInSamePokemon::PhazingInSamePokemon(TeamIndex const index):
	std::logic_error("Attempted to phaze in the currently active Pokemon at index " + bounded::to_string(index) + ".")
	{
}

}	// namespace technicalmachine
