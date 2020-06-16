// Copyright (C) 2020 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/pokemon/active_status.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/end_of_turn.hpp>
#include <tm/heal.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {
namespace {

auto handle_leech_seed(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon other, Weather const weather) -> void {
	if (!pokemon.leech_seeded()) {
		return;
	}
	auto const initial = get_hp(pokemon).current();
	heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
	if (get_hp(other) == 0_bi) {
		return;
	}
	auto const hp_change = (initial - get_hp(pokemon).current()) * healing_multiplier(pokemon.item(generation, weather));
	if (damages_leechers(pokemon.ability())) {
		change_hp(generation, other, weather, -hp_change);
	} else {
		change_hp(generation, other, weather, hp_change);
	}
}

auto handle_burn(Generation const generation, MutableActivePokemon pokemon, Weather const weather) -> void {
	auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(pokemon.ability()), 16_bi, 8_bi);
	heal(generation, pokemon, weather, rational(-1_bi, denominator));
}

auto handle_poison(Generation const generation, MutableActivePokemon pokemon, Weather const weather) -> void {
	auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(pokemon.ability()), 1_bi, -1_bi);
	heal(generation, pokemon, weather, rational(numerator, 8_bi));
}

auto handle_toxic(Generation const generation, MutableActivePokemon pokemon, Weather const weather, bounded::clamped_integer<1, 15> & toxic_counter) -> void {
	if (absorbs_poison_damage(pokemon.ability())) {
		heal(generation, pokemon, weather, rational(1_bi, 8_bi));
	} else {
		heal(generation, pokemon, weather, rational(-toxic_counter, 16_bi));
	}
	++toxic_counter;
}

auto handle_sleep_and_rest(Generation const generation, MutableActivePokemon pokemon, ActivePokemon other, Weather const weather, bool const nightmare, bool const uproar = false) -> void {
	if (uproar) {
		pokemon.clear_status();
		return;
	}
	if (nightmare) {
		heal(generation, pokemon, weather, rational(-1_bi, 4_bi));
	}
	if (harms_sleepers(other.ability())) {
		heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
	}
}

} // namespace

// Generation 1-2
auto ActiveStatus::end_of_attack(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather) & -> void {
	auto const status = pokemon.status().name();
	switch (status) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
			break;
		case Statuses::burn:
			handle_burn(generation, pokemon, weather);
			break;
		case Statuses::poison:
			handle_poison(generation, pokemon, weather);
			break;
		case Statuses::toxic:
			handle_toxic(generation, pokemon, weather, m_toxic_counter);
			break;
		case Statuses::sleep:
		case Statuses::rest:
			break;
	}

	handle_leech_seed(generation, pokemon, other, weather);

	switch (status) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
		case Statuses::burn:
		case Statuses::poison:
		case Statuses::toxic:
			break;
		case Statuses::sleep:
		case Statuses::rest:
			handle_sleep_and_rest(generation, pokemon, other, weather, m_nightmare);
			break;
	}
}

// Generation 3+
auto ActiveStatus::end_of_turn(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather, bool uproar) & -> void {
	handle_leech_seed(generation, pokemon, other, weather);

	switch (pokemon.status().name()) {
		case Statuses::clear:
		case Statuses::freeze:
		case Statuses::paralysis:
			break;
		case Statuses::burn:
			handle_burn(generation, pokemon, weather);
			break;
		case Statuses::poison:
			handle_poison(generation, pokemon, weather);
			break;
		case Statuses::toxic:
			handle_toxic(generation, pokemon, weather, m_toxic_counter);
			break;
		case Statuses::sleep:
		case Statuses::rest:
			handle_sleep_and_rest(generation, pokemon, other, weather, m_nightmare, uproar);
			break;
	}
}

auto ActiveStatus::status_and_leech_seed_effects(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon const other, Weather const weather, bool const uproar) & -> void {
	if (generation <= Generation::two) {
		end_of_attack(generation, pokemon, other, weather);
	} else {
		end_of_turn(generation, pokemon, other, weather, uproar);
	}
}

} // namespace technicalmachine
