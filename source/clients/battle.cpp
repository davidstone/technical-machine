// Generic battle
// Copyright (C) 2017 David Stone
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

#include "battle_result.hpp"
#include "client.hpp"
#include "random_string.hpp"
#include "timestamp.hpp"

#include "network/invalid_simulator_data.hpp"
#include "network/outmessage.hpp"

#include "pokemon_lab/write_team_file.hpp"

#include "../endofturn.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../evaluate/expectiminimax.hpp"

#include "../move/is_switch.hpp"
#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon_not_found.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include "../team_predictor/team_predictor.hpp"

#include <bounded/integer_range.hpp>

#include <containers/algorithms/find.hpp>

#include <boost/filesystem.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
struct DetailedStats;

namespace {

auto make_team(std::random_device::result_type seed, boost::filesystem::path const & team_file) {
	std::mt19937 random_engine(seed);
	auto team = Team(random_engine, team_file);
	return std::make_tuple(std::move(random_engine), std::move(team));
}

}	// namespace

Battle::Battle(std::string opponent_, TeamSize const foe_size, std::random_device::result_type seed, unsigned battle_depth, boost::filesystem::path const & team_file):
	Battle(std::move(opponent_), foe_size, battle_depth, make_team(seed, team_file))
{
	initialize_turn();
}

Battle::Battle(std::string opponent_, TeamSize const foe_size, std::random_device::result_type seed, unsigned battle_depth, Team team):
	Battle(std::move(opponent_), foe_size, battle_depth, std::make_tuple(std::mt19937(seed), std::move(team)))
{
	initialize_turn();
}

Battle::Battle(std::string opponent_, TeamSize const foe_size, unsigned const battle_depth, std::tuple<std::mt19937, Team> tuple):
	opponent_name(std::move(opponent_)),
	random_engine(std::move(std::get<std::mt19937>(tuple))),
	ai(std::move(std::get<Team>(tuple))),
	foe(foe_size),
	slot_memory(begin(ai.team.all_pokemon()), end(ai.team.all_pokemon())),
	updated_hp(ai.team),
	depth(battle_depth)
	{
	initialize_turn();
}

bool Battle::is_me(Party const other_party) const {
	return my_party == other_party;
}

void Battle::set_party_if_unknown(Party const new_party) {
	set_if_unknown(my_party, new_party);
}

void Battle::write_team(network::OutMessage & msg, std::string const & username) const {
	msg.write_team(ai.team, username);
}

Team Battle::predict_foe_team(DetailedStats const & detailed) const {
	return predict_team(detailed, foe.team, random_engine);
}

void Battle::handle_begin_turn(uint16_t turn_count) const {
	std::cout << "Begin turn " << turn_count << '\n';
}

void Battle::handle_request_action(DetailedStats const & detailed, Evaluate const & evaluate, network::OutMessage & msg, uint32_t battle_id, bool can_switch, containers::static_vector<uint8_t, static_cast<intmax_t>(max_moves_per_pokemon)> const & attacks_allowed, bool forced) {
	// At some point, I will create a fail-safe that actually checks that the
	// move TM tries to use is considered a valid move by the server.
	update_from_previous_turn();
	if (!forced) {
		Moves const move = determine_action(detailed, evaluate);
		if (is_switch(move)) {
			// TODO: throw an exception
			assert(can_switch);
			msg.write_switch(battle_id, switch_slot(move));
		} else {
			// TODO: fix for 2v2
			auto const move_index = *index(all_moves(ai.team.pokemon()), move);
			auto const target = other(my_party);
			// TODO: verify everything lines up
			static_cast<void>(attacks_allowed);
			msg.write_move(battle_id, static_cast<uint8_t>(move_index), static_cast<uint8_t>(target.value()));
		}
	} else {
		msg.write_move(battle_id, 1);
	}
	if (!switch_decision_required(ai.team.pokemon())) {
		initialize_turn();
	}
}

void Battle::update_from_previous_turn() {
	do_turn();
	assert(first);
	assert(last);
	correct_hp_and_report_errors(first->team);
	correct_hp_and_report_errors(last->team);
}

Moves Battle::determine_action(DetailedStats const & detailed, Evaluate const & evaluate) {
	std::cout << std::string(20, '=') + '\n';
	std::cout << "Predicting...\n";
	auto predicted = predict_foe_team(detailed);
	std::cout << to_string(predicted) << '\n';

	return expectiminimax(ai.team, predicted, weather, depth, evaluate, random_engine);
}

