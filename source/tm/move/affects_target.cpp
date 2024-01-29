// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.affects_target;

import tm.move.category;
import tm.move.known_move;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.grounded;

import tm.type.effectiveness;
import tm.type.type;

import tm.environment;
import tm.generation;

namespace technicalmachine {

constexpr auto always_affects_target(Generation const generation, MoveName const move) {
	switch (generation) {
		case Generation::one:
			switch (move) {
				case MoveName::Night_Shade:
				case MoveName::Seismic_Toss:
				case MoveName::Sonic_Boom:
				case MoveName::Super_Fang:
					return true;
				default:
					return false;
			}
		case Generation::two:
		case Generation::three:
		case Generation::four:
		case Generation::five:
		case Generation::six:
		case Generation::seven:
		case Generation::eight:
			return false;
	}
}

export template<any_active_pokemon TargetPokemon>
constexpr auto affects_target(KnownMove const move, TargetPokemon const target, Environment const environment) -> bool {
	constexpr auto generation = generation_from<TargetPokemon>;
	auto const effectiveness = Effectiveness(generation, move.type, target.types());
	if (!is_damaging(move.name) or always_affects_target(generation, move.name)) {
		return true;
	}
	return !effectiveness.has_no_effect() and (move.type != Type::Ground or grounded(target, environment));
}

} // namespace technicalmachine
