// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/compress.hpp>
#include <tm/end_of_turn.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/other_team.hpp>
#include <tm/rational.hpp>
#include <tm/saturating_add.hpp>
#include <tm/status_name.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct ActiveStatus {
	constexpr auto set(StatusName const status) & -> void {
		switch (status) {
			case StatusName::rest:
			case StatusName::sleep:
				m_nightmare = false;
				break;
			case StatusName::toxic:
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

	template<any_mutable_active_pokemon PokemonType>
	constexpr auto status_and_leech_seed_effects(PokemonType const pokemon, OtherMutableActivePokemon<PokemonType> const other, Weather const weather, bool const uproar) & -> void {
		if constexpr (generation_from<PokemonType> <= Generation::two) {
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
	template<any_mutable_active_pokemon PokemonType>
	static constexpr auto handle_leech_seed(PokemonType const pokemon, OtherMutableActivePokemon<PokemonType> other, Weather const weather) -> void {
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

	static constexpr auto handle_burn(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
		auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(pokemon.ability()), 16_bi, 8_bi);
		heal(pokemon, weather, rational(-1_bi, denominator));
	}

	static constexpr auto handle_poison(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
		auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(pokemon.ability()), 1_bi, -1_bi);
		heal(pokemon, weather, rational(numerator, 8_bi));
	}

	static constexpr auto handle_toxic(any_mutable_active_pokemon auto const pokemon, Weather const weather, bounded::integer<1, 15> & toxic_counter) -> void {
		if (absorbs_poison_damage(pokemon.ability())) {
			heal(pokemon, weather, rational(1_bi, 8_bi));
		} else {
			heal_exactly(pokemon, weather, -toxic_counter * (pokemon.hp().max() / 16_bi));
		}
		saturating_increment(toxic_counter);
	}

	template<any_mutable_active_pokemon PokemonType>
	static constexpr auto handle_sleep_and_rest(PokemonType const pokemon, OtherActivePokemon<PokemonType> const other, Weather const weather, bool const nightmare, bool const uproar = false) -> void {
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
	template<any_mutable_active_pokemon PokemonType>
	constexpr auto end_of_attack(PokemonType const pokemon, OtherMutableActivePokemon<PokemonType> const other, Weather const weather) & -> void {
		auto const status = pokemon.status().name();
		switch (status) {
			case StatusName::clear:
			case StatusName::freeze:
			case StatusName::paralysis:
				break;
			case StatusName::burn:
				handle_burn(pokemon, weather);
				break;
			case StatusName::poison:
				handle_poison(pokemon, weather);
				break;
			case StatusName::toxic:
				handle_toxic(pokemon, weather, m_toxic_counter);
				break;
			case StatusName::sleep:
			case StatusName::rest:
				break;
		}

		handle_leech_seed(pokemon, other, weather);

		switch (status) {
			case StatusName::clear:
			case StatusName::freeze:
			case StatusName::paralysis:
			case StatusName::burn:
			case StatusName::poison:
			case StatusName::toxic:
				break;
			case StatusName::sleep:
			case StatusName::rest:
				handle_sleep_and_rest(pokemon, other.as_const(), weather, m_nightmare);
				break;
		}
	}

	// Generation 3+
	template<any_mutable_active_pokemon PokemonType>
	constexpr auto end_of_turn(PokemonType const pokemon, OtherMutableActivePokemon<PokemonType> const other, Weather const weather, bool uproar) & -> void {
		handle_leech_seed(pokemon, other, weather);

		switch (pokemon.status().name()) {
			case StatusName::clear:
			case StatusName::freeze:
			case StatusName::paralysis:
				break;
			case StatusName::burn:
				handle_burn(pokemon, weather);
				break;
			case StatusName::poison:
				handle_poison(pokemon, weather);
				break;
			case StatusName::toxic:
				handle_toxic(pokemon, weather, m_toxic_counter);
				break;
			case StatusName::sleep:
			case StatusName::rest:
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
