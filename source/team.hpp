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

#ifndef TEAM_HPP_
#define TEAM_HPP_

#include <cstdint>
#include <random>
#include <string>

#include "active_pokemon.hpp"
#include "entry_hazards.hpp"
#include "screens.hpp"
#include "wish.hpp"

#include "move/shared.hpp"

#include "pokemon/collection.hpp"

#include "stat/chance_to_hit.hpp"
#include "stat/stage.hpp"

namespace technicalmachine {
enum class Species : uint16_t;
class Ability;
class Rational;
class Weather;

class Team {
	public:
		Team ();
		Team (unsigned foe_size, std::mt19937 & random_engine, std::string const & team_file_name);
		Team (Team const & other);
		Team (Team && other);
		Team & operator= (Team const & other);
		Team & operator= (Team && other);
		template<class... Args>
		void add_pokemon(Args&&... args) {
			pokemon.add(shared_moves, std::forward<Args>(args)...);
		}
		void remove_pokemon ();
		
		// Not for variables that give a message at the end of the turn, this is
		// just for some book-keeping variables.
		void reset_end_of_turn();
		void reset_switch();
		void reset_between_turns();
		void update_before_move();
		void clear_field();
		void substitute();
		void lower_pp(Ability const & target);
		void activate_perish_song();
		void attract();
		void awaken(bool value);
		void increase_sleep_counter();
		bool is_loafing() const;
		bool aqua_ring_is_active() const;
		void activate_aqua_ring();
		void baton_pass();
		bool slow_start_is_active() const;
		void bounce();
		bool cannot_be_koed() const;
		void charged();
		bool charge_boosted() const;
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
		void dig();
		void dive();
		void activate_embargo();
		void decrement_embargo();
		bool is_encored() const;
		void activate_encore();
		void increment_encore();
		void endure();
		bool flash_fire_is_active() const;
		void activate_flash_fire();
		bool flinched() const;
		void flinch();
		void fly();
		void focus_energy();
		void fully_trap();
		bool leech_seeded() const;
		bool heal_block_is_active() const;
		void activate_heal_block();
		void decrement_heal_block();
		void hit_with_leech_seed();
		void decrement_lock_in();
		bool locked_on() const;
		void lock_on_to();
		void identify();
		bool ingrained() const;
		void ingrain();
		bool is_fully_paralyzed() const;
		bool magnet_rise_is_active() const;
		void activate_magnet_rise();
		void decrement_magnet_rise();
		bool me_first_is_active() const;
		bool minimized() const;
		bool missed() const;
		void set_miss(bool value);
		void move(bool value = true);
		bool moved() const;
		void activate_mud_sport();
		bool nightmare() const;
		void give_nightmares();
		void partially_trap(bool extended = false);
		void partial_trap_damage();
		void perish_song_turn();
		bool power_trick_is_active() const;
		void activate_power_trick();
		void protect();
		void break_protect();
		void activate_rampage();
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
		bool sport_is_active(Move const & foe_move) const;
		unsigned spit_up_power() const;
		void increment_stockpile();
		int release_stockpile();
		bool switch_decision_required() const;
		bool trapped() const;
		bool imprisoned() const;
		void imprison();
		bool tormented() const;
		void taunt();
		bool is_taunted() const;
		void torment();
		void increment_taunt();
		Rational toxic_ratio() const;
		void increment_toxic();
		void u_turn();
		void use_uproar();
		bool vanish_doubles_power(Moves move_name) const;
		void activate_water_sport();
		void hit_with_yawn();
		bool decrement_yawn();
		void use_bide(Pokemon & target);
		bool is_locked_in_to_bide() const;
		bool can_be_phazed () const;
		unsigned damaged() const;
		void do_damage(unsigned damage);
		bool has_switched() const;
		void update_chance_to_hit(Team const & target, Weather const & weather, bool target_moved);
		ChanceToHit::value_type chance_to_hit() const;
		ChanceToHit::value_type chance_to_miss() const;
		bool can_miss() const;

		typedef uint64_t hash_type;
		hash_type hash () const;
		std::string to_string () const;
		friend bool operator== (Team const & lhs, Team const & rhs);

		PokemonCollection pokemon;
	private:
		friend class Score;
		void load (std::string const & name, unsigned other_size);

		SharedMoves shared_moves;
		ActivePokemon active_pokemon;
	public:
		Stage stage;
		
		// Attacker that used Doom Desire / Future Sight
		// Pokemon ddfs;
		// Set to 3 initially, 1 = delayed attack hits at the end of this turn,
		// 0 = not active
		uint8_t counter = 0;

		Screens screens;
		Wish wish;
		
		EntryHazards entry_hazards;

		// Is this my team?
		bool me;
};
bool operator!= (Team const & lhs, Team const & rhs);

}	// namespace technicalmachine
#endif	// TEAM_HPP_
