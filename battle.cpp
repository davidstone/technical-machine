// Generic battle
// Copyright (C) 2011 David Stone
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

#include "battle.h"
#include <cstdint>
#include <iostream>
#include <string>

#include "analyze_logs.h"
#include "expectiminimax.h"
#include "gender.h"
#include "move.h"
#include "species.h"
#include "team.h"
#include "teampredictor.h"
#include "weather.h"

#include "network/connect.h"

namespace technicalmachine {

GenericBattle::GenericBattle (std::string const & opponent, int battle_depth):
	ai (true, 6),
	foe (false, ai.size),
	log (ai, foe),
	depth (battle_depth) {
	foe.player = opponent;
	ai.replacing = true;
	foe.replacing = true;
}

void GenericBattle::handle_begin_turn (uint16_t turn_count) {
	std::cout << "Begin turn " << turn_count << '\n';
}

void GenericBattle::update_from_previous_turn (network::GenericClient & client, uint32_t battle_id) {
	do_turn (*log.first, *log.last, weather);
	correct_hp_and_report_errors (*log.first);
	correct_hp_and_report_errors (*log.last);
	if (rand () % client.chattiness == 0)
		client.send_channel_message (battle_id, client.get_response ());
}

Move::Moves GenericBattle::determine_action (network::GenericClient & client) {
	Team predicted = foe;
	std::cout << "======================\nPredicting...\n";
	predict_team (client.detailed, predicted, ai.size);
	std::string out;
	predicted.output (out);
	std::cout << out;

	int64_t min_score;
	return expectiminimax (ai, predicted, weather, depth, client.score, min_score);
}

void GenericBattle::handle_use_move (uint8_t moving_party, uint8_t slot, std::string const & nickname, int16_t move_id) {
	if (party == moving_party) {
		log.active = &ai;
		log.inactive = &foe;
	}
	else {
		log.active = &foe;
		log.inactive = &ai;
	}
	if (log.first == nullptr) {
		log.first = log.active;
		log.last = log.inactive;
	}
	int move = move_id;
	if (move >= Move::SWITCH0)
		move += 6;
	log.log_move (static_cast <Move::Moves> (move));
}

void GenericBattle::handle_withdraw (uint8_t party, uint8_t slot, std::string const & nickname) {
}

void GenericBattle::handle_send_out (uint8_t switching_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level) {
	Team * team;
	Team * other;
	if (party == switching_party) {
		team = & ai;
		other = & foe;
	}
	else {
		team = & foe;
		other = & ai;
	}
	log.pokemon_sent_out (species, nickname, level, gender, *team, *other);
}

void GenericBattle::handle_health_change (uint8_t party_changing_health, uint8_t slot, int16_t change_in_health, int16_t remaining_health, int16_t denominator) {
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
	if ((party_changing_health == party) == log.active->me)
		log.active->at_replacement().new_hp = remaining_health;
	else
		log.inactive->at_replacement().new_hp = remaining_health;
}

void GenericBattle::correct_hp_and_report_errors (Team & team) {
	int max_hp = 48;
	for (std::vector<Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		if (team.me)
			max_hp = pokemon->hp.max;
		int pixels = max_hp * pokemon->hp.stat / pokemon->hp.max;
		if (pixels != pokemon->new_hp and (pokemon->new_hp - 1 > pixels or pixels > pokemon->new_hp + 1)) {
			std::cerr << "Uh oh! " + pokemon->get_name () + " has the wrong HP! Pokemon Lab reports approximately " << pokemon->new_hp * pokemon->hp.max / max_hp << " but TM thinks it has " << pokemon->hp.stat << "\n";
			pokemon->hp.stat = pokemon->new_hp * pokemon->hp.max / max_hp;
		}
	}
}

void GenericBattle::handle_set_pp (uint8_t party_changing_pp, uint8_t slot, uint8_t pp) {
}

void GenericBattle::handle_fainted (uint8_t fainting_party, uint8_t slot, std::string const & nickname) {
	Team * fainter;
	if (party == fainting_party)
		fainter = & ai;
	else
		fainter = & foe;
	fainter->at_replacement().fainted = true;
}

} // namespace technicalmachine