void Battle::handle_use_move(Party const user, uint8_t /*slot*/, Moves move_name) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	auto & active = is_me(user) ? ai : foe;
	auto & inactive = is_me(user) ? foe : ai;

	if (first == nullptr) {
		first = std::addressof(active);
		last = std::addressof(inactive);
	}

	active.team.move();
	add_seen_move(all_moves(active.team.replacement()), move_name);
	if (is_damaging(move_name)) {
		move_damage = true;
	}
}

namespace {

auto set_index_of_seen(PokemonCollection & collection, Species const species) {
	for (auto const replacement : integer_range(size(collection))) {
		if (species == collection(replacement)) {
			collection.set_replacement(replacement);
			return true;
		}
	}
	return false;
}

template<typename...Args>
auto switch_or_add(PokemonCollection & collection, Species const species, Args&&... args) {
	auto const add_new_pokemon = !set_index_of_seen(collection, species);
	if (add_new_pokemon) {
		collection.add(species, std::forward<Args>(args)...);
	}
	return add_new_pokemon;
}

auto index_of_first_switch(Pokemon const & pokemon) {
	containers::index_type<MoveContainer> index = 0_bi;
	auto const & moves = all_moves(pokemon);
	while (!is_switch(moves(index))) {
		++index;
	}
	return index;
}

}	// namespace

void Battle::handle_send_out(Party const switcher_party, uint8_t /*slot*/, uint8_t /*index*/, std::string const & nickname, Species species, Gender gender, Level const level) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.

	auto & switcher = get_team(switcher_party);
	auto & other = get_team(technicalmachine::other(switcher_party));

	if (first == nullptr) {
		first = std::addressof(switcher);
		last = std::addressof(other);
	}

	// This is needed to make sure I don't overwrite important information in a
	// situation in which a team switches multiple times in one turn (due to
	// replacing fainted Pokemon).
	auto const replacement = switcher.team.all_pokemon().replacement();
	if (switcher.team.is_me()) {
		std::cerr << to_string(static_cast<Species>(switcher.team.pokemon(replacement))) << '\n';
	}
	
	// This assumes Species Clause is in effect
	auto const added = switch_or_add(switcher.team.all_pokemon(), species, level, gender, nickname);
	if (added) {
		updated_hp.add(switcher.team.is_me(), switcher.team.replacement(), max_damage_precision());
	}
	
	// TODO: I'm skeptical of this logic
	if (other.team.number_of_seen_pokemon() != 0_bi and is_phaze(current_move(other.team.replacement()))) {
		set_phaze_index(other.variable, switcher.team, species, current_move(switcher.team.pokemon()));
	} else if (!moved(switcher.team.pokemon())) {
		Pokemon & pokemon = switcher.team.pokemon(replacement);
		all_moves(pokemon).set_index(static_cast<containers::index_type<MoveCollection>>(index_of_first_switch(pokemon) + switcher.team.all_pokemon().replacement()));
	}
}

void Battle::handle_hp_change(Party const changing, uint8_t /*slot*/, UpdatedHP::VisibleHP remaining_hp) {
	// "slot" is only useful in NvN, which TM does not yet
	// support.
	auto const & team = get_team(changing).team;
	updated_hp.update(team.is_me(), team.replacement(), remaining_hp);
}

void Battle::handle_direct_damage(Party const damaged, uint8_t const /*slot*/, UpdatedHP::VisibleHP const visible_damage) {
	auto const & team = get_team(damaged).team;
	auto const & pokemon = team.replacement();
	std::cerr << "is me: " << team.is_me() << '\n';
	std::cerr << to_string(static_cast<Species>(pokemon)) << '\n';
	assert(move_damage);
	auto const change = make_rational(visible_damage, max_visible_hp_change(team));
	auto const damage = get_hp(pokemon).max() * change;
	updated_hp.direct_damage(team.is_me(), pokemon, damage);
	move_damage = false;
}

int Battle::hp_change(Party const changing, UpdatedHP::VisibleHP const remaining_hp) const {
	auto const & team = get_team(changing).team;
	auto const max_visible = max_visible_hp_change(team);
	if (max_visible < remaining_hp) {
		throw network::InvalidSimulatorData(remaining_hp, 0_bi, max_visible, team.who() + "'s remaining_hp");
	}
	auto const measurable_hp = max_visible * hp_ratio(team.replacement());
	return static_cast<int>(measurable_hp - remaining_hp);
}

