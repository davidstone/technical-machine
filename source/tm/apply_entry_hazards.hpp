// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/type/effectiveness.hpp>

#include <tm/compress.hpp>
#include <tm/entry_hazards.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto removes_toxic_spikes(any_active_pokemon auto const switcher) {
	return is_type(switcher, Type::Poison);
}

template<any_mutable_active_pokemon PokemonType>
constexpr auto apply_toxic_spikes(EntryHazards<generation_from<PokemonType>> const & hazards, PokemonType const switcher, Weather const weather) {
	auto const status = hazards.toxic_spikes() == 1_bi ? Statuses::poison : Statuses::toxic;
	if (indirect_status_can_apply(status, switcher.as_const(), weather)) {
		switcher.set_status(status, weather);
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

template<any_mutable_active_pokemon PokemonType>
constexpr auto apply(EntryHazards<generation_from<PokemonType>> & hazards, PokemonType const switcher, Weather const weather) -> void {
	constexpr auto generation = generation_from<PokemonType>;
	if (switcher.item(weather) == Item::Heavy_Duty_Boots) {
		return;
	}

	if (grounded(switcher.as_const(), weather)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(switcher.as_const())) {
				hazards.clear_toxic_spikes();
			} else if (generation >= Generation::five or !blocks_secondary_damage(switcher.ability())) {
				apply_toxic_spikes(hazards, switcher, weather);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(switcher, weather, spikes_damage(hazards));
		}
	}
	if (hazards.stealth_rock()) {
		heal(switcher, weather, rational(-1_bi, 8_bi) * Effectiveness(generation, Type::Rock, switcher.types()));
	}
}

} // namespace technicalmachine
