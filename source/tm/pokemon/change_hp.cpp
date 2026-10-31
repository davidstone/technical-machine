export module tm.pokemon.change_hp;

import tm.pokemon.any_pokemon;

import tm.stat.current_hp;

import tm.environment;

import bounded;

namespace technicalmachine {

export constexpr auto change_hp(any_mutable_active_pokemon auto const pokemon, Environment const environment, bounded::bounded_integer auto const change) -> void {
	pokemon.set_hp(environment, bounded::clamp<CurrentHP>(pokemon.hp().current() + change));
}

} // namespace technicalmachine
