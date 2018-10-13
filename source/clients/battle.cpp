// Generic battle
// Copyright (C) 2018 David Stone
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
#include "random_string.hpp"
#include "timestamp.hpp"

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

#include <containers/algorithms/find.hpp>
#include <containers/integer_range.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
struct DetailedStats;

Battle::Battle(
	OverallStats const & overall,
	DetailedStats const & detailed,
	LeadStats const & lead,
	Multiplier const & multiplier,
	Evaluate const & evaluate,
	Party const party,
	std::string opponent_,
	unsigned const battle_depth,
	Team team,
	TeamSize const foe_size,
	VisibleFoeHP const max_damage_precision_
):
	m_overall(overall),
	m_detailed(detailed),
	m_lead(lead),
	m_multiplier(multiplier),
	m_evaluate(evaluate),
	m_opponent(std::move(opponent_)),
	m_ai(std::move(team)),
	m_foe(foe_size),
	m_updated_hp(m_ai.team),
	m_depth(battle_depth),
	m_max_damage_precision(max_damage_precision_),
	
	m_ai_party(party)
{
}

Team Battle::predict_foe_team(std::mt19937 & random_engine) const {
	return predict_team(m_overall, m_detailed, m_lead, m_multiplier, m_foe.team, random_engine);
}

Moves Battle::determine_action(std::mt19937 & random_engine) const {
	if (m_ai.team.size() == 0_bi or m_foe.team.size() == 0_bi) {
		std::cerr << "Tried to determine an action with an empty team.\n";
		return Moves::Struggle;
	}

	std::cout << std::string(20, '=') + '\n';
	std::cout << "Predicting...\n";
	auto predicted = predict_foe_team(random_engine);
	std::cout << to_string(predicted) << '\n';

	return expectiminimax(m_ai.team, predicted, m_weather, m_depth, m_evaluate);
}

void Battle::handle_use_move(Party const user, uint8_t /*slot*/, Moves const move_name) {
	auto & active = is_me(user) ? m_ai : m_foe;
	auto & inactive = is_me(user) ? m_foe : m_ai;

	if (m_first == nullptr) {
		m_first = std::addressof(active);
		m_last = std::addressof(inactive);
	}

	active.team.move();
	auto const move = add_seen_move(all_moves(active.team.replacement()), move_name);
	active.flags.used_move.emplace(move, 0_bi);
}

