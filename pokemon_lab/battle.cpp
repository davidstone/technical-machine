// Pokemon Lab battle
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <string>
#include "battle.h"
#include "../analyze_logs.h"
#include "../expectiminimax.h"
#include "../pokemon.h"
#include "../species.h"
#include "../team.h"
#include "../teampredictor.h"
#include "../weather.h"
#include "connect.h"
#include "outmessage.h"

namespace technicalmachine {
namespace pl {

Battle::Battle (Map const & map, std::string const & opponent, int depth_):
	ai (true, map, 6),
	foe (false, map, ai.size),
	log (ai, foe),
	depth (depth_) {
	for (std::vector <Pokemon>::const_iterator pokemon = ai.pokemon.set.begin(); pokemon != ai.pokemon.set.end(); ++pokemon)
		slot_memory.push_back (pokemon->name);
	foe.player = opponent;
	ai.replacing = true;
	foe.replacing = true;
}

void Battle::handle_request_action (BotClient & botclient, uint32_t field_id, uint8_t slot, uint8_t index, bool replace, std::vector <uint8_t> const & switches, bool can_switch, bool forced, std::vector <uint8_t> const & moves) {
	do_turn (*log.first, *log.last, weather);
	incorrect_hp (*log.first);
	incorrect_hp (*log.last);
	if (rand () % 50 == 0) {
		OutMessage comment (OutMessage::CHANNEL_MESSAGE);
		comment.write_int (field_id);
		comment.write_string (botclient.get_response ());
		comment.send (botclient.socket);
	}
	OutMessage msg (OutMessage::BATTLE_ACTION);
	if (forced)
		msg.write_move (field_id, 1);
	else {
		Team predicted = foe;
		std::cout << "======================\nPredicting...\n";
		predict_team (botclient.detailed, predicted, ai.size);
		std::string out;
		predicted.output (out);
		std::cout << out;

		int64_t min_score;
		Move::moves_list move = expectiminimax (ai, predicted, weather, depth, botclient.score, min_score);
		if (Move::is_switch (move))
			msg.write_switch (field_id, switch_slot (move));
		else {
			uint8_t move_index = 0;
			while (ai.pokemon->move.set [move_index].name != move)
				++move_index;
			msg.write_move (field_id, move_index, 1 - party);
		}
	}
	msg.send (botclient.socket);
	if (!ai.replacing)
		log.initialize_turn (ai, foe);
}

void Battle::handle_print (uint8_t category, uint16_t message_id, std::vector <std::string> const & arguments) {
	std::cout << "party id: " << static_cast <int> (party) << '\n';
	std::cout << "category: " << static_cast <int> (category) << '\n';
	std::cout << "message_id: " << message_id << '\n';
	for (std::vector <std::string>::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
		std::cout << "\t" + *it + '\n';
	switch (category) {
		case InMessage::BATTLE_MESSAGES:
			switch (message_id) {
				case 2:
				case 6:
					log.active->miss = true;
					break;
				case 9:
					log.active->ch = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_STAT_LEVEL:
			break;
		case InMessage::STATUS_EFFECTS_BURN:
			if (message_id == 1)
				log.active->at_replacement().move->variable.index = 1;
			break;
		case InMessage::STATUS_EFFECTS_CONFUSION:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
				case 2:	// Confusion ends
					break;
				case 4:
					log.active->hitself = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_FREEZE:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
				case 2:	// Defrost
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_PARALYSIS:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					std::cout << "Me: " << log.active->me << '\n';
					break;
				case 2:
					log.active->fully_paralyzed = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_POISON:
			switch (message_id) {
				case 1:
				case 2:
					log.active->at_replacement().move->variable.index = 1;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_SLEEP:
			case 1:
				log.active->at_replacement().move->variable.index = 1;
				break;
			case 2:		// Woke up
				break;
			break;
		case InMessage::STATUS_EFFECTS_FLINCH:
			switch (message_id)
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
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
			}
			break;
		case InMessage::ABILITY_MESSAGES:
			switch (message_id) {
				case 0:
					log.inactive->at_replacement().ability.name = Ability::AFTERMATH;
					break;
				case 1:
					log.inactive->at_replacement().ability.name = Ability::ANGER_POINT;
					break;
				case 2:
					log.active->at_replacement().ability.name = Ability::ANTICIPATION;	// fix
					break;
				case 3:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::BAD_DREAMS;
					break;
				case 4:
					log.inactive->at_replacement().ability.name = Ability::CLEAR_BODY;
//					log.active->at_replacement().ability.name = Ability::HYPER_CUTTER;
					break;
				case 5:
					log.inactive->at_replacement().ability.name = Ability::CUTE_CHARM;
					break;
				case 6:
					log.inactive->at_replacement().ability.name = Ability::DAMP;
					break;
				case 7:
					log.active->at_replacement().ability.name = Ability::DOWNLOAD;
					break;
				case 8:
					log.active->at_replacement().ability.name = Ability::DRIZZLE;
					break;
				case 9:
					log.active->at_replacement().ability.name = Ability::DROUGHT;
					break;
				case 10:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 11:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::DRY_SKIN;
//					log.active->at_replacement().ability.name = Ability::SOLAR_POWER;
					break;
				case 12:
					log.inactive->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 14:
					log.inactive->at_replacement().ability.name = Ability::FLAME_BODY;
					break;
				case 15:
					log.inactive->at_replacement().ability.name = Ability::FLASH_FIRE;
					break;
				case 16:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::FLOWER_GIFT;
					break;
				case 17:		// $1 Forewarn alerted it to $2!
					log.active->at_replacement().ability.name = Ability::FOREWARN;
					break;
				case 18:		// $1 found $2's $3!
					log.active->at_replacement().ability.name = Ability::FRISK;
					break;
				case 19:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::HYDRATION;
					break;
				case 20:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::ICE_BODY;
					break;
				case 21:
					log.inactive->at_replacement().ability.name = Ability::IMMUNITY;
					break;
				case 22:
					log.inactive->at_replacement().ability.name = Ability::INSOMNIA;
					break;
				case 23:
					log.active->at_replacement().ability.name = Ability::INTIMIDATE;
					break;
				case 24:
					log.inactive->at_replacement().ability.name = Ability::LEAF_GUARD;
					break;
				case 25:
					log.inactive->at_replacement().ability.name = Ability::LEVITATE;
					break;
				case 26:		// $1's $2 drew the attack!
					log.inactive->at_replacement().ability.name = Ability::LIGHTNINGROD;
//					log.active->at_replacement().ability.name = Ability::STORM_DRAIN;
					break;
				case 27:
					log.inactive->at_replacement().ability.name = Ability::LIMBER;
					break;
				case 28:
					log.active->at_replacement().ability.name = Ability::MOLD_BREAKER;
					break;
				case 29:
					log.inactive->at_replacement().ability.name = Ability::MOTOR_DRIVE;
					break;
				case 30:
					log.inactive->at_replacement().ability.name = Ability::OBLIVIOUS;
					break;
				case 31:
					log.inactive->at_replacement().ability.name = Ability::OWN_TEMPO;
					break;
				case 32:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::POISON_HEAL;
					break;
				case 34:
					log.active->at_replacement().ability.name = Ability::PRESSURE;
					break;
				case 35:
					log.active->at_replacement().ability.name = Ability::QUICK_FEET;
					break;
				case 36:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::RAIN_DISH;
					break;
				case 37:
					log.inactive->at_replacement().ability.name = Ability::ROUGH_SKIN;
					break;
				case 38:
					log.active->at_replacement().ability.name = Ability::SAND_STREAM;
					break;
				case 39:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::SHED_SKIN;
					break;
				case 40:
					log.active->at_replacement().ability.name = Ability::SLOW_START;
					break;
				case 41:
					log.active->at_replacement().ability.name = Ability::SNOW_WARNING;
					break;
				case 42:
					log.inactive->at_replacement().ability.name = Ability::SOUNDPROOF;
					break;
				case 43:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::SPEED_BOOST;
					break;
				case 45:
					log.inactive->at_replacement().ability.name = Ability::STEADFAST;
					break;
				case 46:
					log.inactive->at_replacement().ability.name = Ability::STICKY_HOLD;
					break;
				case 47:
					log.inactive->at_replacement().ability.name = Ability::STURDY;
					break;
				case 48:
					log.active->at_replacement().ability.name = Ability::SYNCHRONIZE;		// Fix
					break;
				case 49:
					log.inactive->at_replacement().ability.name = Ability::TANGLED_FEET;
					break;
				case 50:		// $1 traced $2's $3!
					log.active->at_replacement().ability.name = Ability::TRACE;
//					log.inactive->at_replacement().ability.name = Ability::;
					break;
				case 51:
					log.active->at_replacement().ability.name = Ability::TRUANT;
					break;
				case 52:
					log.active->at_replacement().ability.name = Ability::UNBURDEN;
					break;
				case 53:
					log.inactive->at_replacement().ability.name = Ability::WATER_VEIL;
					break;
				case 54:
					log.inactive->at_replacement().ability.name = Ability::WONDER_GUARD;
					break;
				case 55:
					log.inactive->at_replacement().ability.name = Ability::LIQUID_OOZE;
					break;
				case 56:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::ICE_BODY;
					break;
				case 57:
					update_active_print (log, arguments);
					log.active->at_replacement().ability.name = Ability::DRY_SKIN;
					break;
				case 59:		// $1's $2 raised its $3!
//					log.active->at_replacement().ability.name = Ability::
					break;
				case 60:		// $1's $2 paralyzed $3!
//					log.active->at_replacement().ability.name = Ability::
					break;
				case 61:		// $1's $2 poisoned $3!
//					log.active->at_replacement().ability.name = Ability::
					break;
				case 62:		// $1's $2 made $3 fall asleep!
//					log.active->at_replacement().ability.name = Ability::
					break;
			}
			break;
		case InMessage::ITEM_MESSAGES:
			switch (message_id) {
				case 0:		// $1's $2 restored its health a little!
					update_active_print (log, arguments);
					log.active->at_replacement().item.name = Item::LEFTOVERS;
					break;
				case 1:		// $1's $2 cured its $3!
//					log.active->at_replacement().item.name = Item::
					break;
				case 2:
					log.active->at_replacement().item.name = Item::QUICK_CLAW;
					break;
				case 3:		// $1's $2 raised its $3!
//					log.active->at_replacement().item.name = Item::
					break;
				case 4:		// The berry was the wrong flavor for $1!
//					log.active->at_replacement().item.name = Item::
					break;
				case 5:		// #1's $2 weakened $3's power!
//					log.active->at_replacement().item.name = Item::
					break;
				case 6:		// $1 was hurt by $2's $3!
//					log.active->at_replacement().item.name = Item::
					break;
				case 7:		// Fix this
					log.active->at_replacement().item.name = Item::STICKY_BARB;
					break;
				case 8:
					update_active_print (log, arguments);
					log.active->at_replacement().item.name = Item::BLACK_SLUDGE;
					break;
				case 9:		// The $1 latched on to $2!
//					log.active->at_replacement().item.name = Item::
					break;
				case 10:
					log.inactive->at_replacement().item.name = Item::FOCUS_SASH;
					break;
				case 11:
					log.active->at_replacement().item.name = Item::CUSTAP_BERRY;
					break;
				case 12:
					log.active->at_replacement().item.name = Item::POWER_HERB;
					break;
			}
			break;
		default:
			break;
	}
}

void Battle::update_active_print (Log & log, std::vector <std::string> const & arguments) {
	// This needs to be changed to the correct message parser.
	assert (arguments.size() > 0);
	if (arguments [0] [3] - '0' == party) {
		log.active = & ai;
		log.inactive = & foe;
	}
	else {
		log.active = & foe;
		log.inactive = & ai;
	}
}

void Battle::handle_use_move (uint8_t party_, uint8_t slot, std::string const & nickname, uint16_t move_id) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = & ai;
		other = & foe;
	}
	else {
		team = & foe;
		other = & ai;
	}
	log.active = team;
	log.inactive = other;
	if (log.first == NULL) {
		log.first = team;
		log.last = other;
	}
	int move = move_id;
	if (move >= Move::SWITCH0)
		move += 6;
	log.log_move (static_cast <Move::moves_list> (move));
}

void Battle::handle_withdraw (uint8_t party, uint8_t slot, std::string const & nickname) {
}

void Battle::handle_send_out (Map const & map, uint8_t party_, uint8_t slot, uint8_t index, std::string const & nickname, uint16_t species_id, int8_t gender_, uint8_t level) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = & ai;
		other = & foe;
	}
	else {
		team = & foe;
		other = & ai;
	}
	species name = InMessage::pl_to_tm_species (species_id);
	Gender gender;
	gender.from_simulator_int (gender_);
	log.pokemon_sent_out (map, name, nickname, level, gender, *team, *other);
}

