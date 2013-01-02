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
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "battle_result.hpp"

#include "network/client.hpp"
#include "network/invalid_simulator_data.hpp"
#include "network/outmessage.hpp"

#include "pokemon_lab/write_team_file.hpp"

#include "../endofturn.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../evaluate/expectiminimax.hpp"

#include "../move/move.hpp"
#include "../move/moves_forward.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon_not_found.hpp"

#include "../team_predictor/team_predictor.hpp"

namespace technicalmachine {
class DetailedStats;

GenericBattle::GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, std::string const & team_file_name):
	opponent_name (_opponent),
	random_engine (seed),
	ai(random_engine, team_file_name),
	updated_hp(ai),
	depth (battle_depth)
	{
	initialize();
}

GenericBattle::GenericBattle (std::random_device::result_type seed, std::string const & _opponent, unsigned battle_depth, Team const & team):
	opponent_name(_opponent),
	random_engine (seed),
	ai (team),
	updated_hp(ai),
	depth (battle_depth)
	{
	initialize();
}

void GenericBattle::initialize() {
	for (auto const & pokemon : ai.all_pokemon()) {
		slot_memory.emplace_back(pokemon.name());
	}
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
	return predict_team(detailed, foe, random_engine);
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
			// TODO: fix for 2v2
			auto const target = my_party.other();
			msg.write_move(battle_id, move_index, target.value());
		}
	}
	else {
		msg.write_move (battle_id, 1);
	}
	if (!ai.pokemon().will_be_replaced())
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
	Team predicted = predict_foe_team(client.detailed());
	std::cout << predicted.to_string ();

	return expectiminimax(ai, predicted, weather, depth, client.evaluation_constants(), random_engine);
}

void GenericBattle::handle_use_move (Party const user, uint8_t slot, Moves move_name) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	Team & active = is_me(user) ? ai : foe;
	Team & inactive = is_me(user) ? foe : ai;

	if (first == nullptr) {
		first = &active;
		last = &inactive;
	}

	active.move();
	Pokemon & replacement = active.replacement();
	if (!replacement.move.set_index_if_found(move_name)) {
		replacement.move.add(move_name, 3);
	}
	if (replacement.move().is_damaging())
		move_damage = true;
}

void GenericBattle::handle_send_out (Party const switcher_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, uint8_t level) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	Team & switcher = get_team(switcher_party);
	Team & other = get_team(switcher_party.other());

	if (first == nullptr) {
		first = &switcher;
		last = &other;
	}

	// This is needed to make sure I don't overwrite important information in a
	// situation in which a team switches multiple times in one turn (due to
	// replacing fainted Pokemon).
	auto const replacement = switcher.all_pokemon().replacement();
	if (switcher.is_me())
		std::cerr << switcher.pokemon(replacement).to_string() << '\n';
	
	// This assumes Species Clause is in effect
	bool const added = switcher.all_pokemon().add_if_not_present(species, level, gender, nickname);
	if (added) {
		updated_hp.add(switcher.is_me(), switcher.replacement(), max_damage_precision());
	}

	Pokemon & phazer = other.replacement();
	if (phazer.move().is_phaze()) {
		variable(other).set_phaze_index(switcher, species);
	}
	else if (!switcher.pokemon().moved()) {
		Pokemon & pokemon = switcher.pokemon(replacement);
		pokemon.move.set_index(pokemon.index_of_first_switch() + switcher.all_pokemon().replacement());
	}
}

void GenericBattle::handle_hp_change(Party const changing, uint8_t slot, unsigned remaining_hp) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	Team const & team = get_team(changing);
	updated_hp.update(team.is_me(), team.replacement(), remaining_hp);
}

void GenericBattle::handle_direct_damage(Party const damaged, uint8_t const slot, unsigned const visible_damage) {
	Team const & team = get_team(damaged);
	auto const & pokemon = team.replacement();
	std::cerr << "is me: " << team.is_me() << '\n';
	std::cerr << pokemon.to_string() << '\n';
	assert(move_damage);
	Rational const change(visible_damage, max_visible_hp_change(team));
	auto const damage = pokemon.stat(Stat::HP).max * change;
	updated_hp.direct_damage(team.is_me(), pokemon, damage);
	move_damage = false;
}

int GenericBattle::hp_change(Party const changing, unsigned const remaining_hp) const {
	Team const & team = get_team(changing);
	unsigned const max_visible = max_visible_hp_change(team);
	if (max_visible < remaining_hp) {
		throw network::InvalidSimulatorData (remaining_hp, 0u, max_visible, team.who() + "'s remaining_hp");
	}
	unsigned const measurable_hp = max_visible * team.replacement().current_hp();
	return static_cast<int>(measurable_hp - remaining_hp);
}

unsigned GenericBattle::max_visible_hp_change(Party const changer) const {
	return max_visible_hp_change(get_team(changer));
}
unsigned GenericBattle::max_visible_hp_change(Team const & changer) const {
	return max_visible_hp_change(changer.is_me(), changer.replacement());
}
unsigned GenericBattle::max_visible_hp_change(bool const my_pokemon, Pokemon const & changer) const {
	return my_pokemon ? changer.stat(Stat::HP).max : max_damage_precision();
}

bool GenericBattle::is_valid_hp_change(Party changer, unsigned remaining_hp, int received_change) const {
	return hp_change(changer, remaining_hp) == received_change;
}

bool GenericBattle::is_valid_precision(Party changer, unsigned precision) const {
	return max_visible_hp_change(changer) == precision;
}