namespace {

auto set_index_of_seen(PokemonCollection & collection, Species const species) {
	for (auto const replacement : containers::integer_range(size(collection))) {
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

}	// namespace

void Battle::handle_send_out(Party const switcher_party, uint8_t /*slot*/, std::string const & nickname, Species const species, Gender const gender, Level const level) {
	auto & switcher = get_team(switcher_party);
	auto & other = get_team(technicalmachine::other(switcher_party));

	if (m_first == nullptr) {
		m_first = std::addressof(switcher);
		m_last = std::addressof(other);
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
		m_updated_hp.add(switcher.team.is_me(), switcher.team.replacement(), m_max_damage_precision);
	}
	
	if (other.team.number_of_seen_pokemon() != 0_bi and other.flags.used_move and is_phaze(other.flags.used_move->move)) {
		// Does not matter what move we put here if the Pokemon has not moved
		auto const switcher_move = switcher.flags.used_move ? switcher.flags.used_move->move : Moves::Struggle;
		set_phaze_index(other.variable, switcher.team, species, switcher_move);
	} else if (!moved(switcher.team.pokemon())) {
		switcher.flags.used_move.emplace(Move(to_switch(switcher.team.all_pokemon().replacement())), 0_bi);
	}
}

void Battle::handle_direct_damage(Party const damaged, uint8_t const /*slot*/, UpdatedHP::VisibleHP const visible_damage) {
	auto & battle_team = get_team(damaged);
	auto const & team = battle_team.team;
	auto const & pokemon = team.replacement();
	std::cerr << "is me: " << team.is_me() << '\n';
	std::cerr << to_string(static_cast<Species>(pokemon)) << '\n';
	auto const change = rational(visible_damage, max_visible_hp_change(team.is_me(), pokemon));
	auto const damage = get_hp(pokemon).max() * change;
	m_updated_hp.direct_damage(team.is_me(), pokemon, damage);
	battle_team.flags.damaged = damage;
}

void Battle::correct_hp_and_report_errors(Team & team) {
	for (auto & pokemon : team.all_pokemon()) {
		auto const max_visible_for_this_pokemon = max_visible_hp_change(team.is_me(), pokemon);
		auto const tm_estimate = max_visible_for_this_pokemon * hp_ratio(pokemon);
		auto const new_hp = m_updated_hp.get(team.is_me(), pokemon);
		if (tm_estimate == new_hp) {
			continue;
		}
		auto const reported_hp = new_hp * get_hp(pokemon).max() / max_visible_for_this_pokemon;
		auto const min_value = BOUNDED_CONDITIONAL(tm_estimate == 0_bi, 0_bi, tm_estimate - 1_bi);
		auto const max_value = tm_estimate + 1_bi;
		assert(max_value > tm_estimate);
		if (!(min_value <= new_hp and new_hp <= max_value)) {
			std::cerr << "Uh oh! " << to_string(static_cast<Species>(pokemon)) << " has the wrong HP! The server reports ";
			if (!team.is_me()) {
				std::cerr << "approximately ";
			}
			std::cerr << reported_hp << " HP remaining, but TM thinks it has " << get_hp(pokemon).current() << ".\n";
			std::cerr << "max_visible_hp_change: " << max_visible_for_this_pokemon << '\n';
			std::cerr << "pokemon.hp.max: " << get_hp(pokemon).max() << '\n';
			std::cerr << "new_hp: " << new_hp << '\n';
			std::cerr << "tm_estimate: " << tm_estimate << '\n';
//			assert(false);
		}
		get_hp(pokemon) = reported_hp;
	}
}

namespace {

std::string get_extension() {
	// TODO: add support for other formats
	return ".sbt";
}

std::filesystem::path generate_team_file_name(std::mt19937 & random_engine) {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr unsigned file_name_length = 8;
	std::filesystem::path foe_team_file;
	do {
		foe_team_file = "teams/foe";
		foe_team_file /= random_string(random_engine, file_name_length) + get_extension();
	} while (std::filesystem::exists(foe_team_file));
	return foe_team_file;
}

}	// namespace

void Battle::handle_end(Result const result, std::mt19937 & random_engine) const {
	std::cout << timestamp() << ": " << to_string(result) << " a battle vs. " << m_opponent << '\n';
	if (result == Result::lost) {
		pl::write_team(predict_foe_team(random_engine), generate_team_file_name(random_engine));
	}
}

void Battle::do_turn() {
	assert(m_first);
	assert(m_last);
	m_first->team.move(false);
	m_last->team.move(false);
	auto const replacement = [&](Team & switcher, Team & other) {
		switch_pokemon(switcher, other, m_weather, switcher.all_pokemon().replacement());
		switcher.move(false);
		normalize_hp();
	};
	if (switch_decision_required(m_first->team.pokemon())) {
		normalize_hp();
		replacement(m_first->team, m_last->team);
		if (switch_decision_required(m_last->team.pokemon())) {
			replacement(m_last->team, m_first->team);
		}
	} else if (switch_decision_required(m_last->team.pokemon())) {
		normalize_hp();
		replacement(m_last->team, m_first->team);
	} else {
		// Anything with recoil will mess this up

		auto call_battle_move = [&](auto const & name, auto & user, auto & other, bounded::optional<UsedMove> const other_move) {
			Species const species = user.team.pokemon();
			auto const used_move = user.flags.used_move->move;
			std::cout << name << " move: " << to_string(species) << " uses " << to_string(used_move) << '\n';

			call_move(user.team, used_move, static_cast<bool>(user.flags.damaged), other.team, other_move, static_cast<bool>(other.flags.damaged), m_weather, user.variable, user.flags.miss, user.flags.awakens, user.flags.critical_hit, other.flags.damaged);

			normalize_hp(other.team);
		};

		assert(m_first->flags.used_move);
		call_battle_move("First", *m_first, *m_last, bounded::none);

		if (m_last->flags.used_move) {
			call_battle_move("Last", *m_last, *m_first, m_first->flags.used_move);
		} else {
			std::cout << "Last has not moved?\n";
		}

		end_of_turn(m_first->team, m_last->team, m_weather, m_first->flags.shed_skin, m_last->flags.shed_skin);
		normalize_hp();
		
		// I only have to check if the foe fainted because if I fainted, I have
		// to make a decision to replace that Pokemon. I update between each
		// decision point so that is already taken into account.
		while (is_fainted(m_foe.team.pokemon())) {
			auto const move = to_switch(m_foe.team.all_pokemon().replacement());
			// TODO: It is not quite correct to construct a new move, but it
			// should not matter for this.
			m_foe.flags.used_move = UsedMove{Move(move), 0_bi};
			call_battle_move("Foe", m_foe, m_ai, m_ai.flags.used_move);
		}
	}
	std::cout << to_string(m_first->team) << '\n';
	std::cout << to_string(m_last->team) << '\n';
}

void Battle::normalize_hp() {
	normalize_hp(m_ai.team);
	normalize_hp(m_foe.team);
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
	bool const fainted = m_updated_hp.is_fainted(team.is_me(), team.pokemon());
	technicalmachine::normalize_hp(team.pokemon(), fainted);
}

} // namespace technicalmachine