void Battle::handle_health_change (uint8_t party_id, uint8_t slot, int16_t change_in_health, int16_t remaining_health, uint16_t denominator) {
	if (log.move_damage) {
		unsigned effectiveness = get_effectiveness (log.active->at_replacement().move->type, log.inactive->at_replacement ());
		if ((effectiveness > 0) and (GROUND != log.active->at_replacement().move->type or grounded (*log.inactive, weather))) {
			log.inactive->damage = log.inactive->at_replacement().hp.max * change_in_health / denominator;
			if (static_cast <unsigned> (log.inactive->damage) > log.inactive->at_replacement().hp.stat)
				log.inactive->damage = log.inactive->at_replacement().hp.stat;
		}
		log.move_damage = false;
	}
	
	if (remaining_health < 0)
		remaining_health = 0;
	// If the message is about me, active must be me, otherwise, active must not be me
	if ((party_id == party) == log.active->me) {
		log.active->at_replacement().new_hp = remaining_health;
	}
	else {
		log.inactive->at_replacement().new_hp = remaining_health;
	}
}

void Battle::handle_set_pp (uint8_t party_, uint8_t slot, uint8_t pp) {
}

void Battle::handle_fainted (uint8_t party_, uint8_t slot, std::string const & nickname) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = & ai;
		other = & foe;
	}
	else {
		team = & foe;
		other = & ai;
	}
	team->at_replacement().fainted = true;
}

