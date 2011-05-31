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

#include <fstream>
#include <string>
#include <vector>
#include "active.h"
#include "map.h"
#include "pokemon.h"

namespace technicalmachine {

class Team {
	public:
		std::string player;						// The player's name
		Active<Pokemon> pokemon;

		Vanish vanish;
		short damage;		// How much damage will this Pokemon do
		short bide_damage;
		short substitute;	// How much HP does the substitute have left?
		char bide;
		char confused;
		char embargo;
		char encore;
		char heal_block;
		char magnet_rise;
		char partial_trap;		// Number of turns remaining on Bind, Clamp, Fire Spin, Magma Storm, Sand Tomb, Whirlpool, and Wrap
		char perish_song;
		char rampage;			// Number of turns remaining on Outrage, Petal Dance, and Thrash
		char slow_start;
		char stockpile;
		char taunt;
		char toxic;				// Number of turns this Pokemon has already taken Toxic damage (or would have if Magic Guard / Poison Heal weren't in play)
		char uproar;			// Number of turns remaining on Uproar
		char yawn;
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
		bool recharging;
		bool replacing;	// Replacing a fainted Pokemon. Also used for initial switch-in at start of battle.
		bool roost;
		bool shed_skin;
		bool torment;
		bool trapped;			// Block, Mean Look, Spider Web
		bool water_sport;
		char accuracy;
		char evasion;
	
	//	Pokemon ddfs;		// Information about the attacker at the time of using Doom Desire / Future Sight
		char counter;			// Set to 3 initially, 1 = delayed attack hits at the end of this turn, 0 = not active

		// Number of turns remaining
		char light_screen;
		char lucky_chant;
		char mist;
		char reflect;
		char safeguard;
		char tailwind;

		char wish;		// 2 = Wish will heal next turn, 1 = Wish will heal this turn, 0 = Wish is not in play
	
		// The entry hazards represent those down on this team's side of the field. Using an entry hazard puts them down on the opponent's side of the field
		// Number of layers of Spikes / Toxic Spikes
		char spikes;
		char toxic_spikes;
		bool stealth_rock;

		unsigned char replacement;		// If a Pokemon switches / faints, what Pokemon should replace it?
		
		unsigned char size;			// How big is the team?
		
		bool const me;				// Is this my team?
	
		Team (bool isme, Map const &map);
		bool operator== (Team const &other) const;
		Pokemon& at_replacement ();
		Pokemon const & at_replacement () const;
		void output (std::string &output);
	private:
		void load (std::string const &name, Map const &map);
};

}
#endif