auto Battle::max_visible_hp_change(Party const changer) const -> MaxVisibleHPChange {
	return max_visible_hp_change(get_team(changer).team);
}
auto Battle::max_visible_hp_change(Team const & changer) const -> MaxVisibleHPChange {
	return max_visible_hp_change(changer.is_me(), changer.replacement());
}
auto Battle::max_visible_hp_change(bool const my_pokemon, Pokemon const & changer) const -> MaxVisibleHPChange {
	return my_pokemon ? get_hp(changer).max() : max_damage_precision();
}

bool Battle::is_valid_hp_change(Party changer, UpdatedHP::VisibleHP remaining_hp, int received_change) const {
	return hp_change(changer, remaining_hp) == received_change;
}

bool Battle::is_valid_precision(Party changer, unsigned precision) const {
	return max_visible_hp_change(changer) == precision;
}

void Battle::correct_hp_and_report_errors(Team & team) {
	for (auto & pokemon : team.all_pokemon()) {
		auto const tm_estimate = max_visible_hp_change(team.is_me(), pokemon) * hp_ratio(pokemon);
		auto const new_hp = updated_hp.get(team.is_me(), pokemon);
		if (tm_estimate == new_hp) {
			continue;
		}
		auto const reported_hp = new_hp * get_hp(pokemon).max() / max_visible_hp_change(team.is_me(), pokemon);
		auto const min_value = BOUNDED_CONDITIONAL(tm_estimate == 0_bi, 0_bi, tm_estimate - 1_bi);
		auto const max_value = tm_estimate + 1_bi;
		assert(max_value > tm_estimate);
		if (!(min_value <= new_hp and new_hp <= max_value)) {
			std::cerr << "Uh oh! " + to_string(static_cast<Species>(pokemon)) + " has the wrong HP! The server reports ";
			if (!team.is_me()) {
				std::cerr << "approximately ";
			}
			std::cerr << reported_hp << " HP remaining, but TM thinks it has " << get_hp(pokemon).current() << ".\n";
			std::cerr << "max_visible_hp_change: " << max_visible_hp_change(team.is_me(), pokemon) << '\n';
			std::cerr << "pokemon.hp.max: " << get_hp(pokemon).max() << '\n';
			std::cerr << "new_hp: " << new_hp << '\n';
			std::cerr << "tm_estimate: " << tm_estimate << '\n';
//			assert(false);
		}
		get_hp(pokemon) = reported_hp;
	}
}

void Battle::handle_set_pp(Party const, uint8_t /*slot*/, uint8_t /*pp*/) {
	// This function may actually be useless. I believe that any PP change is
	// already handled by other mechanisms.
}

void Battle::handle_fainted(Party const fainter, uint8_t /*slot*/) {
	// "slot" is only useful in situations other than 1v1, which TM does not yet
	// support.
	auto const & team = get_team(fainter).team;
	std::cerr << to_string(static_cast<Species>(team.pokemon())) << " fainted\n";
	updated_hp.faint(team.is_me(), team.pokemon());
}

namespace {

std::string get_extension() {
	// TODO: add support for other formats
	return ".sbt";
}

template<typename RandomEngine>
boost::filesystem::path generate_team_file_name(RandomEngine & random_engine) {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr unsigned file_name_length = 8;
	boost::filesystem::path foe_team_file;
	do {
		foe_team_file = "teams/foe";
		foe_team_file /= random_string(random_engine, file_name_length) + get_extension();
	} while (boost::filesystem::exists(foe_team_file));
	return foe_team_file;
}

}	// namespace

void Battle::handle_end(Client const & client, Result const result) const {
	std::cout << timestamp() << ": " << to_string(result) << " a battle vs. " << opponent() << '\n';
	if (result == Result::lost) {
		pl::write_team(predict_foe_team(client.detailed()), generate_team_file_name(random_engine));
	}
}

uint8_t Battle::switch_slot(Moves move) const {
	Species const name = ai.team.pokemon(to_replacement(move));
	auto const it = containers::find(begin(slot_memory), end(slot_memory), name);
	if (it == end(slot_memory)) {
		throw PokemonNotFound(name);
	}
	return static_cast<std::uint8_t>(it - begin(slot_memory));
}

VisibleFoeHP Battle::max_damage_precision() const {
	return 48_bi;
}

void Battle::initialize_turn() {
	ai.team.reset_between_turns();
	foe.team.reset_between_turns();
	updated_hp.reset_between_turns();
	// Simulators might not send an HP change message if a move does 0 damage.
	move_damage = false;
	
	first = nullptr;
	last = nullptr;
	ai.flags = {};
	foe.flags = {};
}


