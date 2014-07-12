// Flags for the active Pokemon
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
#include "collection.hpp"

#include "../rational.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <utility>

namespace technicalmachine {
class Ability;
class Move;
class Pokemon;
class Weather;

class ActivePokemon {
public:
	operator Pokemon const & () const {
		return all_pokemon()();
	}
	operator Pokemon & () {
		return all_pokemon()();
	}
	operator Species() const;

	auto was_used_last(Moves move) const -> bool;
	// Not for variables that give a message at the end of the turn, this is
	// just for some book-keeping variables.
	auto reset_end_of_turn() -> void;
	auto reset_switch() -> void;
	auto reset_between_turns() -> void;
	auto clear_field() -> void;
	auto update_before_move() -> void;
	auto substitute() const -> Substitute const &;
	auto use_substitute() -> void;
	auto attract() -> void;
	auto awaken(bool value) -> void;
	auto awaken_probability() const {
		return get_status(*this).awaken_probability(get_ability(*this), m_flags.awakening);
	}
	auto aqua_ring_is_active() const -> bool;
	auto activate_aqua_ring() -> void;
	auto is_baton_passing() const -> bool;
	auto baton_pass() -> void;
	auto cannot_be_koed() const -> bool;
	auto charge_boosted() const -> bool;
	auto charge() -> void;
	auto is_confused() const -> bool;
	auto confuse() -> void;
	auto handle_confusion() -> void;
	auto critical_hit() const -> bool;
	auto set_critical_hit(bool value) -> void;
	auto curse() -> void;
	auto is_cursed() const -> bool;
	auto defense_curled() const -> bool;
	auto defense_curl() -> void;
	auto use_destiny_bond() -> void;
	// Requires that move is actually one of this Pokemon's moves
	auto is_disabled(Moves move) const -> bool;
	auto disable() -> void;
	auto advance_disable() -> void;
	auto activate_embargo() -> void;
	auto advance_embargo() -> void;
	auto is_encored() const -> bool;
	auto activate_encore() -> void;
	auto advance_encore() -> void;
	auto endure() -> void;
	// This function should be used instead of checking if hp == 0 to handle
	// messages being sent about multiple Pokemon fainting in one turn.
	// Using this function will allow TM to correctly update an entire turn
	// from a message.
	auto is_fainted() const -> bool;
	auto faint() -> void;
	auto flash_fire_is_active() const -> bool;
	auto activate_flash_fire() -> void;
	auto flinched() const -> bool;
	auto flinch() -> void;
	auto has_focused_energy() const -> bool;
	auto focus_energy() -> void;
	auto fully_trap() -> void;
	auto heal_block_is_active() const -> bool;
	auto activate_heal_block() -> void;
	auto advance_heal_block() -> void;
	auto leech_seeded() const -> bool;
	auto hit_with_leech_seed() -> void;
	auto is_loafing() const -> bool;
	auto advance_lock_in() -> void;
	auto locked_on() const -> bool;
	auto use_lock_on() -> void;
	auto identify() -> void;
	auto used_imprison() const -> bool;
	auto use_imprison() -> void;
	auto ingrained() const -> bool;
	auto ingrain() -> void;
	auto is_fully_paralyzed() const -> bool;
	auto magnet_rise() const -> MagnetRise const &;
	auto activate_magnet_rise() -> void;
	auto advance_magnet_rise() -> void;
	auto me_first_is_active() const -> bool;
	auto fury_cutter_power() const {
		return m_flags.last_used_move.fury_cutter_power();
	}
	auto momentum_move_power() const {
		return m_flags.last_used_move.momentum_move_power();
	}
	auto triple_kick_power() const {
		return m_flags.last_used_move.triple_kick_power();
	}
	auto metronome_boost() const {
		return m_flags.last_used_move.metronome_boost();
	}
	auto minimized() const -> bool;
	auto missed() const -> bool;
	auto set_miss(bool value) -> void;
	auto set_moved(bool value = true) -> void;
	auto moved() const -> bool;
	auto moved_since_switch() const -> bool;
	auto activate_mud_sport() -> void;
	auto is_having_a_nightmare() const -> bool;
	auto give_nightmares() -> void;
	auto partially_trap() -> void;
	auto partial_trap_damage() -> void;
	auto activate_perish_song() -> void;
	auto perish_song_turn() -> void;
	auto power_trick_is_active() const -> bool;
	auto activate_power_trick() -> void;
	auto protect() -> void;
	auto break_protect() -> void;
	auto activate_rampage() -> void;
	auto is_recharging() const -> bool;
	auto recharge() -> bool;
	auto use_recharge_move() -> void;
	auto is_roosting() const -> bool;
	auto roost() -> void;
	auto shed_skin_activated() const -> bool;
	auto shed_skin(bool value) -> void;
	auto increase_sleep_counter() -> void;
	auto slow_start_is_active() const -> bool;
	auto sport_is_active(Move const & foe_move) const -> bool;

