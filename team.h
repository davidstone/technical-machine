// Team data structure
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TEAM_H_
#define TEAM_H_

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include "active.h"
#include "move.h"

namespace technicalmachine {

class Map;
class Pokemon;

class Team {
	public:
		std::string player;						// The player's name
		Active<Pokemon> pokemon;

		Vanish vanish;
		int16_t damage;		// How much damage will this Pokemon do
		int16_t bide_damage;
		int16_t substitute;	// How much HP does the substitute have left?
		int8_t bide;
		int16_t chance_to_hit;
		int8_t confused;
		int8_t embargo;
		int8_t encore;
		int8_t heal_block;
		int8_t magnet_rise;
		int8_t partial_trap;		// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm, Sand Tomb, Whirlpool, and Wrap
		int8_t perish_song;
		int8_t rampage;			// Number of turns remaining on Outrage, Petal Dance, and Thrash
		int8_t slow_start;
		int8_t stockpile;
		int8_t taunt;
		int8_t toxic;				// Number of turns this Pokemon has already taken Toxic damage (or would have if Magic Guard / Poison Heal weren't in play)
		int8_t uproar;			// Number of turns remaining on Uproar
		int8_t yawn;
		bool aqua_ring;
		bool attract;
		bool awaken;		// Will it wake up
		bool ch;
		bool charge;
		bool curse;
		bool damaged;
		bool defense_curl;
		bool destiny_bond;
		bool endure;
		bool ff;				// Flash Fire
		bool flinch;
		bool focus_energy;
		bool fully_paralyzed;
		bool gastro_acid;
		bool hitself;			// Will this Pokemon hit itself in its confusion this turn?
		bool identified;
		bool imprison;			// Has this Pokemon used Imprison?
		bool ingrain;
		bool leech_seed;
		bool loaf;
		bool lock_on;
		bool mf;				// Me First
		bool minimize;
		bool miss;
		bool moved;
		bool mud_sport;
		bool nightmare;
		bool pass;
		bool power_trick;
		bool protect;
		bool recharging;
		bool replacing;	// Replacing a fainted Pokemon. Also used for initial switch-in at start of battle.
		bool roost;
		bool shed_skin;
		bool torment;
		bool trapped;			// Block, Mean Look, Spider Web
		bool water_sport;
		int8_t accuracy;
		int8_t evasion;
	
	//	Pokemon ddfs;		// Information about the attacker at the time of using Doom Desire / Future Sight
		int8_t counter;			// Set to 3 initially, 1 = delayed attack hits at the end of this turn, 0 = not active

		// Number of turns remaining
		int8_t light_screen;
		int8_t lucky_chant;
		int8_t mist;
		int8_t reflect;
		int8_t safeguard;
		int8_t tailwind;

		int8_t wish;		// 2 = Wish will heal next turn, 1 = Wish will heal this turn, 0 = Wish is not in play
	
		// The entry hazards represent those down on this team's side of the field. Using an entry hazard puts them down on the opponent's side of the field
		// Number of layers of Spikes / Toxic Spikes
		int8_t spikes;
		int8_t toxic_spikes;
		bool stealth_rock;

		uint8_t replacement;		// If a Pokemon switches / faints, what Pokemon should replace it?
		
		uint8_t size;			// How big is the team?
		
		bool me;				// Is this my team?
	
		Team (bool isme, Map const &map, unsigned size);
		bool operator== (Team const &other) const;
		Pokemon& at_replacement ();
		Pokemon const & at_replacement () const;
		uint64_t hash () const;
		void output (std::string &output);
	private:
		void load (std::string const &name, Map const &map, unsigned size);
};

}
#endif
