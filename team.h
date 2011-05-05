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
#include "pokemon.h"

namespace technicalmachine {

class Team {
	public:
		std::string player;						// The player's name
		Active<Pokemon> active;

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
		bool charge;
		bool curse;
		bool damaged;
		bool defense_curl;
		bool destiny_bond;
		bool ff;				// Flash Fire
		bool flinch;
		bool focus_energy;
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
		bool moved;
		bool mud_sport;
		bool nightmare;
		bool pass;
		bool power_trick;
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
		
		const bool me;				// Is this my team?
	
		bool operator== (const Team &other) const {
			if (this->active.set.size() != other.active.set.size())
				return false;
			for (size_t n = 0; n != this->active.set.size(); ++n) {
				if (this->active.set [n] != other.active.set [n])
					return false;
			}
			return this->active->name == other.active->name and this->vanish == other.vanish and this->bide == other.bide and this->confused == other.confused and this->embargo == other.embargo and this->encore == other.encore and this->heal_block == other.heal_block and this->magnet_rise == other.magnet_rise and this->partial_trap == other.partial_trap and this->perish_song == other.perish_song and this->rampage == other.rampage and this->slow_start == other.slow_start and this->stockpile == other.stockpile and this->taunt == other.taunt and this->toxic == other.toxic and this->uproar == other.yawn and this->aqua_ring == other.aqua_ring and this->attract == other.attract and this->charge == other.charge and this->curse == other.curse and this->defense_curl == other.defense_curl and this->destiny_bond == other.destiny_bond and this->ff == other.ff and this->focus_energy == other.focus_energy and this->identified == other.identified and this->imprison == other.imprison and this->ingrain == other.ingrain and this->leech_seed == other.leech_seed and this->loaf == other.loaf and this->lock_on == other.lock_on and this->minimize == other.minimize and this->mud_sport == other.mud_sport and this->nightmare == other.nightmare and this->torment == other.torment and this->trapped == other.trapped and this->water_sport == other.water_sport and this->accuracy == other.accuracy and this->evasion == other.evasion and this->counter == other.counter and this->light_screen == other.light_screen and this->lucky_chant == other.lucky_chant and this->mist == other.mist and this->reflect == other.reflect and this->safeguard == other.safeguard and this->tailwind == other.tailwind and this->wish == other.wish and this->spikes == other.spikes and this->toxic_spikes == other.toxic_spikes and this->stealth_rock == other.stealth_rock and this->me == other.me;
		}
		Team (bool isme);
};

std::string search (std::ifstream &file, std::string &output2, const std::string &data);
void loadteam (Team &team, const std::string &name, const Map &map, int detailed [][7]);
void loadpokemon (Team &team, Pokemon &member);
void pokelabteam (Team &team, const std::string &name, const Map &map);
void pokelabpokemon (Team& team, std::ifstream &file, const Map &map);
void poteam (Team &team, const std::string &name);
void popokemon (Team &team, std::ifstream &file, const species pokemon_converter [], const abilities ability_converter [], const items item_converter [], const natures nature_converter [], const moves_list move_converter []);
unsigned poconverter (const std::string &data, const std::string &end, const std::string &line);

}
#endif
