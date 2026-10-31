export module tm.pokemon.faint;

import tm.pokemon.any_pokemon;

import tm.environment;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto faint(any_mutable_active_pokemon auto const pokemon) -> void {
	pokemon.set_hp(Environment(), 0_bi);
}

} // namespace technicalmachine