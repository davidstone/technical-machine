// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.active_status;

import tm.pokemon.any_pokemon;
import tm.pokemon.change_hp;

import tm.status.status_name;

import tm.compress;
import tm.environment;
import tm.generation;
import tm.heal;
import tm.rational;
import tm.saturating_add;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto handle_leech_seed(any_mutable_active_pokemon auto const pokemon, any_mutable_active_pokemon auto const other, Environment const environment) -> void {
	if (!pokemon.leech_seeded()) {
		return;
	}
	auto const initial = pokemon.hp().current();
	heal(pokemon, environment, rational(-1_bi, 8_bi));
	if (other.hp().current() == 0_bi) {
		return;
	}
	auto const hp_change = (initial - pokemon.hp().current()) * healing_multiplier(pokemon.item(environment));
	if (damages_leechers(pokemon.ability())) {
		change_hp(other, environment, -hp_change);
	} else {
		change_hp(other, environment, hp_change);
	}
}

constexpr auto handle_burn(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
	auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(pokemon.ability()), 16_bi, 8_bi);
	heal(pokemon, environment, rational(-1_bi, denominator));
}

constexpr auto handle_poison(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
	auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(pokemon.ability()), 1_bi, -1_bi);
	heal(pokemon, environment, rational(numerator, 8_bi));
}

constexpr auto handle_sleep_and_rest(any_mutable_active_pokemon auto const pokemon, any_active_pokemon auto const other, Environment const environment, bool const nightmare, bool const uproar = false) -> void {
	if (uproar) {
		pokemon.clear_status();
		return;
	}
	if (nightmare) {
		heal(pokemon, environment, rational(-1_bi, 4_bi));
	}
	if (harms_sleepers(other.ability())) {
		heal(pokemon, environment, rational(-1_bi, 8_bi));
	}
}

export struct ActiveStatus {
	constexpr auto set(StatusName const status) & -> void {
		switch (status) {
			case StatusName::rest:
			case StatusName::sleep:
				m_state = no_nightmare_state;
				break;
			case StatusName::toxic:
				m_state = 1_bi;
				break;
			default:
				break;
		}
	}
	constexpr auto give_nightmares() & -> void {
		m_state = nightmare_state;
	}

	template<any_mutable_active_pokemon PokemonType>
	constexpr auto status_and_leech_seed_effects(PokemonType const pokemon, any_mutable_active_pokemon auto const other, Environment const environment, bool const uproar) & -> void {
		if constexpr (generation_from<PokemonType> <= Generation::two) {
			end_of_attack(pokemon, other, environment);
		} else {
			end_of_turn(pokemon, other, environment, uproar);
		}
	}

	friend auto operator==(ActiveStatus, ActiveStatus) -> bool = default;
	friend constexpr auto compress(ActiveStatus const value) {
		return compress(value.m_state);
	}

private:
	constexpr auto handle_toxic(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
		if (absorbs_poison_damage(pokemon.ability())) {
			heal(pokemon, environment, rational(1_bi, 8_bi));
		} else {
			heal_exactly(pokemon, environment, -toxic_counter() * (pokemon.hp().max() / 16_bi));
		}
		saturating_increment(m_state);
	}

	// Generation 1-2
	constexpr auto end_of_attack(any_mutable_active_pokemon auto const pokemon, any_mutable_active_pokemon auto const other, Environment const environment) & -> void {
		auto const status = pokemon.status().name();
		switch (status) {
			case StatusName::clear:
			case StatusName::freeze:
			case StatusName::paralysis:
				break;
			case StatusName::burn:
				handle_burn(pokemon, environment);
				break;
			case StatusName::poison:
				handle_poison(pokemon, environment);
				break;
			case StatusName::toxic:
				handle_toxic(pokemon, environment);
				break;
			case StatusName::sleep:
			case StatusName::rest:
				break;
		}

		handle_leech_seed(pokemon, other, environment);

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
				handle_sleep_and_rest(pokemon, other.as_const(), environment, is_nightmare());
				break;
		}
	}

	// Generation 3+
	constexpr auto end_of_turn(any_mutable_active_pokemon auto const pokemon, any_mutable_active_pokemon auto const other, Environment const environment, bool uproar) & -> void {
		handle_leech_seed(pokemon, other, environment);

		switch (pokemon.status().name()) {
			case StatusName::clear:
			case StatusName::freeze:
			case StatusName::paralysis:
				break;
			case StatusName::burn:
				handle_burn(pokemon, environment);
				break;
			case StatusName::poison:
				handle_poison(pokemon, environment);
				break;
			case StatusName::toxic:
				handle_toxic(pokemon, environment);
				break;
			case StatusName::sleep:
			case StatusName::rest:
				handle_sleep_and_rest(pokemon, other.as_const(), environment, is_nightmare(), uproar);
				break;
		}
	}

	constexpr auto is_nightmare() const -> bool {
		BOUNDED_ASSERT(m_state == no_nightmare_state or m_state == nightmare_state);
		return m_state == nightmare_state;
	}
	using ToxicCounter = bounded::integer<1, 15>;
	constexpr auto toxic_counter() const -> ToxicCounter {
		return m_state;
	}

	// This is the equivalent of
	#if 0
	union {
		std::byte m_none{};
		bool m_nightmare;
		ToxicCounter m_toxic_counter;
	};
	#endif
	// but written to be constexpr-friendly. The discriminator is the status of
	// the active Pokemon.
	static constexpr auto nightmare_state = 1_bi;
	static constexpr auto no_nightmare_state = 2_bi;
	bounded::integer<1, 15> m_state = 1_bi;
};

} // namespace technicalmachine
