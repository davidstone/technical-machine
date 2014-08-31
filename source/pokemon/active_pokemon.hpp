// ActivePokemon and MutableActivePokemon provide a view; they do not own data
// Copyright (C) 2014 David Stone
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

#ifndef ACTIVE_POKEMON_HPP_
#define ACTIVE_POKEMON_HPP_

#include "active_pokemon_flags.hpp"
#include "pokemon.hpp"
#include "pokemon.hpp"

#include "../rational.hpp"

#include "../move/moves_forward.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <utility>

namespace technicalmachine {
class Ability;
class Move;
class Weather;

class ActivePokemon {
public:
	ActivePokemon(Pokemon const & pokemon, ActivePokemonFlags const & flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	ActivePokemon(ActivePokemon const & other) = default;
	ActivePokemon(ActivePokemon && other) = default;
	ActivePokemon & operator=(ActivePokemon const & other) = delete;
	ActivePokemon & operator=(ActivePokemon && other) = delete;

	operator Pokemon const & () const {
		return m_pokemon;
	}
	operator Species() const;

	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;

	friend auto operator==(ActivePokemon const lhs, ActivePokemon rhs) -> bool;
	auto hash() const noexcept {
		return technicalmachine::hash(m_flags);
	}

private:
	Pokemon const & m_pokemon;
	ActivePokemonFlags const & m_flags;
};

class MutableActivePokemon {
public:
	MutableActivePokemon(Pokemon & pokemon, ActivePokemonFlags & flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	MutableActivePokemon(MutableActivePokemon const & other) = default;
	MutableActivePokemon(MutableActivePokemon && other) = default;
	MutableActivePokemon & operator=(MutableActivePokemon const & other) = delete;
	MutableActivePokemon & operator=(MutableActivePokemon && other) = delete;

	operator ActivePokemon() const {
		return ActivePokemon(m_pokemon, m_flags);
	}
	operator Pokemon & () {
		return m_pokemon;
	}
	operator Species() const {
		return static_cast<Species>(static_cast<ActivePokemon>(*this));
	}

	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

	auto clear_field() -> void;
	auto update_before_move() -> void;
	auto use_substitute() -> void;
	auto attract() -> void;
	auto awaken(bool value) -> void;
	auto activate_aqua_ring() -> void;
	auto baton_pass() -> void;
	auto charge() -> void;
	auto confuse() -> void;
	auto handle_confusion() -> void;
	auto set_critical_hit(bool value) -> void;
	auto curse() -> void;
	auto defense_curl() -> void;
	auto use_destiny_bond() -> void;
	auto disable() -> void;
	auto advance_disable() -> void;
	auto activate_embargo() -> void;
	auto advance_embargo() -> void;
	auto activate_encore() -> void;
	auto advance_encore() -> void;
	auto endure() -> void;
	auto faint() -> void;
	auto activate_flash_fire() -> void;
	auto flinch() -> void;
	auto focus_energy() -> void;
	auto fully_trap() -> void;
	auto activate_heal_block() -> void;
	auto advance_heal_block() -> void;
	auto hit_with_leech_seed() -> void;
	auto advance_lock_in() -> void;
	auto use_lock_on() -> void;
	auto identify() -> void;
	auto use_imprison() -> void;
	auto ingrain() -> void;
	auto activate_magnet_rise() -> void;
	auto advance_magnet_rise() -> void;
	auto set_miss(bool value) -> void;
	auto set_moved(bool value = true) -> void;
	auto activate_mud_sport() -> void;
	auto give_nightmares() -> void;
	auto partially_trap() -> void;
	auto partial_trap_damage() -> void;
	auto activate_perish_song() -> void;
	auto perish_song_turn() -> void;
	auto activate_power_trick() -> void;
	auto protect() -> void;
	auto break_protect() -> void;
	auto activate_rampage() -> void;
	auto recharge() -> bool;
	auto use_recharge_move() -> void;
	auto roost() -> void;
	auto shed_skin(bool value) -> void;
	auto increase_sleep_counter() -> void;

	auto increment_stockpile() -> void;
	auto release_stockpile() -> bounded::integer<0, Stockpile::max>;

	
	auto taunt() -> void;
	auto torment() -> void;
	auto advance_taunt() -> void;
	auto advance_toxic() -> void;
	auto u_turn() -> void;
	auto use_uproar() -> void;
	auto activate_water_sport() -> void;
	auto hit_with_yawn() -> void;
	// Advance the yawn counter and possibly put the Pokemon to sleep
	auto try_to_activate_yawn(Weather weather) -> void;

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() -> bool;
	auto dig() -> bool;
	auto dive() -> bool;
	auto fly() -> bool;
	auto shadow_force() -> bool;

	auto use_bide(Pokemon & target) -> void;

	auto direct_damage(damage_type damage) -> void;
	auto indirect_damage(damage_type damage) -> void;
	auto register_damage(damage_type damage) -> void;
	auto increment_move_use_counter() -> void;

	
private:
	Pokemon & m_pokemon;
	ActivePokemonFlags & m_flags;
};

auto operator!=(ActivePokemon lhs, ActivePokemon rhs) -> bool;

inline auto hash(ActivePokemon const pokemon) noexcept {
	return pokemon.hash();
}


inline auto last_used_move(ActivePokemon const pokemon) -> LastUsedMove {
	return pokemon.m_flags.last_used_move;
}
inline auto substitute(ActivePokemon const pokemon) -> Substitute const & {
	return pokemon.m_flags.substitute;
}


inline auto shed_skin_probability(ActivePokemon const pokemon) {
	return get_ability(pokemon).can_clear_status(get_status(pokemon)) ? 0.3 : 0.0;
}

auto lower_pp(MutableActivePokemon user, Ability target) -> void;

auto has_switched(ActivePokemon const pokemon) -> bool;

inline auto awaken_probability(ActivePokemon const pokemon) {
	return get_status(pokemon).awaken_probability(get_ability(pokemon));
}


inline auto aqua_ring_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.aqua_ring;
}

inline auto is_baton_passing(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_baton_passing;
}

inline auto cannot_be_koed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.enduring;
}

inline auto charge_boosted(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.charged and get_type(current_move(pokemon), pokemon) == Type::Electric;
}

inline auto is_confused(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.confusion.is_active();
}

inline auto critical_hit(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.critical_hit;
}

inline auto is_cursed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_cursed;
}

