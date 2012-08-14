// Pokemon Lab battle
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

#include "battle.hpp"

#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

namespace technicalmachine {
namespace pl {

void Battle::handle_print (uint8_t category, uint16_t message_id, std::vector <std::string> const & arguments) {
#if 0
// This needs a re-write
//	std::cout << "party id: " << static_cast <int> (party) << '\n';
//	std::cout << "category: " << static_cast <int> (category) << '\n';
//	std::cout << "message_id: " << message_id << '\n';
	for (std::string const & argument : arguments)
		std::cout << "\t" + argument + '\n';
	switch (category) {
		case InMessage::BATTLE_MESSAGES:
			switch (message_id) {
				case 2:
				case 6:
					active->miss = true;
					break;
				case 9:
					active->ch = true;
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_STAT_LEVEL:
			break;
		case InMessage::STATUS_EFFECTS_BURN:
			if (message_id == 1)
				active->at_replacement().move().variable.index = 1;
			break;
		case InMessage::STATUS_EFFECTS_CONFUSION:
			switch (message_id) {
				case 1:
					active->at_replacement().move().variable.index = 1;
					break;
				case 2:	// Confusion ends
					break;
				case 4:
					active->hitself = true;
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_FREEZE:
			switch (message_id) {
				case 1:
					active->at_replacement().move().variable.index = 1;
					break;
				case 2:	// Defrost
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_PARALYSIS:
			switch (message_id) {
				case 1:
					active->at_replacement().move().variable.index = 1;
					std::cout << "Me: " << active->me << '\n';
					break;
				case 2:
					active->fully_paralyzed = true;
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_POISON:
			switch (message_id) {
				case 1:
				case 2:
					active->at_replacement().move().variable.index = 1;
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_SLEEP:
			switch (message_id) {
				case 1:
					active->at_replacement().move().variable.index = 1;
					break;
				case 2:		// Woke up
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_FLINCH:
			switch (message_id) {
				case 1:
					active->at_replacement().move().variable.index = 1;
					break;
				default:
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_ATTRACT:
			break;
		// Weather messages need to be parsed in case of items that extend their effects
		case InMessage::WEATHER_HAIL:
			break;
		case InMessage::WEATHER_RAIN:
			break;
		case InMessage::WEATHER_SANDSTORM:
			break;
		case InMessage::WEATHER_SUN:
			break;
		case InMessage::BATTLE_MESSAGES_UNIQUE:
			switch (message_id) {
				case 0:		// Multi-hit moves
					break;
				case 1:		// $1 was cured of $2!
					break;
				case 2:		// But nothing happened!
					break;
				case 7:		// Parse this message to get base attack of each Pokemon (Beat Up)
					break;
				case 9:		// $1 became the $2 type!
					break;
				case 10:		// $1 stole $2's $3!
					break;
				case 11:		// Could not steal the $1 from $2!
					break;
				case 12:		// $1 knocked off the foe $2's $3!
					break;
				case 13:		// $1 obtained $2! (trick)
					break;
				case 14:		// Magnitude $1!
					break;
				case 18:		// $1 was released!
					break;		// Determine whether Grip Claw was used
				case 22:		// $1 is tightening its focus! Can be useful with BP and U-turn
					break;
				case 26:		// $1's taunt wore off
					break;
				case 31:		// $1 consumed $2's $3 (Bug Bite)
				case 147:	// $1 used $2's $3 (Pluck)
					break;
				case 37:		// $1 learned $2!
					break;
				case 58:		// $1 was hit by recoil!
					break;
				case 70:		// $1's $2 was disabled!
					break;
				case 80:		// $1 wore off! (Reflect / Light Screen)
					break;
				case 97:		// $1 became confused due to fatigue (Thrash clones)	
					break;
				case 136:	// $1's substitute faded
					break;
				case 146:	// $1 got a $2! (Recycle)
					break;
				case 153:	// $1 calmed down (Uproar)
					break;
				case 156:	// $1's encore ended.
					break;
				case 157:	// $1 flung its $2!
					break;
				default:
					break;
			}
			break;
		case InMessage::ABILITY_MESSAGES:
			switch (message_id) {
				case 0:
					inactive->at_replacement().ability.name = Ability::AFTERMATH;
					break;
				case 1:
					inactive->at_replacement().ability.name = Ability::ANGER_POINT;
					break;
				case 2:
					active->at_replacement().ability.name = Ability::ANTICIPATION;	// fix
					break;
				case 3:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::BAD_DREAMS;
					break;
				case 4:
					inactive->at_replacement().ability.name = Ability::CLEAR_BODY;
//					active->at_replacement().ability.name = Ability::HYPER_CUTTER;
//					active->at_replacement().ability.name = Ability::WHITE_SMOKE;
					break;
				case 5:
					inactive->at_replacement().ability.name = Ability::CUTE_CHARM;
					break;
				case 6:
					inactive->at_replacement().ability.name = Ability::DAMP;
					break;
				case 7:
					active->at_replacement().ability.name = Ability::DOWNLOAD;
					break;
				case 8:
					active->at_replacement().ability.name = Ability::DRIZZLE;
					break;
				case 9:
					active->at_replacement().ability.name = Ability::DROUGHT;
					break;
				case 10:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 11:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::DRY_SKIN;
//					active->at_replacement().ability.name = Ability::SOLAR_POWER;
					break;
				case 12:
					inactive->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 14:
					inactive->at_replacement().ability.name = Ability::FLAME_BODY;
					break;
				case 15:
					inactive->at_replacement().ability.name = Ability::FLASH_FIRE;
					break;
				case 16:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::FLOWER_GIFT;
					break;
				case 17:		// $1 Forewarn alerted it to $2!
					active->at_replacement().ability.name = Ability::FOREWARN;
					break;
				case 18:		// $1 found $2's $3!
					active->at_replacement().ability.name = Ability::FRISK;
					break;
				case 19:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::HYDRATION;
					break;
				case 20:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::ICE_BODY;
					break;
				case 21:
					inactive->at_replacement().ability.name = Ability::IMMUNITY;
					break;
				case 22:
					inactive->at_replacement().ability.name = Ability::INSOMNIA;
//					inactive->at_replacement().ability.name = Ability::VITAL_SPIRIT;
					break;
				case 23:
					active->at_replacement().ability.name = Ability::INTIMIDATE;
					break;
				case 24:
					inactive->at_replacement().ability.name = Ability::LEAF_GUARD;
					break;
				case 25:
					inactive->at_replacement().ability.name = Ability::LEVITATE;
					break;
				case 26:		// $1's $2 drew the attack!
					inactive->at_replacement().ability.name = Ability::LIGHTNINGROD;
//					active->at_replacement().ability.name = Ability::STORM_DRAIN;
					break;
				case 27:
					inactive->at_replacement().ability.name = Ability::LIMBER;
					break;
				case 28:
					active->at_replacement().ability.name = Ability::MOLD_BREAKER;
					break;
				case 29:
					inactive->at_replacement().ability.name = Ability::MOTOR_DRIVE;
					break;
				case 30:
					inactive->at_replacement().ability.name = Ability::OBLIVIOUS;
					break;
				case 31:
					inactive->at_replacement().ability.name = Ability::OWN_TEMPO;
					break;
				case 32:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::POISON_HEAL;
					break;
				case 34:
					active->at_replacement().ability.name = Ability::PRESSURE;
					break;
				case 35:
					active->at_replacement().ability.name = Ability::QUICK_FEET;
					break;
				case 36:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::RAIN_DISH;
					break;
				case 37:
					inactive->at_replacement().ability.name = Ability::ROUGH_SKIN;
					break;
				case 38:
					active->at_replacement().ability.name = Ability::SAND_STREAM;
					break;
				case 39:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::SHED_SKIN;
					break;
				case 40:
					active->at_replacement().ability.name = Ability::SLOW_START;
					break;
				case 41:
					active->at_replacement().ability.name = Ability::SNOW_WARNING;
					break;
				case 42:
					inactive->at_replacement().ability.name = Ability::SOUNDPROOF;
					break;
				case 43:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::SPEED_BOOST;
					break;
				case 45:
					inactive->at_replacement().ability.name = Ability::STEADFAST;
					break;
				case 46:
					inactive->at_replacement().ability.name = Ability::STICKY_HOLD;
					break;
				case 47:
					inactive->at_replacement().ability.name = Ability::STURDY;
					break;
				case 48:
					active->at_replacement().ability.name = Ability::SYNCHRONIZE;		// Fix
					break;
				case 49:
					inactive->at_replacement().ability.name = Ability::TANGLED_FEET;
					break;
				case 50:		// $1 traced $2's $3!
					active->at_replacement().ability.name = Ability::TRACE;
//					inactive->at_replacement().ability.name = Ability::;
					break;
				case 51:
					active->at_replacement().ability.name = Ability::TRUANT;
					break;
				case 52:
					active->at_replacement().ability.name = Ability::UNBURDEN;
					break;
				case 53:
					inactive->at_replacement().ability.name = Ability::WATER_VEIL;
					break;
				case 54:
					inactive->at_replacement().ability.name = Ability::WONDER_GUARD;
					break;
				case 55:
					inactive->at_replacement().ability.name = Ability::LIQUID_OOZE;
					break;
				case 56:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::ICE_BODY;
					break;
				case 57:
					update_active_print (arguments);
					active->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 59:		// $1's $2 raised its $3!
//					active->at_replacement().ability.name = Ability::
					break;
				case 60:		// $1's $2 paralyzed $3!
//					active->at_replacement().ability.name = Ability::
					break;
				case 61:		// $1's $2 poisoned $3!
//					active->at_replacement().ability.name = Ability::
					break;
				case 62:		// $1's $2 made $3 fall asleep!
//					active->at_replacement().ability.name = Ability::
					break;
				default:
					break;
			}
			break;
		case InMessage::ITEM_MESSAGES:
			switch (message_id) {
				case 0:		// $1's $2 restored its health a little!
					update_active_print (arguments);
					active->at_replacement().item.name = Item::LEFTOVERS;
					break;
				case 1:		// $1's $2 cured its $3!
//					active->at_replacement().item.name = Item::
					break;
				case 2:
					active->at_replacement().item.name = Item::QUICK_CLAW;
					break;
				case 3:		// $1's $2 raised its $3!
//					active->at_replacement().item.name = Item::
					break;
				case 4:		// The berry was the wrong flavor for $1!
//					active->at_replacement().item.name = Item::
					break;
				case 5:		// #1's $2 weakened $3's power!
//					active->at_replacement().item.name = Item::
					break;
				case 6:		// $1 was hurt by $2's $3!
//					active->at_replacement().item.name = Item::
					break;
				case 7:		// Fix this
					active->at_replacement().item.name = Item::STICKY_BARB;
					break;
				case 8:
					update_active_print (arguments);
					active->at_replacement().item.name = Item::BLACK_SLUDGE;
					break;
				case 9:		// The $1 latched on to $2!
//					active->at_replacement().item.name = Item::
					break;
				case 10:
					inactive->at_replacement().item.name = Item::FOCUS_SASH;
					break;
				case 11:
					active->at_replacement().item.name = Item::CUSTAP_BERRY;
					break;
				case 12:
					active->at_replacement().item.name = Item::POWER_HERB;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
#endif
}

void Battle::update_active_print (std::vector <std::string> const & arguments) {
#if 0
	// This needs to be changed to the correct message parser.
	assert (!arguments.empty ());
	if (boost::lexical_cast <int> (arguments [0] [3]) == party) {
		active = & ai;
		inactive = & foe;
	}
	else {
		active = & foe;
		inactive = & ai;
	}
#endif
}

void Battle::handle_set_move (uint8_t pokemon, uint8_t move_slot, int16_t new_move, uint8_t pp, uint8_t max_pp) {
}

uint8_t Battle::get_target () const {
	return 1 - my_party();
}

}	// namespace pl
}	// namespace technicalmachine
