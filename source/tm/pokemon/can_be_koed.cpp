// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.can_be_koed;

import tm.pokemon.any_pokemon;

import tm.ability;
import tm.environment;
import tm.generation;
import tm.item;

namespace technicalmachine {

export enum class CanBeKOed {
	yes,
	no,
	consume_item
};

// TODO: Focus Band activation?
export template<any_active_pokemon PokemonType>
constexpr auto can_be_koed(
	PokemonType const pokemon,
	Environment const environment
) -> CanBeKOed {
	if (pokemon.last_used_move().is_enduring()) {
		return CanBeKOed::no;
	}
	auto const hp = pokemon.hp();
	if (hp.current() != hp.max()) {
		return CanBeKOed::yes;
	}
	constexpr auto generation = generation_from<PokemonType>;
	if (generation >= Generation::five and pokemon.ability() == Ability::Sturdy) {
		return CanBeKOed::no;
	}
	if (pokemon.item(environment) == Item::Focus_Sash) {
		return CanBeKOed::consume_item;
	}
	return CanBeKOed::yes;
}

} // namespace technicalmachine
