// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.grounded;

import tm.pokemon.any_pokemon;
import tm.pokemon.is_type;

import tm.type.type;

import tm.ability;
import tm.environment;
import tm.generation;
import tm.item;

namespace technicalmachine {

export template<any_active_pokemon PokemonType>
auto grounded(PokemonType const pokemon, Environment const environment) -> bool {
	auto item_grounds = [=] {
		auto const item = generation_from<PokemonType> <= Generation::four ?
			pokemon.unrestricted_item() :
			pokemon.item(environment);
		return item == Item::Iron_Ball;
	};

	return
		!(
			is_type(pokemon, Type::Flying) or
			is_immune_to_ground(pokemon.ability()) or
			pokemon.magnet_rise().is_active() or
			pokemon.item(environment) == Item::Air_Balloon
		) or
		environment.gravity() or
		item_grounds() or
		pokemon.ingrained();
}

} // namespace technicalmachine
