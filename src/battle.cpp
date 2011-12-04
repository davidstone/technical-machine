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
#include <vector>

#include "endofturn.h"
#include "expectiminimax.h"
#include "gender.h"
#include "move.h"
#include "species.h"
#include "switch.h"
#include "team.h"
#include "teampredictor.h"
#include "weather.h"

#include "network/connect.h"
#include <iostream>

namespace technicalmachine {

GenericBattle::GenericBattle (std::string const & opponent, int battle_depth):
	ai (true, 6),
	foe (false, ai.size),
	depth (battle_depth),
	active (nullptr),
	inactive (nullptr),
	party (-1)
	{
	foe.player = opponent;
	ai.replacing = true;
	foe.replacing = true;
	for (Pokemon const & pokemon : ai.pokemon.set)
		slot_memory.push_back (pokemon.name);
	initialize_turn ();
}

GenericBattle::GenericBattle (std::string const & opponent, int battle_depth, Team const & team):
	ai (team),
	foe (false, ai.size),
	depth (battle_depth),
	party (-1)
	{
	foe.player = opponent;
	ai.replacing = true;
	foe.replacing = true;
	for (Pokemon const & pokemon : ai.pokemon.set)
		slot_memory.push_back (pokemon.name);
	initialize_turn ();
}

void GenericBattle::handle_begin_turn (uint16_t turn_count) const {
	std::cout << "Begin turn " << turn_count << '\n';
}

void GenericBattle::update_from_previous_turn (network::GenericClient & client, uint32_t battle_id) {
	do_turn ();
	correct_hp_and_report_errors (*first);
	correct_hp_and_report_errors (*last);
	if (rand () % client.chattiness == 0)
		client.send_channel_message (battle_id, client.get_response ());
}

Move::Moves GenericBattle::determine_action (network::GenericClient & client) {
	Team predicted = foe;
	std::cout << "======================\n";
	std::cout << "Predicting...\n";
	predict_team (client.detailed, predicted, ai.size);
	std::string out;
	predicted.output (out);
	std::cout << out;

	int64_t min_score;
	return expectiminimax (ai, predicted, weather, depth, client.score, min_score);
}

void GenericBattle::handle_use_move (uint8_t moving_party, uint8_t slot, Move::Moves move_name) {
	bool const is_me = (party == moving_party);
	active = is_me ? & ai : & foe;
	inactive = is_me ? & foe : & ai;

	if (first == nullptr) {
		first = active;
		last = inactive;
	}

	active->moved = true;
	if (active->at_replacement().find_move (move_name)) {
		Move move (move_name, 3, inactive->size);
		active->at_replacement().move.add (move);
	}
	active->at_replacement().move->variable.index = 0;
	if (active->at_replacement().move->basepower != 0)
		move_damage = true;
}

void GenericBattle::handle_withdraw (uint8_t switching_party, uint8_t slot, std::string const & nickname) const {
}

void GenericBattle::handle_send_out (uint8_t switching_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level) {
	std::cerr << "switching_party: " << static_cast <int> (switching_party) << '\n';
	std::cerr << "nickname: " + nickname + "\n";
	bool const is_me = (switching_party == party);
	active = is_me ? &ai : &foe;
	inactive = is_me ? &foe : &ai;

	if (first == nullptr) {
		first = active;
		last = inactive;
	}

	size_t const replacement = active->replacement;		// This is needed to make sure I don't overwrite important information in a situation in which a team switches multiple times in one turn (due to replacing fainted Pokemon).
	
	// If it hasn't been seen already, add it to the team.
	if (!active->seen_pokemon (species))
		active->add_pokemon (species, nickname, level, gender);
	
	// Special analysis when a Pokemon is brought out due to a phazing move
	if (inactive->at_replacement().move->is_phaze ()) {
		inactive->at_replacement().move->variable.index = 0;
		while (active->pokemon.set [inactive->at_replacement().move->variable.index].name != species)
			++inactive->at_replacement().move->variable.index;
	}
	else if (!active->moved) {
		active->pokemon.set [replacement].move.index = 0;
		while (active->pokemon.set [replacement].move->name != Move::SWITCH0)
			++active->pokemon.set [replacement].move.index;
		active->pokemon.set [replacement].move.index += active->replacement;		
	}
}

void GenericBattle::handle_health_change (uint8_t party_changing_health, uint8_t slot, int16_t change_in_health, int16_t remaining_health, int16_t denominator) {
	if (move_damage) {
		unsigned effectiveness = get_effectiveness (active->at_replacement().move->type, inactive->at_replacement ());
		if ((effectiveness > 0) and (GROUND != active->at_replacement().move->type or grounded (*inactive, weather))) {
			inactive->damage = inactive->at_replacement().hp.max * change_in_health / denominator;
			if (static_cast <unsigned> (inactive->damage) > inactive->at_replacement().hp.stat)
				inactive->damage = inactive->at_replacement().hp.stat;
		}
		move_damage = false;
	}
	
	if (remaining_health < 0)
		remaining_health = 0;
	// If the message is about me, active must be me, otherwise, active must not be me
	if ((party_changing_health == party) == active->me)
		active->at_replacement().new_hp = remaining_health;
	else
		inactive->at_replacement().new_hp = remaining_health;
}

void GenericBattle::correct_hp_and_report_errors (Team & team) {
	for (Pokemon & pokemon : team.pokemon.set) {
		int const max_hp = (team.me) ? pokemon.hp.max : get_max_damage_precision ();
		int const pixels = max_hp * pokemon.hp.stat / pokemon.hp.max;
		if (pixels != pokemon.new_hp and (pokemon.new_hp - 1 > pixels or pixels > pokemon.new_hp + 1)) {
			std::cerr << "Uh oh! " + pokemon.get_name () + " has the wrong HP! The server reports approximately " << pokemon.new_hp * pokemon.hp.max / max_hp << " but TM thinks it has " << pokemon.hp.stat << "\n";
			pokemon.hp.stat = pokemon.new_hp * pokemon.hp.max / max_hp;
			std::cerr << "max_hp: " << max_hp << '\n';
			std::cerr << "pokemon.hp.max: " << pokemon.hp.max << '\n';
			std::cerr << "pokemon.hp.stat: " << pokemon.hp.stat << '\n';
			std::cerr << "pokemon.new_hp: " << pokemon.new_hp << '\n';
			std::cerr << "pixels: " << pixels << '\n';
		}
	}
}

void GenericBattle::handle_set_pp (uint8_t party_changing_pp, uint8_t slot, uint8_t pp) {
}

void GenericBattle::handle_fainted (uint8_t fainting_party, uint8_t slot) {
	Team & fainter = (party == fainting_party) ? ai : foe;
	fainter.at_replacement().fainted = true;
}

uint8_t GenericBattle::switch_slot (Move::Moves move) const {
	uint8_t slot = move - Move::SWITCH0;
	uint8_t n = 0;
	for (; n != slot_memory.size(); ++n) {
		if (slot_memory [n] == ai.pokemon.set [slot].name)
			break;
	}
	return n;
}

unsigned GenericBattle::get_max_damage_precision () {
	return 48;
}

void GenericBattle::initialize_turn () {
	initialize_team (ai);
	initialize_team (foe);
	// Simulators might not send an HP change message if a move does 0 damage.
	move_damage = false;
	
	first = nullptr;
	last = nullptr;
}

void GenericBattle::initialize_team (Team & team) {
	for (Pokemon & pokemon : team.pokemon.set)
		pokemon.move.index = 0;
	team.ch = false;
	team.fully_paralyzed = false;
	team.hitself = false;
	team.miss = false;
	team.replacement = team.pokemon.index;
	team.replacing = false;
}

void GenericBattle::do_turn () {
	first->moved = false;
	last->moved = false;
	if (first->replacing) {
		normalize_hp ();
		switchpokemon (*first, *last, weather);
		first->moved = false;
		normalize_hp ();
		if (last->replacing) {
			switchpokemon (*last, *first, weather);
			last->moved = false;
			normalize_hp ();
			last->replacing = false;
		}
		first->replacing = false;
	}
	else {
		// Anything with recoil will mess this up
		usemove (*first, *last, weather, last->damage);
		last->pokemon->normalize_hp ();
		usemove (*last, *first, weather, first->damage);
		first->pokemon->normalize_hp ();

		endofturn (*first, *last, weather);
		normalize_hp ();

		while (foe.pokemon->fainted) {
			if (!foe.pokemon->move->is_switch()) {
				foe.pokemon->move.index = 0;
				while (foe.pokemon->move->name != Move::SWITCH0)
					++foe.pokemon->move.index;
				foe.pokemon->move.index += foe.replacement;
			}
			usemove (foe, ai, weather, first->damage);
		}
	}
	std::string out;
	first->output (out);
	std::cout << out;
	last->output (out);
	std::cout << out;
}

void GenericBattle::normalize_hp () {
	ai.pokemon->normalize_hp ();
	foe.pokemon->normalize_hp ();
}

} // namespace technicalmachine