void GenericBattle::correct_hp_and_report_errors (Team & team) {
	for (auto & pokemon : team.all_pokemon()) {
		auto const tm_estimate = max_visible_hp_change(team.is_me(), pokemon) * pokemon.current_hp();
		auto const new_hp = updated_hp.get(team.is_me(), pokemon);
		if (tm_estimate == new_hp)
			return;
		auto const reported_hp = new_hp * pokemon.stat(Stat::HP).max / max_visible_hp_change(team.is_me(), pokemon);
		unsigned const min_value = (tm_estimate == 0) ? 0 : (tm_estimate - 1);
		unsigned const max_value = tm_estimate + 1;
		assert(max_value > tm_estimate);
		if (!(min_value <= new_hp and new_hp <= max_value)) {
			std::cerr << "Uh oh! " + pokemon.to_string() + " has the wrong HP! The server reports ";
			if (!team.is_me())
				std::cerr << "approximately ";
			std::cerr << reported_hp << " HP remaining, but TM thinks it has " << pokemon.stat(Stat::HP).stat << ".\n";
			std::cerr << "max_visible_hp_change: " << max_visible_hp_change(team.is_me(), pokemon) << '\n';
			std::cerr << "pokemon.hp.max: " << pokemon.stat(Stat::HP).max << '\n';
			std::cerr << "new_hp: " << new_hp << '\n';
			std::cerr << "tm_estimate: " << tm_estimate << '\n';
			assert(false);
		}
		pokemon.correct_error_in_hp(reported_hp);
	}
}

void GenericBattle::handle_set_pp (Party const changer, uint8_t slot, uint8_t pp) {
	// This function may actually be useless. I believe that any PP change is
	// already handled by other mechanisms.
}

void GenericBattle::handle_fainted (Party const fainter, uint8_t slot) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	auto const team = get_team(fainter);
	std::cerr << team.pokemon().to_string() << " fainted\n";
	updated_hp.faint(team.is_me(), team.pokemon());
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
	updated_hp.reset_between_turns();
	// Simulators might not send an HP change message if a move does 0 damage.
	move_damage = false;
	
	first = nullptr;
	last = nullptr;
}

void GenericBattle::do_turn () {
	first->move(false);
	last->move(false);
	auto const replacement = [&](Team & switcher, Team & other) {
		switchpokemon(switcher, other, weather);
		switcher.move(false);
		normalize_hp();
	};
	if (first->pokemon().will_be_replaced()) {
		normalize_hp();
		replacement(*first, *last);
		if (last->pokemon().will_be_replaced()) {
			replacement(*last, *first);
		}
	}
	else if (last->pokemon().will_be_replaced()) {
		normalize_hp();
		replacement(*last, *first);
	}
	else {
		std::cout << "First move: " + first->pokemon().to_string() + " uses " + first->pokemon().move().to_string() + '\n';
		std::cout << "Last move: " + last->pokemon().to_string() + " uses " + last->pokemon().move().to_string() + '\n';
		// Anything with recoil will mess this up
		
		constexpr bool damage_is_known = true;
		std::cerr << "First\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';

		register_damage();
		call_move(*first, *last, weather, variable(*first), damage_is_known);
		std::cerr << "Second\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';
		normalize_hp(*last);
		std::cerr << "Third\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';

		register_damage();
		call_move(*last, *first, weather, variable(*last), damage_is_known);
		std::cerr << "Fourth\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';
		normalize_hp(*first);
		std::cerr << "Fifth\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';

		register_damage();
		endofturn (*first, *last, weather);
		std::cerr << "Sixth\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';
		normalize_hp ();
		std::cerr << "Seventh\n";
		std::cerr << "AI HP: " << ai.pokemon().stat(Stat::HP).stat << '\n';
		std::cerr << "Foe HP: " << foe.pokemon().stat(Stat::HP).stat << '\n';
		
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
			call_move(foe, ai, weather, foe_variable, damage_is_known);
		}
	}
	std::cout << first->to_string ();
	std::cout << last->to_string ();
}

namespace {
void register_individual_damage(Team & team, UpdatedHP const & updated_hp) {
	auto const damage = updated_hp.damage(team.is_me(), team.pokemon());
	team.pokemon().register_damage(damage);
}
}

void GenericBattle::register_damage() {
	register_individual_damage(ai, updated_hp);
	register_individual_damage(foe, updated_hp);
}

void GenericBattle::normalize_hp() {
	normalize_hp(ai);
	normalize_hp(foe);
}

void GenericBattle::normalize_hp(Team & team) {
	bool const fainted = updated_hp.is_fainted(team.is_me(), team.pokemon());
	team.pokemon().normalize_hp(fainted);
}

std::string const & GenericBattle::opponent() const {
	return opponent_name;
}

Team const & GenericBattle::get_team(Party party) const {
	return is_me(party) ? ai : foe;
}
Team & GenericBattle::get_team(Party party) {
	return is_me(party) ? ai : foe;
}

Variable const & GenericBattle::variable(Team const & team) const {
	return team.is_me() ? ai_variable : foe_variable;
}
Variable & GenericBattle::variable(Team const & team) {
	return team.is_me() ? ai_variable : foe_variable;
}

void GenericBattle::handle_flinch(Party const party) {
	variable(get_team(party)).set_flinch(true);
}

void GenericBattle::handle_miss(Party const party) {
	get_team(party).pokemon().set_miss(true);
}

void GenericBattle::handle_critical_hit(Party const party) {
	get_team(party).pokemon().set_critical_hit(true);
}

void GenericBattle::handle_ability_message(Party party, Ability::Abilities ability) {
	get_team(party).replacement().ability().name = ability;
}

void GenericBattle::handle_item_message(Party party, Item::Items item) {
	get_team(party).replacement().item().name = item;
}

void GenericBattle::slot_memory_bring_to_front() {
	for (Species & name : slot_memory) {
		if (ai.replacement().name() == name)
			std::swap (slot_memory.front(), name);
	}
}

} // namespace technicalmachine
