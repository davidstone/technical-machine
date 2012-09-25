// Generic battle
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

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "battle_result.hpp"

#include "network/client.hpp"
#include "network/outmessage.hpp"

#include "pokemon_lab/write_team_file.hpp"

#include "../endofturn.hpp"
#include "../expectiminimax.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../move/move.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon_not_found.hpp"

#include "../team_predictor/team_predictor.hpp"

namespace technicalmachine {
class DetailedStats;
enum class Moves : uint16_t;

GenericBattle::GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, std::string const & team_file_name):
	opponent_name (_opponent),
	random_engine (seed),
	ai (6, random_engine, team_file_name),
	depth (battle_depth)
	{
	ai.all_pokemon().for_each ([this] (Pokemon const & pokemon)->void {
		slot_memory.emplace_back(pokemon.name());
	});
	initialize_turn ();
}

GenericBattle::GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, Team const & team):
	opponent_name(_opponent),
	random_engine (seed),
	ai (team),
	depth (battle_depth)
	{
	ai.all_pokemon().for_each ([this] (Pokemon const & pokemon)->void {
		slot_memory.emplace_back(pokemon.name());
	});
	initialize_turn ();
}

bool GenericBattle::is_me (Party const other_party) const {
	return my_party == other_party;
}

void GenericBattle::set_party_if_unknown(Party const new_party) {
	my_party.set_if_unknown(new_party);
}

void GenericBattle::write_team (network::OutMessage & msg, std::string const & username) const {
	msg.write_team (ai, username);
}

Team GenericBattle::predict_foe_team (DetailedStats const & detailed) const {
	return predict_team (detailed, foe, ai.all_pokemon().size());
}

void GenericBattle::handle_begin_turn (uint16_t turn_count) const {
	std::cout << "Begin turn " << turn_count << '\n';
}

void GenericBattle::handle_request_action (network::GenericClient & client, network::OutMessage & msg, uint32_t battle_id, bool can_switch, std::vector<uint8_t> const & attacks_allowed, bool forced) {
	// At some point, I will create a fail-safe that actually checks that the
	// move TM tries to use is considered a valid move by the server.
	update_from_previous_turn (client, battle_id);
	if (!forced) {
		Moves const move = determine_action(client);
		if (Move::is_switch (move))
			msg.write_switch (battle_id, switch_slot (move));
		else {
			uint8_t move_index = 0;
			while (ai.pokemon().move(move_index).name != move)
				++move_index;
			msg.write_move (battle_id, move_index, get_target ());
		}
	}
	else {
		msg.write_move (battle_id, 1);
	}
	if (!ai.pokemon().replacing())
		initialize_turn ();
}

void GenericBattle::update_from_previous_turn (network::GenericClient & client, uint32_t battle_id) {
	do_turn ();
	correct_hp_and_report_errors (*first);
	correct_hp_and_report_errors (*last);
	client.taunt_foe(battle_id);
}

Moves GenericBattle::determine_action(network::GenericClient & client) {
	std::cout << std::string (20, '=') + '\n';
	std::cout << "Predicting...\n";
	Team predicted = predict_team(client.detailed(), foe, ai.all_pokemon().size());
	std::cout << predicted.to_string ();

	return expectiminimax(ai, predicted, weather, depth, client.score(), random_engine);
}

void GenericBattle::handle_use_move (Party const user, uint8_t slot, Moves move_name) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	Team & active = (my_party == user) ? ai : foe;
	Team & inactive = (my_party == user) ? foe : ai;

	if (first == nullptr) {
		first = &active;
		last = &inactive;
	}

	active.move();
	Pokemon & replacement = active.all_pokemon().at_replacement();
	if (!replacement.move.set_index_if_found(move_name)) {
		replacement.move.add(move_name, 3, inactive.all_pokemon().real_size());
	}
	replacement.move().variable.reset_index();
	if (replacement.move().is_damaging())
		move_damage = true;
}

void GenericBattle::handle_send_out (Party const switcher, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	Team & active = (my_party == switcher) ? ai : foe;
	Team & inactive = (my_party == switcher) ? foe : ai;

	if (first == nullptr) {
		first = &active;
		last = &inactive;
	}

	// This is needed to make sure I don't overwrite important information in a
	// situation in which a team switches multiple times in one turn (due to
	// replacing fainted Pokemon).
	auto const replacement = active.all_pokemon().replacement();
	
	// This assumes Species Clause is in effect
	if (!active.all_pokemon().seen(species)) {
		active.add_pokemon (species, level, gender, nickname);
		active.all_pokemon().at_replacement ().new_hp = max_damage_precision();
	}

	Pokemon & phazer = inactive.all_pokemon().at_replacement();
	if (phazer.move().is_phaze()) {
		phazer.move().variable.set_phaze_index(active, species);
	}
	else if (!active.pokemon().moved()) {
		Pokemon & pokemon = active.pokemon(replacement);
		pokemon.move.set_index(pokemon.index_of_first_switch() + active.all_pokemon().replacement());
	}
}

