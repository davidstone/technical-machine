// Generic battle
// Copyright (C) 2013 David Stone
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

#include <boost/filesystem.hpp>

#include "battle_result.hpp"
#include "client.hpp"
#include "random_string.hpp"

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

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include "../team_predictor/team_predictor.hpp"

namespace technicalmachine {
class DetailedStats;

Battle::Battle(std::string const & _opponent, std::random_device::result_type seed, unsigned battle_depth, std::string const & team_file_name):
	opponent_name (_opponent),
	random_engine (seed),
	ai(random_engine, team_file_name),
	updated_hp(ai),
	depth (battle_depth)
	{
	initialize();
}

Battle::Battle(std::string const & _opponent, std::random_device::result_type seed, unsigned battle_depth, Team const & team):
	opponent_name(_opponent),
	random_engine (seed),
	ai (team),
	updated_hp(ai),
	depth (battle_depth)
	{
	initialize();
}

void Battle::initialize() {
	for (auto const & pokemon : ai.all_pokemon()) {
		slot_memory.emplace_back(pokemon);
	}
	initialize_turn ();
}

bool Battle::is_me (Party const other_party) const {
	return my_party == other_party;
}

void Battle::set_party_if_unknown(Party const new_party) {
	my_party.set_if_unknown(new_party);
}

void Battle::write_team (network::OutMessage & msg, std::string const & username) const {
	msg.write_team (ai, username);
}

Team Battle::predict_foe_team (DetailedStats const & detailed) const {
	return predict_team(detailed, foe, random_engine);
}

void Battle::handle_begin_turn (uint16_t turn_count) const {
	std::cout << "Begin turn " << turn_count << '\n';
}

void Battle::handle_request_action(DetailedStats const & detailed, Evaluate const & evaluate, network::OutMessage & msg, uint32_t battle_id, bool can_switch, std::vector<uint8_t> const & attacks_allowed, bool forced) {
	// At some point, I will create a fail-safe that actually checks that the
	// move TM tries to use is considered a valid move by the server.
	update_from_previous_turn();
	if (!forced) {
		Moves const move = determine_action(detailed, evaluate);
		if (is_switch(move))
			msg.write_switch (battle_id, switch_slot (move));
		else {
			uint8_t move_index = 0;
			while (ai.pokemon().move(move_index) != move)
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

void Battle::update_from_previous_turn() {
	do_turn ();
	correct_hp_and_report_errors (*first);
	correct_hp_and_report_errors (*last);
}

Moves Battle::determine_action(DetailedStats const & detailed, Evaluate const & evaluate) {
	std::cout << std::string (20, '=') + '\n';
	std::cout << "Predicting...\n";
	Team predicted = predict_foe_team(detailed);
	std::cout << to_string(predicted) << '\n';

	return expectiminimax(ai, predicted, weather, depth, evaluate, random_engine);
}

void Battle::handle_use_move (Party const user, uint8_t slot, Moves move_name) {
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
		replacement.move.add(move_name);
	}
	if (is_damaging(replacement.move()))
		move_damage = true;
}

void Battle::handle_send_out (Party const switcher_party, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, Level const level) {
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
		std::cerr << to_string(static_cast<Species>(switcher.pokemon(replacement))) << '\n';
	
	// This assumes Species Clause is in effect
	bool const added = switcher.all_pokemon().add_if_not_present(species, level, gender, nickname);
	if (added) {
		updated_hp.add(switcher.is_me(), switcher.replacement(), max_damage_precision());
	}
	
	// TODO: I'm skeptical of this logic
	if (other.number_of_seen_pokemon() != 0 and is_phaze(other.replacement().move())) {
		variable(other).set_phaze_index(switcher, species);
	}
	else if (!switcher.pokemon().moved()) {
		Pokemon & pokemon = switcher.pokemon(replacement);
		pokemon.move.set_index(pokemon.index_of_first_switch() + switcher.all_pokemon().replacement());
	}
}

void Battle::handle_hp_change(Party const changing, uint8_t slot, unsigned remaining_hp) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	Team const & team = get_team(changing);
	updated_hp.update(team.is_me(), team.replacement(), remaining_hp);
}

void Battle::handle_direct_damage(Party const damaged, uint8_t const slot, unsigned const visible_damage) {
	Team const & team = get_team(damaged);
	auto const & pokemon = team.replacement();
	std::cerr << "is me: " << team.is_me() << '\n';
	std::cerr << to_string(static_cast<Species>(pokemon)) << '\n';
	assert(move_damage);
	Rational const change(visible_damage, max_visible_hp_change(team));
	auto const damage = static_cast<unsigned>(get_stat(pokemon, StatNames::HP).max) * change;
	updated_hp.direct_damage(team.is_me(), pokemon, damage);
	move_damage = false;
}

int Battle::hp_change(Party const changing, unsigned const remaining_hp) const {
	Team const & team = get_team(changing);
	unsigned const max_visible = max_visible_hp_change(team);
	if (max_visible < remaining_hp) {
		throw network::InvalidSimulatorData (remaining_hp, 0u, max_visible, team.who() + "'s remaining_hp");
	}
	unsigned const measurable_hp = max_visible * hp_ratio(team.replacement());
	return static_cast<int>(measurable_hp - remaining_hp);
}

unsigned Battle::max_visible_hp_change(Party const changer) const {
	return max_visible_hp_change(get_team(changer));
}
unsigned Battle::max_visible_hp_change(Team const & changer) const {
	return max_visible_hp_change(changer.is_me(), changer.replacement());
}
unsigned Battle::max_visible_hp_change(bool const my_pokemon, Pokemon const & changer) const {
	return my_pokemon ? static_cast<unsigned>(get_stat(changer, StatNames::HP).max) : max_damage_precision();
}

bool Battle::is_valid_hp_change(Party changer, unsigned remaining_hp, int received_change) const {
	return hp_change(changer, remaining_hp) == received_change;
}

bool Battle::is_valid_precision(Party changer, unsigned precision) const {
	return max_visible_hp_change(changer) == precision;
}

namespace {

// The server reports Technical Machine's HP tracking is wrong
void correct_error_in_hp(Pokemon & pokemon, unsigned const correct_hp_stat) {
	get_stat(pokemon, StatNames::HP).stat = correct_hp_stat;
}

}	// namespace

void Battle::correct_hp_and_report_errors (Team & team) {
	for (auto & pokemon : team.all_pokemon()) {
		auto const tm_estimate = max_visible_hp_change(team.is_me(), pokemon) * hp_ratio(pokemon);
		auto const new_hp = updated_hp.get(team.is_me(), pokemon);
		if (tm_estimate == new_hp)
			return;
		auto const reported_hp = new_hp * static_cast<unsigned>(get_stat(pokemon, StatNames::HP).max) / max_visible_hp_change(team.is_me(), pokemon);
		unsigned const min_value = (tm_estimate == 0) ? 0 : (tm_estimate - 1);
		unsigned const max_value = tm_estimate + 1;
		assert(max_value > tm_estimate);
		if (!(min_value <= new_hp and new_hp <= max_value)) {
			std::cerr << "Uh oh! " + to_string(static_cast<Species>(pokemon)) + " has the wrong HP! The server reports ";
			if (!team.is_me())
				std::cerr << "approximately ";
			std::cerr << reported_hp << " HP remaining, but TM thinks it has " << get_stat(pokemon, StatNames::HP).stat << ".\n";
			std::cerr << "max_visible_hp_change: " << max_visible_hp_change(team.is_me(), pokemon) << '\n';
			std::cerr << "pokemon.hp.max: " << get_stat(pokemon, StatNames::HP).max << '\n';
			std::cerr << "new_hp: " << new_hp << '\n';
			std::cerr << "tm_estimate: " << tm_estimate << '\n';
//			assert(false);
		}
		correct_error_in_hp(pokemon, reported_hp);
	}
}

void Battle::handle_set_pp (Party const changer, uint8_t slot, uint8_t pp) {
	// This function may actually be useless. I believe that any PP change is
	// already handled by other mechanisms.
}

void Battle::handle_fainted (Party const fainter, uint8_t slot) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	auto const team = get_team(fainter);
	std::cerr << to_string(static_cast<Species>(team.pokemon())) << " fainted\n";
	updated_hp.faint(team.is_me(), team.pokemon());
}

namespace {

std::string get_extension() {
	// TODO: add support for other formats
	return ".sbt";
}

template<typename RandomEngine>
std::string generate_team_file_name(RandomEngine & random_engine) {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr unsigned file_name_length = 8;
	std::string foe_team_file;
	do {
		foe_team_file = "teams/foe/";
		foe_team_file += random_string(random_engine, file_name_length);
		foe_team_file += get_extension();
	} while (boost::filesystem::exists(foe_team_file));
	return foe_team_file;
}

}	// namespace

void Battle::handle_end (Client const & client, Result const result) const {
	client.print_with_time_stamp(std::cout, to_string(result) + " a battle vs. " + opponent());
	if (result == Result::lost) {
		pl::write_team(predict_foe_team(client.detailed()), generate_team_file_name(random_engine));
	}
}

uint8_t Battle::switch_slot (Moves move) const {
	auto const slot = static_cast<uint8_t>(to_replacement(move));
	Species const name = ai.pokemon(slot);
	for (uint8_t n = 0; n != slot_memory.size(); ++n) {
		if (slot_memory [n] == name)
			return n;
	}
	throw PokemonNotFound(name);
}

uint16_t Battle::max_damage_precision () const {
	return 48;
}

void Battle::initialize_turn () {
	ai.reset_between_turns();
	foe.reset_between_turns();
	updated_hp.reset_between_turns();
	// Simulators might not send an HP change message if a move does 0 damage.
	move_damage = false;
	
	first = nullptr;
	last = nullptr;
}

namespace {

void update_to_correct_switch(ActivePokemon & pokemon) {
	pokemon.all_moves().set_index(pokemon.all_pokemon().replacement());
}
}	// namespace

void Battle::do_turn () {
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
		std::cout << "First move: " << to_string(static_cast<Species>(first->pokemon())) << " uses " << to_string(first->pokemon().move()) << '\n';
		std::cout << "Last move: " << to_string(static_cast<Species>(last->pokemon())) << " uses " << to_string(last->pokemon().move()) << '\n';
		// Anything with recoil will mess this up
		
		constexpr bool damage_is_known = true;
		std::cerr << "First\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';

		register_damage();
		call_move(*first, *last, weather, variable(*first), damage_is_known);
		std::cerr << "Second\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';
		normalize_hp(*last);
		std::cerr << "Third\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';

		register_damage();
		call_move(*last, *first, weather, variable(*last), damage_is_known);
		std::cerr << "Fourth\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';
		normalize_hp(*first);
		std::cerr << "Fifth\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';

		register_damage();
		endofturn (*first, *last, weather);
		std::cerr << "Sixth\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';
		normalize_hp ();
		std::cerr << "Seventh\n";
		std::cerr << "AI HP: " << get_stat(ai.pokemon(), StatNames::HP).stat << '\n';
		std::cerr << "Foe HP: " << get_stat(foe.pokemon(), StatNames::HP).stat << '\n';
		
		// I only have to check if the foe fainted because if I fainted, I have
		// to make a decision to replace that Pokemon. I update between each
		// decision point so that is already taken into account.
		auto & pokemon = foe.pokemon();
		while (pokemon.will_be_replaced()) {
			// I suspect this check of is_switch() is not needed and may
			// actually be wrong, but I'm not sure, so I'm leaving it as is.
			if (!is_switch(pokemon.move())) {
				update_to_correct_switch(pokemon);
			}
			call_move(foe, ai, weather, foe_variable, damage_is_known);
		}
	}
	std::cout << to_string(*first) << '\n';
	std::cout << to_string(*last) << '\n';
}

