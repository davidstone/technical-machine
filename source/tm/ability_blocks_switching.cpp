// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ability_blocks_switching;

import tm.pokemon.any_pokemon;
import tm.pokemon.grounded;
import tm.pokemon.is_type;

import tm.type.type;

import tm.ability;
import tm.environment;
import tm.generation;

namespace technicalmachine {

export template<any_active_pokemon ActivePokemonType>
constexpr bool ability_blocks_switching(Ability const ability, ActivePokemonType const switcher, Environment const environment) {
	constexpr auto generation = generation_from<ActivePokemonType>;
	auto ghost_immunity = [&] {
		return generation >= Generation::six and is_type(switcher, Type::Ghost);
	};
	switch (ability) {
		case Ability::Shadow_Tag:
			return (generation <= Generation::three or switcher.ability() != Ability::Shadow_Tag) and !ghost_immunity();
		case Ability::Arena_Trap:
			return grounded(switcher, environment) and !ghost_immunity();
		case Ability::Magnet_Pull:
			return is_type(switcher, Type::Steel) and !ghost_immunity();
		default:
			return false;
	}
}

} // namespace technicalmachine
