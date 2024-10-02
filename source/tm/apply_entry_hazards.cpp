// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.apply_entry_hazards;

import tm.pokemon.any_pokemon;
import tm.pokemon.grounded;
import tm.pokemon.indirect_status_can_apply;
import tm.pokemon.is_type;

import tm.status.status_name;

import tm.type.effectiveness;
import tm.type.type;

import tm.ability;
import tm.entry_hazards;
import tm.environment;
import tm.generation;
import tm.heal;
import tm.item;
import tm.rational;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto removes_toxic_spikes(any_active_pokemon auto const switcher) {
	return is_type(switcher, Type::Poison);
}

template<any_mutable_active_pokemon PokemonType>
constexpr auto apply_toxic_spikes(EntryHazards<generation_from<PokemonType>> const hazards, PokemonType const switcher, Ability const other_ability, Environment const environment) {
	auto const status = hazards.toxic_spikes() == 1_bi ? StatusName::poison : StatusName::toxic;
	auto const weather = environment.effective_weather(switcher.ability(), other_ability);
	if (indirect_status_can_apply(status, switcher.as_const(), weather)) {
		switcher.set_status(status, environment);
	}
}

template<Generation generation>
constexpr auto spikes_damage(EntryHazards<generation> const hazards) -> rational<bounded::constant_t<-1>, bounded::integer<4, 8>> {
	switch (hazards.spikes().value()) {
		case 0: std::unreachable();
		case 1: return rational(-1_bi, 8_bi);
		case 2: return rational(-1_bi, 6_bi);
		case 3: return rational(-1_bi, 4_bi);
		default: std::unreachable();
	}
}

export template<any_mutable_active_pokemon PokemonType>
constexpr auto apply(EntryHazards<generation_from<PokemonType>> & hazards, PokemonType const switcher, Ability const other_ability, Environment const environment) -> void {
	constexpr auto generation = generation_from<PokemonType>;
	if (switcher.item(environment) == Item::Heavy_Duty_Boots) {
		return;
	}

	if (grounded(switcher.as_const(), environment)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(switcher.as_const())) {
				hazards.clear_toxic_spikes();
			} else if (generation >= Generation::five or !blocks_secondary_damage(switcher.ability())) {
				apply_toxic_spikes(hazards, switcher, other_ability, environment);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(switcher, environment, spikes_damage(hazards));
		}
	}
	if (hazards.stealth_rock()) {
		heal(switcher, environment, rational(-1_bi, 8_bi) * Effectiveness(generation, Type::Rock, switcher.types()));
	}
}

} // namespace technicalmachine
