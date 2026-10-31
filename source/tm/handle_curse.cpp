export module tm.handle_curse;

import tm.pokemon.any_pokemon;

import tm.environment;
import tm.heal;
import tm.rational;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto handle_curse(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
	if (!pokemon.is_cursed()) {
		return;
	}
	heal(pokemon, environment, rational(-1_bi, 4_bi));
}

} // namespace technicalmachine