namespace {
void register_individual_damage(Team & team, UpdatedHP const & updated_hp) {
	auto const damage = updated_hp.damage(team.is_me(), team.pokemon());
	team.pokemon().register_damage(damage);
}
}

void Battle::register_damage() {
	register_individual_damage(ai, updated_hp);
	register_individual_damage(foe, updated_hp);
}

void Battle::normalize_hp() {
	normalize_hp(ai);
	normalize_hp(foe);
}

void Battle::normalize_hp(Team & team) {
	bool const fainted = updated_hp.is_fainted(team.is_me(), team.pokemon());
	team.pokemon().normalize_hp(fainted);
}

std::string const & Battle::opponent() const {
	return opponent_name;
}

Team const & Battle::get_team(Party party) const {
	return is_me(party) ? ai : foe;
}
Team & Battle::get_team(Party party) {
	return is_me(party) ? ai : foe;
}

Variable const & Battle::variable(Team const & team) const {
	return team.is_me() ? ai_variable : foe_variable;
}
Variable & Battle::variable(Team const & team) {
	return team.is_me() ? ai_variable : foe_variable;
}

void Battle::handle_flinch(Party const party) {
	variable(get_team(party)).set_flinch(true);
}

void Battle::handle_miss(Party const party) {
	get_team(party).pokemon().set_miss(true);
}

void Battle::handle_critical_hit(Party const party) {
	get_team(party).pokemon().set_critical_hit(true);
}

void Battle::handle_ability_message(Party party, Ability::Abilities ability) {
	get_ability(get_team(party).replacement()) = ability;
}

void Battle::handle_item_message(Party party, Item::Items item) {
	get_item(get_team(party).replacement()).name = item;
}

void Battle::slot_memory_bring_to_front() {
	for (Species & name : slot_memory) {
		if (ai.replacement() == name)
			std::swap (slot_memory.front(), name);
	}
}

} // namespace technicalmachine