void GenericBattle::handle_hp_change (Party const changing, uint8_t slot, uint16_t change_in_hp, uint16_t remaining_hp, uint16_t denominator) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	bool const my_team = (my_party == changing);
	Team & changer = my_team ? ai : foe;
	Team & other = my_team ? foe : ai;
	Pokemon & pokemon = changer.all_pokemon().at_replacement();
	if (move_damage) {
		if (other.all_pokemon().at_replacement().move().affects_target(changer.pokemon(), weather))
			changer.pokemon().do_damage(std::min(static_cast<uint16_t>(pokemon.hp.max * change_in_hp / denominator), pokemon.hp.stat));
		move_damage = false;
	}
	pokemon.new_hp = remaining_hp;
}

void GenericBattle::correct_hp_and_report_errors (Team & team) {
	team.all_pokemon().for_each([this, & team] (Pokemon & pokemon) {
		Stat::stat_type const max_hp = team.is_me() ? pokemon.hp.max : max_damage_precision();
		Stat::stat_type const tm_estimate = max_hp * pokemon.hp.stat / pokemon.hp.max;
		if (tm_estimate == pokemon.new_hp)
			return;
		Stat::stat_type const reported_hp = pokemon.new_hp * pokemon.hp.max / max_hp;
		if (!(tm_estimate - 1 <= pokemon.new_hp and pokemon.new_hp <= tm_estimate + 1)) {
			std::cerr << "Uh oh! " + pokemon.to_string () + " has the wrong HP! The server reports ";
			if (!team.is_me())
				std::cerr << "approximately ";
			std::cerr << reported_hp << " HP remaining, but TM thinks it has " << pokemon.hp.stat << ".\n";
			std::cerr << "max_hp: " << max_hp << '\n';
			std::cerr << "pokemon.hp.max: " << pokemon.hp.max << '\n';
			std::cerr << "pokemon.hp.stat: " << pokemon.hp.stat << '\n';
			std::cerr << "pokemon.new_hp: " << pokemon.new_hp << '\n';
			std::cerr << "tm_estimate: " << tm_estimate << '\n';
		}
		pokemon.hp.stat = reported_hp;
	});
}

void GenericBattle::handle_set_pp (Party const changer, uint8_t slot, uint8_t pp) {
	// This function may actually be useless. I believe that any PP change is
	// already handled by other mechanisms.
}

void GenericBattle::handle_fainted (Party const fainting, uint8_t slot) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	Team & fainter = (my_party == fainting) ? ai : foe;
	fainter.all_pokemon().at_replacement().faint();
}

void GenericBattle::handle_end (network::GenericClient & client, Result const result) const {
	std::string const verb = to_string (result);
	client.print_with_time_stamp(std::cout, verb + " a battle vs. " + opponent());
	if (result == Result::lost) {
		pl::write_team(predict_foe_team(client.detailed()), client.generate_team_file_name());
	}
}

uint8_t GenericBattle::switch_slot (Moves move) const {
	uint8_t const slot = static_cast<uint8_t>(Move::to_replacement(move));
	Species const name = ai.pokemon(slot).name();
	for (uint8_t n = 0; n != slot_memory.size(); ++n) {
		if (slot_memory [n] == name)
			return n;
	}
	throw PokemonNotFound(name);
}

uint16_t GenericBattle::max_damage_precision () const {
	return 48;
}

void GenericBattle::initialize_turn () {
	ai.reset_between_turns();
	foe.reset_between_turns();
	// Simulators might not send an HP change message if a move does 0 damage.
	move_damage = false;
	
	first = nullptr;
	last = nullptr;
}

void GenericBattle::do_turn () {
	first->move(false);
	last->move(false);
	if (first->pokemon().replacing()) {
		normalize_hp ();
		switchpokemon (*first, *last, weather);
		first->move(false);
		normalize_hp ();
		if (last->pokemon().replacing()) {
			switchpokemon (*last, *first, weather);
			last->move(false);
			normalize_hp ();
			last->pokemon().not_replacing();
		}
		first->pokemon().not_replacing();
	}
	else {
		std::cout << "First move: " + first->pokemon().to_string() + " uses " + first->pokemon().move().to_string() + '\n';
		std::cout << "Last move: " + last->pokemon().to_string() + " uses " + last->pokemon().move().to_string() + '\n';
		// Anything with recoil will mess this up
		
		constexpr bool damage_is_known = true;
		call_move(*first, *last, weather, damage_is_known);
		last->pokemon().normalize_hp ();

		call_move(*last, *first, weather, damage_is_known);
		first->pokemon().normalize_hp ();

		endofturn (*first, *last, weather);
		normalize_hp ();
		
		// I only have to check if the foe fainted because if I fainted, I have
		// to make a decision to replace that Pokemon. I update between each
		// decision point so that is already taken into account.
		auto & pokemon = foe.pokemon();
		while (pokemon.will_be_replaced()) {
			// I suspect this check of is_switch() is not needed and may
			// actually be wrong, but I'm not sure, so I'm leaving it as is.
			if (!pokemon.move().is_switch()) {
				pokemon.update_to_correct_switch();
			}
			call_move(foe, ai, weather, damage_is_known);
		}
	}
	std::cout << first->to_string ();
	std::cout << last->to_string ();
}

void GenericBattle::normalize_hp () {
	ai.pokemon().normalize_hp ();
	foe.pokemon().normalize_hp ();
}

std::string const & GenericBattle::opponent() const {
	return opponent_name;
}


} // namespace technicalmachine
