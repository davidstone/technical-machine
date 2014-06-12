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

#include "collection.hpp"
#include "confusion.hpp"
#include "disable.hpp"
#include "embargo.hpp"
#include "encore.hpp"
#include "heal_block.hpp"
#include "last_used_move.hpp"
#include "magnet_rise.hpp"
#include "partial_trap.hpp"
#include "perish_song.hpp"
#include "rampage.hpp"
#include "slow_start.hpp"
#include "stockpile.hpp"
#include "substitute.hpp"
#include "taunt.hpp"
#include "toxic.hpp"
#include "uproar.hpp"
#include "vanish.hpp"
#include "yawn.hpp"

#include "../damage.hpp"
#include "../random_damage.hpp"

#include "../bide/bide.hpp"

#include "../stat/chance_to_hit.hpp"
#include "../stat/stage.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <cstdint>

namespace technicalmachine {
class Ability;
class Move;
class Pokemon;
class Rational;
class Weather;
template<typename Numerator, typename Denominator>
class bounded_rational;

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
	Rational awaken_probability() const;
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
	PokemonCollection::index_type index() const;
	bool ingrained() const;
	void ingrain();
	bool is_fully_paralyzed() const;
	void lower_pp(Ability const & target);
	auto magnet_rise() const -> MagnetRise const &;
	void activate_magnet_rise();
	void advance_magnet_rise();
	bool me_first_is_active() const;
	auto fury_cutter_power() const {
		return m_last_used_move.fury_cutter_power();
	}
	auto momentum_move_power() const {
		return m_last_used_move.momentum_move_power();
	}
	auto triple_kick_power() const {
		return m_last_used_move.triple_kick_power();
	}
	auto metronome_boost() const {
		return m_last_used_move.metronome_boost();
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
	bool can_be_phazed() const;
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
	Rational shed_skin_probability() const;
	void increase_sleep_counter();
	bool slow_start_is_active() const;
	bool sport_is_active(Move const & foe_move) const;

	auto stage() const -> Stage const &;
	auto stage() -> Stage &;
	
	auto spit_up_power() const {
		return m_stockpile.spit_up_power();
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
		return m_toxic.ratio_drained();
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
	Rational random_damage_multiplier() const;
	void direct_damage(damage_type damage);
	void indirect_damage(damage_type damage);
	void register_damage(damage_type damage);
	void increment_move_use_counter();
	void update_chance_to_hit(ActivePokemon const & target, Weather const & weather, bool target_moved);
	// If the move is a hit, returns the chance to hit, otherwise, returns
	// the chance to miss.
	auto accuracy_probability() const -> ChanceToHit;
	
	bool will_be_replaced() const;
	
	// Fix any rounding issues caused by not seeing the foe's exact HP.
	void normalize_hp(bool fainted);
	
	PokemonCollection const & all_pokemon() const;
	PokemonCollection & all_pokemon();

	friend bool operator== (ActivePokemon const & lhs, ActivePokemon const & rhs);
	typedef uint64_t hash_type;
	hash_type hash() const;
	hash_type max_hash() const;

private:
	// I'd make this a reference but I don't want to manually define a copy
	// and move assignment operator to simply verify that the referents are
	// the same.
	PokemonCollection m_all_pokemon;
	HP::current_type m_damaged = 0_bi;
	Bide m_bide;
	ChanceToHit m_chance_to_hit = ChanceToHit(100_bi, 100_bi);
	Confusion m_confusion;
	Disable m_disable;
	Embargo m_embargo;
	Encore m_encore;
	HealBlock m_heal_block;
	LastUsedMove m_last_used_move;
	MagnetRise m_magnet_rise;
	Substitute m_substitute;
	PartialTrap m_partial_trap;
	PerishSong m_perish_song;
	Rampage m_rampage;
	RandomDamage m_random_damage;
	Stage m_stage;
	SlowStart m_slow_start;
	Stockpile m_stockpile;
	Taunt m_taunt;
	Toxic m_toxic;
	Uproar m_uproar;
	Vanish m_vanish;
	Yawn m_yawn;
	bool m_aqua_ring = false;
	bool m_attracted = false;
	// Will it wake up
	bool m_awakening = false;
	bool m_charged = false;
	bool m_critical_hit = false;
	bool m_is_cursed = false;
	bool m_defense_curled = false;
	bool m_destiny_bond = false;
	bool m_enduring = false;
	bool m_flash_fire = false;
	bool m_flinched = false;
	bool m_has_focused_energy = false;
	bool m_is_fully_paralyzed = false;
	// Block, Mean Look, Spider Web
	bool m_fully_trapped = false;
	bool m_gastro_acid = false;
	bool m_identified = false;
	bool m_used_imprison = false;
	bool m_ingrained = false;
	bool m_leech_seeded = false;
	bool m_is_loafing_turn = false;
	bool m_locked_on = false;
	bool m_me_first_is_active = false;
	bool m_minimized = false;
	bool m_missed = false;
	bool m_moved = false;
	bool m_mud_sport = false;
	bool m_is_having_a_nightmare = false;
	bool m_is_baton_passing = false;
	bool m_power_trick_is_active = false;
	bool m_is_protecting = false;
	bool m_is_recharging = false;
	bool m_is_roosting = false;
	bool m_shed_skin_activated = false;
	bool m_is_tormented = false;
	bool m_u_turning = false;
	bool m_water_sport = false;
	bool m_will_be_replaced = false;
};
bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs);

void switch_pokemon(ActivePokemon & pokemon);

}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
