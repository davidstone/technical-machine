// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.pokemon_not_found;

import tm.pokemon.species;

import tm.string_conversions.species;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export struct PokemonNotFound : std::runtime_error {
	explicit PokemonNotFound(Species const species):
		std::runtime_error(containers::concatenate<std::string>("Unable to find "sv, to_string(species)))
	{
	}
};

} // namespace technicalmachine