inline auto defense_curled(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.defense_curled;
}

inline auto is_disabled(ActivePokemon const pokemon, Moves const move_name) -> bool {
	return pokemon.m_flags.disable.move_is_disabled(*index(all_moves(pokemon), move_name));
}

inline auto is_encored(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.encore.is_active();
}

inline auto is_fainted(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_fainted;
}

inline auto flash_fire_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.flash_fire;
}

inline auto flinched(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.flinched;
}

inline auto has_focused_energy(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.has_focused_energy;
}

inline auto used_imprison(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.used_imprison;
}

inline auto ingrained(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.ingrained;
}

inline auto heal_block_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.heal_block.is_active();
}

inline auto is_fully_paralyzed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_fully_paralyzed;
}

inline auto leech_seeded(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.leech_seeded;
}

inline auto is_loafing(ActivePokemon const pokemon) -> bool {
	return get_ability(pokemon).is_loafing(pokemon.m_flags.is_loafing_turn);
}

inline auto locked_on(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.locked_on;
}

inline auto magnet_rise(ActivePokemon const pokemon) -> MagnetRise const & {
	return pokemon.m_flags.magnet_rise;
}

inline auto me_first_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.me_first_is_active;
}

inline auto minimized(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.minimized;
}

inline auto missed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.missed;
}

inline auto moved(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.moved;
}

inline auto is_having_a_nightmare(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_having_a_nightmare;
}

inline auto power_trick_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.power_trick_is_active;
}

inline auto is_recharging(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_recharging;
}

inline auto is_roosting(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_roosting;
}

inline auto shed_skin_activated(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.shed_skin_activated;
}

inline auto slow_start_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.slow_start.is_active();
}

inline auto sport_is_active(ActivePokemon const pokemon, Move const & foe_move) -> bool {
	switch (get_type(foe_move, pokemon)) {
	case Type::Electric:
		return pokemon.m_flags.mud_sport;
	case Type::Fire:
		return pokemon.m_flags.water_sport;
	default:
		return false;
	}
}

inline auto stage(ActivePokemon const pokemon) -> Stage const & {
	return pokemon.m_flags.stage;
}
inline auto stage(MutableActivePokemon pokemon) -> Stage & {
	return pokemon.m_flags.stage;
}

inline auto spit_up_power(ActivePokemon const pokemon) -> decltype(pokemon.m_flags.stockpile.spit_up_power()) {
	return pokemon.m_flags.stockpile.spit_up_power();
}

inline auto switch_decision_required(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_baton_passing or pokemon.m_flags.u_turning or is_fainted(pokemon);
}

inline auto fully_trapped(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.fully_trapped;
}

inline auto trapped(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.fully_trapped or ingrained(pokemon) or pokemon.m_flags.partial_trap.is_active();
}

inline auto is_tormented(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_tormented;
}

inline auto toxic_ratio(ActivePokemon const pokemon) -> decltype(pokemon.m_flags.toxic.ratio_drained()) {
	return pokemon.m_flags.toxic.ratio_drained();
}

inline auto is_taunted(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.taunt.is_active();
}

inline auto vanish_doubles_power(ActivePokemon const pokemon, Moves const move_name) -> bool {
	return pokemon.m_flags.vanish.doubles_move_power(move_name);
}

inline auto is_locked_in_to_bide(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.bide.is_active();
}

inline auto damaged(ActivePokemon const pokemon) -> bounded::integer<0, HP::max_value> {
	return pokemon.m_flags.damaged;
}

inline auto random_damage_multiplier(ActivePokemon const pokemon) -> decltype(pokemon.m_flags.random_damage()) {
	return pokemon.m_flags.random_damage();
}



}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
