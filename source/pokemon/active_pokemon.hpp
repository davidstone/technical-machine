// ActivePokemon and MutableActivePokemon provide a view; they do not own data
// Copyright (C) 2017 David Stone
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

#pragma once

#include "active_pokemon_flags.hpp"
#include "pokemon.hpp"
#include "pokemon.hpp"

#include "../operators.hpp"
#include "../rational.hpp"

#include "../move/moves.hpp"
#include "../move/is_switch.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {
struct Ability;
struct Move;
struct Weather;

struct ActivePokemon {
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
	operator Species() const {
		return static_cast<Pokemon const &>(*this);
	}

	TECHNICALMACHINE_ACTIVE_POKEMON_FRIEND_FUNCTIONS;

	friend auto operator==(ActivePokemon const lhs, ActivePokemon const rhs) {
		return lhs.m_flags == rhs.m_flags;
	}

private:
	Pokemon const & m_pokemon;
	ActivePokemonFlags const & m_flags;
};

struct MutableActivePokemon {
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
	operator Pokemon & () const {
		return m_pokemon;
	}
	operator Species() const {
		return static_cast<Species>(static_cast<ActivePokemon>(*this));
	}

	friend auto stage(MutableActivePokemon pokemon) -> Stage &;

	auto clear_field() {
		m_flags.leech_seeded = false;
		m_flags.partial_trap = {};
	}
	auto update_before_move() {
		m_flags.destiny_bond = false;
		m_flags.locked_on = false;
		m_flags.moved = true;
	}

	auto activate_aqua_ring() {
		m_flags.aqua_ring = true;
	}
	auto attract() {
		m_flags.attracted = true;
	}
	auto baton_pass() {
		m_flags.is_baton_passing = true;
	}
	auto charge() {
		m_flags.charged = true;
	}
	auto confuse() -> void;
	auto handle_confusion() {
		m_flags.confusion.do_turn(*this);
	}
	auto curse() {
		m_flags.is_cursed = true;
	}
	auto defense_curl() {
		m_flags.defense_curled = true;
	}
	auto use_destiny_bond() {
		m_flags.destiny_bond = true;
	}
	auto disable(Moves const move) {
		if (is_regular(move)) {
			m_flags.disable.activate(move);
		}
	}
	auto advance_disable() {
		m_flags.disable.advance_one_turn();
	}
	auto activate_embargo() {
		m_flags.embargo.activate();
	}
	auto advance_embargo() {
		m_flags.embargo.advance_one_turn();
	}
	auto activate_encore() {
		m_flags.encore.activate();
	}
	auto advance_encore() {
		m_flags.encore.advance_one_turn();
	}
	auto endure() {
		m_flags.enduring = true;
	}
	auto faint() {
		get_hp(*this) = 0_bi;
		m_flags.is_fainted = true;
	}
	auto activate_flash_fire() {
		m_flags.flash_fire = true;
	}
	auto flinch() {
		m_flags.flinched = true;
	}
	auto focus_energy() {
		m_flags.has_focused_energy = true;
	}
	auto fully_trap() {
		m_flags.fully_trapped = true;
	}
	auto activate_heal_block() {
		m_flags.heal_block.activate();
	}
	auto advance_heal_block() {
		m_flags.heal_block.advance_one_turn();
	}
	auto identify() {
		m_flags.identified = true;
	}
	auto use_imprison() {
		m_flags.used_imprison = true;
	}
	auto ingrain() {
		m_flags.ingrained = true;
	}
	auto hit_with_leech_seed() {
		m_flags.leech_seeded = true;
	}
	