	auto stage() const -> Stage const &;
	auto stage() -> Stage &;
	
	auto spit_up_power() const {
		return m_flags.stockpile.spit_up_power();
	}
	auto increment_stockpile() -> void;
	auto release_stockpile() -> bounded::integer<0, Stockpile::max>;

	auto switch_decision_required() const -> bool;
	
	auto fully_trapped() const -> bool;
	auto trapped() const -> bool;
	auto is_tormented() const -> bool;
	auto taunt() -> void;
	auto is_taunted() const -> bool;
	auto torment() -> void;
	auto advance_taunt() -> void;
	auto toxic_ratio() const {
		return m_flags.toxic.ratio_drained();
	}
	auto advance_toxic() -> void;
	auto u_turn() -> void;
	auto use_uproar() -> void;
	auto vanish_doubles_power(Moves move_name) const -> bool;
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
	auto is_locked_in_to_bide() const -> bool;
	auto damaged() const -> bounded::integer<0, HP::max_value>;
	auto random_damage_multiplier() const {
		return m_flags.random_damage();
	}

	auto direct_damage(damage_type damage) -> void;
	auto indirect_damage(damage_type damage) -> void;
	auto register_damage(damage_type damage) -> void;
	auto increment_move_use_counter() -> void;
	auto update_chance_to_hit(ActivePokemon const & target, Weather weather, bool target_moved) -> void;
	// If the move is a hit, returns the chance to hit, otherwise, returns
	// the chance to miss.
	auto accuracy_probability() const -> ChanceToHit;
	
	auto will_be_replaced() const -> bool;
	
	auto all_pokemon() const -> PokemonCollection const & {
		return m_all_pokemon;
	}
	auto all_pokemon() -> PokemonCollection & {
		return m_all_pokemon;
	}

	friend auto operator==(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool;
	auto hash() const noexcept {
		return technicalmachine::hash(m_flags);
	}

private:
	// I'd make this a reference but I don't want to manually define a copy
	// and move assignment operator to simply verify that the referents are
	// the same.
	PokemonCollection m_all_pokemon;
	ActivePokemonFlags m_flags;
};
auto operator!=(ActivePokemon const & lhs, ActivePokemon const & rhs) -> bool;

inline auto hash(ActivePokemon const & pokemon) noexcept {
	return pokemon.hash();
}

inline auto shed_skin_probability(ActivePokemon const & pokemon) {
	if (!get_ability(pokemon).can_clear_status(get_status(pokemon))) {
		return make_rational(BOUNDED_CONDITIONAL(pokemon.shed_skin_activated(), 0_bi, 10_bi), 10_bi);
	}
	auto const numerator = BOUNDED_CONDITIONAL(pokemon.shed_skin_activated(), 3_bi, 7_bi);
	return make_rational(static_cast<bounded::integer<0, 10>>(numerator), 10_bi);
}

auto lower_pp(ActivePokemon & user, Ability target) -> void;

auto has_switched(ActivePokemon const & pokemon) -> bool;

}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
