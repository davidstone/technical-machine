export module tm.pokemon.pokemon_not_found;

import tm.pokemon.species;

import tm.string_conversions.species;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export struct PokemonNotFound : std::runtime_error {
	explicit PokemonNotFound(Species const species):
		std::runtime_error(containers::concatenate<std::string>("Unable to find "_sv, to_string(species)))
	{
	}
};

} // namespace technicalmachine