void Battle::do_turn() {
	assert(first);
	assert(last);
	first->team.move(false);
	last->team.move(false);
	auto const replacement = [&](Team & switcher, Team & other) {
		switch_pokemon(switcher, other, weather, switcher.all_pokemon().replacement());
		switcher.move(false);
		normalize_hp();
	};
	if (switch_decision_required(first->team.pokemon())) {
		normalize_hp();
		replacement(first->team, last->team);
		if (switch_decision_required(last->team.pokemon())) {
			replacement(last->team, first->team);
		}
	} else if (switch_decision_required(last->team.pokemon())) {
		normalize_hp();
		replacement(last->team, first->team);
	} else {
		std::cout << "First move: " << to_string(static_cast<Species>(first->team.pokemon())) << " uses " << to_string(current_move(first->team.pokemon())) << '\n';
		std::cout << "Last move: " << to_string(static_cast<Species>(last->team.pokemon())) << " uses " << to_string(current_move(last->team.pokemon())) << '\n';
		// Anything with recoil will mess this up
		
		constexpr bool damage_is_known = true;
		std::cerr << "First\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';

		register_damage();
		
		call_move(first->team, current_move(first->team.pokemon()), last->team, weather, first->variable, first->flags.miss, first->flags.awakens, first->flags.critical_hit, damage_is_known);
		std::cerr << "Second\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';
		normalize_hp(last->team);
		std::cerr << "Third\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';

		register_damage();
		call_move(last->team, current_move(last->team.pokemon()), first->team, weather, last->variable, last->flags.miss, last->flags.awakens, last->flags.critical_hit, damage_is_known);
		std::cerr << "Fourth\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';
		normalize_hp(first->team);
		std::cerr << "Fifth\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';

		register_damage();
		end_of_turn(first->team, last->team, weather, first->flags.shed_skin, last->flags.shed_skin);
		std::cerr << "Sixth\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';
		normalize_hp();
		std::cerr << "Seventh\n";
		std::cerr << "AI HP: " << get_hp(ai.team.pokemon()).current() << '\n';
		std::cerr << "Foe HP: " << get_hp(foe.team.pokemon()).current() << '\n';
		
		// I only have to check if the foe fainted because if I fainted, I have
		// to make a decision to replace that Pokemon. I update between each
		// decision point so that is already taken into account.
		while (is_fainted(foe.team.pokemon())) {
			set_index(all_moves(foe.team.pokemon()), to_switch(foe.team.all_pokemon().replacement()));
			call_move(foe.team, current_move(foe.team.pokemon()), ai.team, weather, foe.variable, false, false, false, damage_is_known);
		}
	}
	std::cout << to_string(first->team) << '\n';
	std::cout << to_string(last->team) << '\n';
}

namespace {
void register_individual_damage(Team & team, UpdatedHP const & updated_hp) {
	auto const damage = updated_hp.damage(team.is_me(), team.pokemon());
	team.pokemon().register_damage(damage);
}
}

void Battle::register_damage() {
	register_individual_damage(ai.team, updated_hp);
	register_individual_damage(foe.team, updated_hp);
}

void Battle::normalize_hp() {
	normalize_hp(ai.team);
	normalize_hp(foe.team);
}

namespace {

// Fix any rounding issues caused by not seeing the foe's exact HP.
auto normalize_hp(MutableActivePokemon pokemon, bool const fainted) {
	if (fainted) {
		pokemon.faint();
	} else {
		HP & hp = get_hp(pokemon);
		hp = bounded::max(hp.current(), 1_bi);
	}
}

}	// namespace

void Battle::normalize_hp(Team & team) {
	bool const fainted = updated_hp.is_fainted(team.is_me(), team.pokemon());
	technicalmachine::normalize_hp(team.pokemon(), fainted);
}

std::string const & Battle::opponent() const {
	return opponent_name;
}

void Battle::handle_flinch(Party const party) {
	set_flinch(get_team(party).variable, true);
}

void Battle::handle_miss(Party const party) {
	get_team(party).flags.miss = true;
}

void Battle::handle_critical_hit(Party const party) {
	get_team(party).flags.critical_hit = true;
}

void Battle::handle_ability_message(Party party, Ability::Abilities ability) {
	get_ability(get_team(party).team.replacement()) = ability;
}

void Battle::handle_item_message(Party party, Item item) {
	get_item(get_team(party).team.replacement()) = item;
}

void Battle::slot_memory_bring_to_front() {
	auto const it = containers::find(begin(slot_memory), end(slot_memory), ai.team.replacement());
	if (it != end(slot_memory)) {
		std::swap(*it, front(slot_memory));
	}
}

} // namespace technicalmachine
