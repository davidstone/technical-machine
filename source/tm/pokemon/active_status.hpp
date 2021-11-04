// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/active_pokemon_forward.hpp>

#include <tm/compress.hpp>
#include <tm/end_of_turn.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/status.hpp>
#include <tm/saturating_add.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Weather;

struct ActiveStatus {
	constexpr auto set(Statuses const status) & -> void {
		switch (status) {
			case Statuses::rest:
			case Statuses::sleep:
				m_nightmare = false;
				break;
			case Statuses::toxic:
				m_toxic_counter = 1_bi;
				break;
			default:
				m_none = {};
				break;
		}
	}
	constexpr auto give_nightmares() & -> void {
		m_nightmare = true;
	}

	template<Generation generation>
	constexpr auto status_and_leech_seed_effects(MutableActivePokemon<generation> pokemon, MutableActivePokemon<generation> const other, Weather const weather, bool const uproar) & -> void {
		if constexpr (generation <= Generation::two) {
			end_of_attack(pokemon, other, weather);
		} else {
			end_of_turn(pokemon, other, weather, uproar);
		}
	}

	// TODO: ???
	friend auto operator==(ActiveStatus const lhs, ActiveStatus const rhs) -> bool {
		// TODO: std::bit_cast
		return reinterpret_cast<std::byte const &>(lhs) == reinterpret_cast<std::byte const &>(rhs);
	}
	friend auto compress(ActiveStatus const value) {
		// TODO: std::bit_cast
		auto const byte = reinterpret_cast<std::byte const &>(value);
		return compress(bounded::assume_in_range(static_cast<std::uint8_t>(byte), 0_bi, 15_bi));
	}
private:
	template<Generation generation>
	static constexpr auto handle_leech_seed(MutableActivePokemon<generation> pokemon, MutableActivePokemon<generation> other, Weather const weather) -> void {
		if (!pokemon.leech_seeded()) {
			return;
		}
		auto const initial = pokemon.hp().current();
		heal(pokemon, weather, rational(-1_bi, 8_bi));
		if (other.hp().current() == 0_bi) {
			return;
		}
		auto const hp_change = (initial - pokemon.hp().current()) * healing_multiplier(pokemon.item(weather));
		if (damages_leechers(pokemon.ability())) {
			change_hp(other, weather, -hp_change);
		} else {
			change_hp(other, weather, hp_change);
		}
	}

	template<Generation generation>
	static constexpr auto handle_burn(MutableActivePokemon<generation> pokemon, Weather const weather) -> void {
		auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(pokemon.ability()), 16_bi, 8_bi);
		heal(pokemon, weather, rational(-1_bi, denominator));
	}

	template<Generation generation>
	static constexpr auto handle_poison(MutableActivePokemon<generation> pokemon, Weather const weather) -> void {
		auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(pokemon.ability()), 1_bi, -1_bi);
		heal(pokemon, weather, rational(numerator, 8_bi));
	}

	template<Generation generation>
	static constexpr auto handle_toxic(MutableActivePokemon<generation> pokemon, Weather const weather, bounded::integer<1, 15> & toxic_counter) -> void {
		if (absorbs_poison_damage(pokemon.ability())) {
			heal(pokemon, weather, rational(1_bi, 8_bi));
		} else {
			heal(pokemon, weather, rational(-toxic_counter, 16_bi));
		}
		saturating_increment(toxic_counter);
	}

	template<Generation generation>
	static constexpr auto handle_sleep_and_rest(MutableActivePokemon<generation> pokemon, ActivePokemon<generation> other, Weather const weather, bool const nightmare, bool const uproar = false) -> void {
		if (uproar) {
			pokemon.clear_status();
			return;
		}
		if (nightmare) {
			heal(pokemon, weather, rational(-1_bi, 4_bi));
		}
		if (harms_sleepers(other.ability())) {
			heal(pokemon, weather, rational(-1_bi, 8_bi));
		}
	}

	// Generation 1-2
	template<Generation generation>
	constexpr auto end_of_attack(MutableActivePokemon<generation> pokemon, MutableActivePokemon<generation> const other, Weather const weather) & -> void {
		auto const status = pokemon.status().name();
		switch (status) {
			case Statuses::clear:
			case Statuses::freeze:
			case Statuses::paralysis:
				break;
			case Statuses::burn:
				handle_burn(pokemon, weather);
				break;
			case Statuses::poison:
				handle_poison(pokemon, weather);
				break;
			case Statuses::toxic:
				handle_toxic(pokemon, weather, m_toxic_counter);
				break;
			case Statuses::sleep:
			case Statuses::rest:
				break;
		}

		handle_leech_seed(pokemon, other, weather);

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
				handle_sleep_and_rest(pokemon, other.as_const(), weather, m_nightmare);
				break;
		}
	}

	// Generation 3+
	template<Generation generation>
	constexpr auto end_of_turn(MutableActivePokemon<generation> pokemon, MutableActivePokemon<generation> const other, Weather const weather, bool uproar) & -> void {
		handle_leech_seed(pokemon, other, weather);

		switch (pokemon.status().name()) {
			case Statuses::clear:
			case Statuses::freeze:
			case Statuses::paralysis:
				break;
			case Statuses::burn:
				handle_burn(pokemon, weather);
				break;
			case Statuses::poison:
				handle_poison(pokemon, weather);
				break;
			case Statuses::toxic:
				handle_toxic(pokemon, weather, m_toxic_counter);
				break;
			case Statuses::sleep:
			case Statuses::rest:
				handle_sleep_and_rest(pokemon, other.as_const(), weather, m_nightmare, uproar);
				break;
		}
	}

	// The discriminator is the status of the active Pokemon.
	union {
		std::byte m_none{};
		bool m_nightmare;
		bounded::integer<1, 15> m_toxic_counter;
	};
};

} // namespace technicalmachine