	auto advance_lock_in() -> void;
	auto use_lock_on() {
		m_flags.locked_on = true;
	}
	auto activate_magnet_rise() {
		m_flags.magnet_rise.activate();
	}
	auto advance_magnet_rise() {
		m_flags.magnet_rise.advance_one_turn();
	}
	auto set_moved(bool const value) {
		m_flags.moved = value;
	}
	auto activate_mud_sport() {
		m_flags.mud_sport = true;
	}
	auto give_nightmares() {
		m_flags.is_having_a_nightmare = true;
	}
	auto partially_trap() {
		m_flags.partial_trap.activate();
	}
	auto partial_trap_damage() {
		m_flags.partial_trap.damage(*this);
	}
	auto activate_perish_song() {
		m_flags.perish_song.activate();
	}
	auto perish_song_turn() -> void;
	auto activate_power_trick() {
		m_flags.power_trick_is_active = !m_flags.power_trick_is_active;
	}
	auto protect() {
		m_flags.is_protecting = true;
	}
	auto break_protect() {
		m_flags.is_protecting = false;
	}
	auto activate_rampage() {
		m_flags.rampage.activate();
	}
	auto recharge() -> bool;
	auto use_recharge_move() {
		m_flags.is_recharging = true;
	}
	auto roost() {
		m_flags.is_roosting = true;
	}
	auto increase_sleep_counter(bool awakens) -> void;

	auto increment_stockpile() -> void;
	auto release_stockpile() -> bounded::integer<0, Stockpile::max>;

	auto use_substitute() -> void;

	auto taunt() {
		m_flags.taunt.activate();
	}
	auto advance_taunt() {
		m_flags.taunt.advance_one_turn();
	}
	auto torment() {
		m_flags.is_tormented = true;
	}
	auto advance_toxic() {
		m_flags.toxic.increment();
	}
	auto u_turn() {
		m_flags.u_turning = true;
	}
	auto use_uproar() {
		m_flags.uproar.advance_one_turn();
	}
	auto activate_water_sport() {
		m_flags.water_sport = true;
	}
	auto hit_with_yawn() {
		m_flags.yawn.activate();
	}
	// Advance the yawn counter and possibly put the Pokemon to sleep
	auto try_to_activate_yawn(Weather weather) -> void;

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() {
		return m_flags.vanish.bounce();
	}
	auto dig() {
		return m_flags.vanish.dig();
	}
	auto dive() {
		return m_flags.vanish.dive();
	}
	auto fly() {
		return m_flags.vanish.fly();
	}
	auto shadow_force() {
		return m_flags.vanish.shadow_force();
	}


	auto use_bide(Pokemon & target) -> void;

	auto direct_damage(damage_type damage) -> void;
	auto indirect_damage(damage_type const damage) {
		get_hp(*this) -= damage;
	}
	auto register_damage(damage_type const damage) {
		m_flags.damaged = damage;
	}
	auto increment_move_use_counter(Moves const move) {
		m_flags.last_used_move.increment(move);
	}
	
private:
	Pokemon & m_pokemon;
	ActivePokemonFlags & m_flags;
};


inline auto last_used_move(ActivePokemon const pokemon) -> LastUsedMove {
	return pokemon.m_flags.last_used_move;
}
inline auto substitute(ActivePokemon const pokemon) -> Substitute const & {
	return pokemon.m_flags.substitute;
}


inline auto shed_skin_probability(ActivePokemon const pokemon) {
	return get_ability(pokemon).can_clear_status(get_status(pokemon)) ? 0.3 : 0.0;
}

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

inline auto charge_boosted(ActivePokemon const pokemon, Moves const move) -> bool {
	return pokemon.m_flags.charged and get_type(move, pokemon) == Type::Electric;
}

inline auto is_confused(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.confusion.is_active();
}

inline auto is_cursed(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.is_cursed;
}

inline auto defense_curled(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.defense_curled;
}

inline auto is_disabled(ActivePokemon const pokemon, Moves const move_name) -> bool {
	return pokemon.m_flags.disable.move_is_disabled(move_name);
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

inline auto slow_start_is_active(ActivePokemon const pokemon) -> bool {
	return pokemon.m_flags.slow_start.is_active();
}

inline auto sport_is_active(ActivePokemon const pokemon, Moves const foe_move) -> bool {
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
