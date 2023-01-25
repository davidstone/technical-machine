// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
