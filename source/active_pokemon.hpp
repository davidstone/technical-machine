// Team data structure
// Copyright (C) 2012 David Stone
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

#include "confusion.hpp"
#include "embargo.hpp"
#include "partial_trap.hpp"
#include "perish_song.hpp"
#include "substitute.hpp"
#include "taunt.hpp"
#include "toxic.hpp"
#include "uproar.hpp"
#include "vanish.hpp"
#include "yawn.hpp"

#include "bide/bide.hpp"

#include "stat/chance_to_hit.hpp"
#include "stat/stage.hpp"

namespace technicalmachine {
class Ability;
class Move;
class Pokemon;
class Rational;
class Team;
class Weather;

class ActivePokemon {
	public:
		// Not for variables that give a message at the end of the turn, this is
		// just for some book-keeping variables.
		void reset_end_of_turn();
		void reset_switch();
		void reset_between_turns();
		void clear_field();
		void update_before_move();
		bool substitute(unsigned max_hp);
		void attract();
		void awaken(bool value);
		bool aqua_ring_is_active() const;
		void activate_aqua_ring();
		bool is_baton_passing() const;
		void baton_pass();
		void bounce();
		bool cannot_be_koed() const;
		bool charge_boosted() const;
		void charge();
		bool is_confused() const;
		void confuse();
		void handle_confusion(Pokemon & pokemon);
		bool critical_hit() const;
		void set_critical_hit(bool value);
		void curse();
		bool is_cursed() const;
		bool defense_curled() const;
		void defense_curl();
		void use_destiny_bond();
		void dig();
		void dive();
		void activate_embargo();
		void decrement_embargo();
		void endure();
		bool flash_fire_is_active() const;
		void activate_flash_fire();
		bool flinched() const;
		void flinch();
		void fly();
		void focus_energy();
		void fully_trap();
		bool leech_seeded() const;
		void hit_with_leech_seed();
		void clear_leech_seed();
		bool is_loafing(Ability const & ability) const;
		bool locked_on() const;
		void lock_on_to();
		void identify();
		bool imprisoned() const;
		void imprison();
		bool ingrained() const;
		void ingrain();
		bool is_fully_paralyzed() const;
		bool me_first_is_active() const;
		bool minimized() const;
		bool missed() const;
		void set_miss(bool value);
		void move(bool value = true);
		bool moved() const;
		void activate_mud_sport();
		bool nightmare() const;
		void give_nightmares();
		void partially_trap(bool extended);
		void partial_trap_damage(Pokemon & pokemon);
		void activate_perish_song();
		bool perish_song_turn();
		bool power_trick_is_active() const;
		void activate_power_trick();
		void protect();
		void break_protect();
		bool recharging() const;
		bool recharge();
		void use_recharge_move();
		bool replacing() const;
		void not_replacing();
		bool is_roosting() const;
		void roost();
		void shadow_force();
		bool shed_skin_activated() const;
		void shed_skin(bool value);
		void increase_sleep_counter(Pokemon & pokemon);
		bool sport_is_active(Move const & foe_move) const;
		bool switch_decision_required() const;
		bool trapped() const;
		bool tormented() const;
		void taunt();
		bool is_taunted() const;
		void torment();
		void increment_taunt();
		Rational toxic_ratio() const;
		void increment_toxic();
		void u_turn();
		void use_uproar();
		void increment_uproar();
		bool vanish_doubles_power(Moves move_name) const;
		void activate_water_sport();
		void hit_with_yawn();
		bool decrement_yawn();
		void use_bide(Pokemon & target);
		bool is_locked_in_to_bide() const;
		unsigned damaged() const;
		void do_damage(unsigned damage);
		void update_chance_to_hit(Team const & user, Team const & target, Weather const & weather, bool target_moved);
		ChanceToHit::value_type chance_to_hit() const;
		ChanceToHit::value_type chance_to_miss() const;
		bool can_miss() const;

		friend bool operator== (ActivePokemon const & lhs, ActivePokemon const & rhs);
		typedef uint64_t hash_type;
		hash_type hash() const;
		hash_type max_hash() const;

	private:
		friend class Score;
		uint16_t damage_done_to_active = 0;
		Bide bide;
		ChanceToHit cached_chance_to_hit;
		Confusion confusion;
		Embargo embargo;
		Substitute active_substitute;
		PartialTrap partial_trap;
		PerishSong perish_song;
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
		// Replacing a fainted Pokemon. Also used for initial switch-in at
		// start of battle.
		bool is_replacing = false;
		bool roosting = false;
		bool shedding_skin = false;
		bool is_tormented = false;
		bool u_turning = false;
		bool water_sport = false;
};
bool operator!= (ActivePokemon const & lhs, ActivePokemon const & rhs);

}	// namespace technicalmachine
#endif	// ACTIVE_POKEMON_HPP_
