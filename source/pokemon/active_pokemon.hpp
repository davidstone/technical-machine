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

#include <cstdint>

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
	template<typename... Args>
	Move const & regular_move(Args && ... args) const {
		auto const & self = static_cast<Pokemon const &>(*this);
		return all_moves().regular_move(std::forward<Args>(args)...);
	}
	template<typename... Args>
	Move & regular_move(Args && ... args) {
		auto const & self = static_cast<Pokemon const &>(*this);
		return all_moves().regular_move(std::forward<Args>(args)...);
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
	void decrement_embargo();
	bool is_encored() const;
	void activate_encore();
	void increment_encore();
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
	void decrement_heal_block();
	bool leech_seeded() const;
	void hit_with_leech_seed();
	void clear_leech_seed();
	bool is_loafing() const;
	void decrement_lock_in();
	bool locked_on() const;
	void lock_on_to();
	void identify();
	bool imprisoned() const;
	void imprison();
	PokemonCollection::index_type index() const;
	bool ingrained() const;
	void ingrain();
	bool is_fully_paralyzed() const;
	void lower_pp(Ability const & target);
	auto magnet_rise() const -> MagnetRise const &;
	void activate_magnet_rise();
	void decrement_magnet_rise();
	bool me_first_is_active() const;
	bounded_integer::native_integer<10, 160> fury_cutter_power() const;
	bounded_integer::native_integer<30, 480> momentum_move_power() const;
	bounded_integer::native_integer<0, 30> triple_kick_power() const;
	bounded_rational<bounded_integer::native_integer<10, 20>, bounded_integer::native_integer<10, 10>> metronome_boost() const;
	bool minimized() const;
	bool missed() const;
	void set_miss(bool value);
	void set_moved(bool value = true);
	bool moved() const;
	bool moved_since_switch() const;
	void activate_mud_sport();
	bool nightmare() const;
	void give_nightmares();
	void partially_trap(bool extended);
	void partial_trap_damage();
	void activate_perish_song();
	void perish_song_turn();
	bool can_be_phazed() const;
	bool power_trick_is_active() const;
	void activate_power_trick();
	void protect();
	void break_protect();
	void activate_rampage();
	bool recharging() const;
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
	
	bounded_integer::native_integer<0, Stockpile::max * 100> spit_up_power() const;
	void increment_stockpile();
	bounded_integer::native_integer<0, Stockpile::max> release_stockpile();

	bool is_switching_to_self () const;
	bool is_switching_to_self(Moves switch_move) const;
	bool has_switched() const;
	bool switch_decision_required() const;
	
	bool trapped() const;
	bool tormented() const;
	void taunt();
	bool is_taunted() const;
	void torment();
	void increment_taunt();
	auto toxic_ratio() const {
		return toxic.ratio_drained();
	}
	void increment_toxic();
	void u_turn();
	void use_uproar();
	bool vanish_doubles_power(Moves move_name) const;
	void activate_water_sport();
	void hit_with_yawn();
	bool decrement_yawn();

	// Returns whether the Pokemon ends up in a Vanished state
	bool bounce();
	bool dig();
	bool dive();
	bool fly();
	bool shadow_force();

	void use_bide(Pokemon & target);
	bool is_locked_in_to_bide() const;
	bounded_integer::native_integer<0, HP::max_value> damaged() const;
	Rational random_damage_multiplier() const;
	void direct_damage(damage_type damage);
	void indirect_damage(damage_type damage);
	void register_damage(damage_type damage);
	void increment_move_use_counter();
	void update_chance_to_hit(ActivePokemon const & target, Weather const & weather, bool target_moved);
	// If the move is a hit, returns the chance to hit, otherwise, returns
	// the chance to miss.
	Rational accuracy_probability() const;
	
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
	friend class Evaluate;
	// I'd make this a reference but I don't want to manually define a copy
	// and move assignment operator to simply verify that the referents are
	// the same.
	PokemonCollection m_all_pokemon;
	HP::current_type damage_done_to_active = 0_bi;
	Bide bide;
	ChanceToHit cached_chance_to_hit;
	Confusion confusion;
	Disable m_disable;
	Embargo embargo;
	Encore encore;
	HealBlock heal_block;
	LastUsedMove last_used_move;
	MagnetRise m_magnet_rise;
	Substitute m_substitute;
	PartialTrap partial_trap;
	PerishSong perish_song;
	Rampage rampage;
	RandomDamage random_damage;
	Stage m_stage;
	SlowStart slow_start;
	Stockpile stockpile;
	Taunt m_taunt;
	Toxic toxic;
	Uproar uproar;
	Vanish vanish;
	Yawn yawn;
	bool aqua_ring = false;
	bool attracted = false;
	// Will it wake up
	bool awakening = false;
	bool ch = false;
	bool charged = false;
	bool cursed = false;
	bool used_defense_curl = false;
	bool destiny_bond = false;
	bool enduring = false;
	bool flash_fire = false;
	bool flinched_this_turn = false;
	bool focusing_energy = false;
	bool fully_paralyzed = false;
	// Block, Mean Look, Spider Web
	bool fully_trapped = false;
	bool gastro_acid = false;
	bool identified = false;
	bool used_imprison = false;
	bool ingrain_active = false;
	bool leech_seed = false;
	bool loaf = false;
	bool lock_on = false;
	bool me_first = false;
	bool minimize = false;
	bool miss = false;
	bool has_moved = false;
	bool mud_sport = false;
	bool nightmares = false;
	bool pass = false;
	bool power_trick = false;
	bool protecting = false;
	bool recharge_lock_in = false;
	bool roosting = false;
	bool shedding_skin = false;
	bool is_tormented = false;
	bool u_turning = false;
	bool water_sport = false;
	bool m_will_be_replaced = false;
};
bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs);

void switch_pokemon(ActivePokemon & pokemon);

}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
