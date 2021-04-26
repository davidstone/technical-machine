// Exception if unable to find a Pokemon that should exist
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/pokemon_not_found.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

PokemonNotFound::PokemonNotFound(Species const species):
	std::logic_error(containers::concatenate<std::string>("Unable to find "sv, to_string(species)))
{
}

}	// namespace technicalmachine
