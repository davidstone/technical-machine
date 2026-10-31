export module tm.pokemon.find_required_pokemon_index;

import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon_not_found;
import tm.pokemon.species;

import tm.find_required_index;

import bounded;
import containers;

namespace technicalmachine {

export template<typename Container>
constexpr auto find_required_pokemon_index(Container const & container, Species const species) {
	return find_required_index(containers::transform(container, &containers::range_value_t<Container>::species), species);
}

} // namespace technicalmachine
