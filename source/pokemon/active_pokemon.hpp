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
	operator Pokemon const & () const;
	operator Pokemon & ();
	operator Species() const;
	MoveCollection const & all_moves() const;
	MoveCollection & all_moves();
	template<typename... Args>
	Move const & move(Args&&... args) const {
		return all_moves()(std::forward<Args>(args)...);
	}
	bool was_used_last(Moves move) const;
	// Not for variables that give a message at the end of the turn, this is
	// just for some book-keeping variables.
	void reset_end_of_turn();
	void reset_switch();
	void reset_between_turns();
	void clear_field();
	void update_before_move();
	auto substitute() const -> Substitute const &;
	void use_substitute();
	void attract();
	void awaken(bool value);
	auto awaken_probability() const {
		return get_status(*this).awaken_probability(get_ability(*this), m_flags.awakening);
	}
	bool aqua_ring_is_active() const;
	void activate_aqua_ring();
	bool is_baton_passing() const;
	void baton_pass();
	bool cannot_be_koed() const;
	bool charge_boosted() const;
	void charge();
	bool is_confused() const;
	void confuse();
	void handle_confusion();
	bool critical_hit() const;
	void set_critical_hit(bool value);
	void curse();
	bool is_cursed() const;
	bool defense_curled() const;
	void defense_curl();
	void use_destiny_bond();
	// Requires that move is actually one of this Pokemon's moves
	bool is_disabled(Moves move) const;
	void disable();
	void advance_disable();
	void activate_embargo();
	void advance_embargo();
	bool is_encored() const;
	void activate_encore();
	void advance_encore();
	void endure();
	// This function should be used instead of checking if hp == 0 to handle
	// messages being sent about multiple Pokemon fainting in one turn.
	// Using this function will allow TM to correctly update an entire turn
	// from a message.
	bool is_fainted() const;
	void faint();
	bool flash_fire_is_active() const;
	void activate_flash_fire();
	bool flinched() const;
	void flinch();
	auto has_focused_energy() const -> bool;
	void focus_energy();
	void fully_trap();
	bool heal_block_is_active() const;
	void activate_heal_block();
	void advance_heal_block();
	bool leech_seeded() const;
	void hit_with_leech_seed();
	void clear_leech_seed();
	bool is_loafing() const;
	void advance_lock_in();
	bool locked_on() const;
	void use_lock_on();
	void identify();
	bool used_imprison() const;
	void use_imprison();
	bool ingrained() const;
	void ingrain();
	bool is_fully_paralyzed() const;
	void lower_pp(Ability const & target);
	auto magnet_rise() const -> MagnetRise const &;
	void activate_magnet_rise();
	void advance_magnet_rise();
	bool me_first_is_active() const;
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
	bool minimized() const;
	bool missed() const;
	void set_miss(bool value);
	void set_moved(bool value = true);
	bool moved() const;
	bool moved_since_switch() const;
	void activate_mud_sport();
	bool is_having_a_nightmare() const;
	void give_nightmares();
	void partially_trap();
	void partial_trap_damage();
	void activate_perish_song();
	void perish_song_turn();
	bool power_trick_is_active() const;
	void activate_power_trick();
	void protect();
	void break_protect();
	void activate_rampage();
	bool is_recharging() const;
	bool recharge();
	void use_recharge_move();
	bool is_roosting() const;
	void roost();
	bool shed_skin_activated() const;
	void shed_skin(bool value);
	void increase_sleep_counter();
	bool slow_start_is_active() const;
	bool sport_is_active(Move const & foe_move) const;

	auto stage() const -> Stage const &;
	auto stage() -> Stage &;
	
	auto spit_up_power() const {
		return m_flags.stockpile.spit_up_power();
	}
	void increment_stockpile();
	bounded::integer<0, Stockpile::max> release_stockpile();

	bool is_switching_to_self () const;
	bool is_switching_to_self(Moves switch_move) const;
	bool has_switched() const;
	bool switch_decision_required() const;
	
	auto fully_trapped() const -> bool;
	bool trapped() const;
	bool is_tormented() const;
	void taunt();
	bool is_taunted() const;
	void torment();
	void advance_taunt();
	auto toxic_ratio() const {
		return m_flags.toxic.ratio_drained();
	}
	void advance_toxic();
	void u_turn();
	void use_uproar();
	bool vanish_doubles_power(Moves move_name) const;
	void activate_water_sport();
	auto hit_with_yawn() -> void;
	// Advance the yawn counter and possibly put the Pokemon to sleep
	auto try_to_activate_yawn(Weather weather) -> void;

	// Returns whether the Pokemon ends up in a Vanished state
	bool bounce();
	bool dig();
	bool dive();
	bool fly();
	bool shadow_force();

	void use_bide(Pokemon & target);
	bool is_locked_in_to_bide() const;
	bounded::integer<0, HP::max_value> damaged() const;
	auto random_damage_multiplier() const {
		return m_flags.random_damage();
	}

	void direct_damage(damage_type damage);
	void indirect_damage(damage_type damage);
	void register_damage(damage_type damage);
	void increment_move_use_counter();
	void update_chance_to_hit(ActivePokemon const & target, Weather weather, bool target_moved);
	// If the move is a hit, returns the chance to hit, otherwise, returns
	// the chance to miss.
	auto accuracy_probability() const -> ChanceToHit;
	
	bool will_be_replaced() const;
	
	// Fix any rounding issues caused by not seeing the foe's exact HP.
	void normalize_hp(bool fainted);
	
	PokemonCollection const & all_pokemon() const;
	PokemonCollection & all_pokemon();

	friend bool operator== (ActivePokemon const & lhs, ActivePokemon const & rhs);
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
bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs);

void switch_pokemon(ActivePokemon & pokemon);

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


}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