void Battle::handle_begin_turn (uint16_t turn_count) {
	std::cout << "Begin turn " << turn_count << '\n';
}

void Battle::handle_set_move (uint8_t pokemon, uint8_t move_slot, uint16_t new_move, uint8_t pp, uint8_t max_pp) {
}

uint8_t Battle::switch_slot (Move::moves_list move) {
	uint8_t slot = move - Move::SWITCH0;
	for (uint8_t n = 0; n != slot_memory.size(); ++n) {
		if (slot_memory [n] == ai.pokemon.set [slot].name)
			return n;
	}
	assert (false);
	return 0;		// This should never happen
}

void Battle::incorrect_hp (Team & team) {
	unsigned max_hp = 48;
	for (std::vector<Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		if (team.me)
			max_hp = pokemon->hp.max;
		unsigned pixels = max_hp * pokemon->hp.stat / pokemon->hp.max;
		if (pixels != pokemon->new_hp and (pokemon->new_hp - 1U > pixels or pixels > pokemon->new_hp + 1U)) {
			std::cerr << "Uh oh! " + pokemon->get_name () + " has the wrong HP! Pokemon Lab reports approximately " << pokemon->new_hp * pokemon->hp.max / max_hp << " but TM thinks it has " << pokemon->hp.stat << "\n";
			pokemon->hp.stat = pokemon->new_hp * pokemon->hp.max / max_hp;
		}
	}
}

}
}
