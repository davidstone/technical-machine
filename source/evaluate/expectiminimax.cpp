// Expectiminimax
// Copyright (C) 2016 David Stone
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

#include "expectiminimax.hpp"

#include "evaluate.hpp"
#include "move_scores.hpp"
#include "reorder.hpp"
#include "transposition.hpp"

#include "../ability.hpp"
#include "../block.hpp"
#include "../endofturn.hpp"
#include "../switch.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../move/base_power.hpp"
#include "../move/is_switch.hpp"
#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon.hpp"

#include "../stat/calculate.hpp"
#include "../stat/chance_to_hit.hpp"
#include "../stat/stat.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include <bounded/integer_range.hpp>

#include <boost/timer.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
namespace {

// This prints out search equal to the maximum depth normally, but any deeper
// searches will also print out with a single tab. This is not recommended for
// depth greater than 2.
constexpr bool verbose = false;

double select_move_branch(Team & ai, Team & foe, Weather weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn);
double select_move_branch(Team & ai, Team & foe, Weather weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn, MoveScores & ai_scores, MoveScores & foe_scores);
double order_branch(Team & ai, Team & foe, Weather weather, unsigned depth, Evaluate const & evaluate);
double accuracy_branch(Team & first, Team & last, Weather weather, unsigned depth, Evaluate const & evaluate);
double random_move_effects_branch(Team & first, Team & last, Weather weather, unsigned depth, Evaluate const & evaluate);
double awaken_branch(Team & first, Team & last, Weather weather, unsigned depth, Evaluate const & evaluate);
double use_move_branch(Team & first, Team & last, Variable const & first_variable, Variable const & last_variable, Weather & weather, unsigned depth, Evaluate const & evaluate);
double use_move_and_follow_up(Team & user, Team & other, Variable const & user_variable, Variable const & other_variable, Weather & weather, unsigned depth, Evaluate const & evaluate);
double end_of_turn_branch(Team first, Team last, Weather weather, unsigned depth, Evaluate const & evaluate);
double end_of_turn_order_branch(Team & team, Team & other, Team * first, Team * last, Weather weather, unsigned depth, Evaluate const & evaluate);
double replace(Team & ai, Team & foe, Weather weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn);
double fainted(Team ai, Team foe, Weather weather, unsigned depth, Evaluate const & evaluate);

double initial_move_then_switch_branch(Team & switcher, Team const & other, Weather weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn = false);
double move_then_switch_branch(Team & switcher, Team const & other, Variable const & user_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn = false);
double switch_after_move_branch(Team switcher, Team other, Variable const & user_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate);

#if 0
Moves random_action (Team const & ai, Team const & foe, Weather weather, std::mt19937 & random_engine);
Moves random_switch(Team const & ai, std::mt19937 & random_engine);
std::vector<Moves> all_switches(TeamSize team_size, PokemonCollection::index_type index);
Moves random_move_or_switch (Team const & ai, Team const & foe, Weather weather, std::mt19937 & random_engine);
#endif

void print_best_move (Team const & team, Moves best_move, double score);
void print_action (Team const & team, bool first_turn);
void print_estimated_score (bool first_turn, bool is_me, double estimate);
void update_best_move (double & alpha, double beta, bool first_turn, Moves new_move, Moves & best_move);
void update_foe_best_move (Team & foe, MoveScores & foe_scores, double & beta, double max_score, bool first_turn);

}	// namespace

Moves expectiminimax (Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, std::mt19937 &) {
	std::cout << std::string (20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	double min_score = 0.0;
	boost::timer timer;
//	Moves best_move = random_action (ai, foe, weather, random_engine);
	Moves best_move = Moves::Switch0;
	try {
		for (unsigned deeper = 1; deeper <= depth; ++deeper) {
			bool const full_evaluation = (deeper == depth);
			min_score = select_type_of_move(ai, foe, weather, deeper, evaluate, best_move, full_evaluation);
		}
	}
	catch (InvalidCollectionIndex const & ex) {
		// If there was an error, I just use the data from the deepest search I
		// could do that did not encounter that error.
		std::cerr << std::string (20, '=') + '\n';
		std::cerr << ex.what ();
		std::cerr << std::string (20, '=') + '\n';
	}
	std::cout << "Determined best move in " << timer.elapsed() << " seconds.\n";
	print_best_move (ai, best_move, min_score);
	return best_move;
}

double select_type_of_move(Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	assert(depth > 0);
	--depth;
	
	if (get_hp(ai.pokemon()) == 0_bi or get_hp(foe.pokemon()) == 0_bi) {
		return replace(ai, foe, weather, depth, evaluate, best_move, first_turn);
	} else if (switch_decision_required(ai.pokemon())) {
		return initial_move_then_switch_branch(ai, foe, weather, depth, evaluate, best_move, first_turn);
	} else {
		assert(!switch_decision_required(foe.pokemon()));
		return select_move_branch(ai, foe, weather, depth, evaluate, best_move, first_turn);
	}
}

namespace {

double select_move_branch(Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	MoveScores ai_scores(ai.pokemon());
	MoveScores foe_scores(foe.pokemon());
	return select_move_branch(ai, foe, weather, depth, evaluate, best_move, first_turn, ai_scores, foe_scores);
}

double select_move_branch(Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn, MoveScores & ai_scores, MoveScores & foe_scores) {
	// This calls itself at one lower depth in order to get an initial estimate
	// for ai_scores and foe_scores, because the algorithm works faster if you
	// start with the correct result. The results from one less depth are used
	// to estimate the correct result.
	if (depth >= 1) {
		select_move_branch(ai, foe, weather, depth - 1, evaluate, best_move, false, ai_scores, foe_scores);
	}
	auto const ai_index = reorder(LegalSelections(ai, foe.pokemon(), weather), ai_scores, true);
	auto const foe_index = reorder(LegalSelections(foe, ai.pokemon(), weather), foe_scores, false);

	// Working from the inside loop out:

	// The following begins by setting beta to the largest possible value. This
	// is the variable that the opposing player is trying to minimize. As long
	// as the opposing player has any move that won't guarantee their loss,
	// that move will score lower (more negative) than victory, and thus
	// the opponent will set that as their best response to the particular move
	// that the AI uses.

	// After looking at each response the opponent has to a given move, beta is
	// finally set to whatever the score will be if the AI uses that move.
	// alpha is initially set to the lowest possible value, so as long as the
	// AI has any move that won't guarantee its loss, that move will score
	// higher (more positive) than -victory, and thus the AI will set
	// that as its best response. It then replaces that move if it finds a move
	// for which the opponent's best response is more positive than the first
	// move found. In other words, it finds the move for the AI for which the
	// foe's best response is the weakest.
	
	// Something to consider as a potential speed up at the cost of some
	// accuracy (but would allow a deeper, thus more accurate, search) would be
	// to pick from all random numbers randomly, rather than seeing the outcome
	// of all of them and averaging it. In other words, do several trials
	// assuming a particular (but different for each trial) set of random
	// numbers are selected, and then average that result. This would give
	// massive reductions to the branching factor, and with a large enough
	// number of trials could be close enough to the average to potentially
	// speed up the program enough to justify the loss in accuracy.
	
	// I subtract 1 from -victory to make sure that even a guaranteed
	// loss is seen as better than not returning a result. This way, I can do
	// some things when my intermediate scores are strictly greater than alpha,
	// rather than greater than or equal to, which can save a few calculations.
	
	// For a similar reason, I later set beta to victory + 1.
	
	// This change also has the advantage of making sure a move is always put
	// into best_move without any additional logic, such as pre-filling it with
	// some result.
	
	auto alpha = static_cast<double>(-victory - 1_bi);
	for (auto const & ai_move : ai_index) {
		set_index(all_moves(ai.pokemon()), ai_move);
		print_action (ai, first_turn);
		auto beta = static_cast<double>(victory + 1_bi);
		for (auto const & foe_move : foe_index) {
			set_index(all_moves(foe.pokemon()), foe_move);
			print_action (foe, first_turn);
			auto const max_score = order_branch(ai, foe, weather, depth, evaluate);
			update_foe_best_move(foe, foe_scores, beta, max_score, first_turn);
			// Alpha-Beta pruning
			if (beta <= alpha)
				break;
		}
		ai_scores.set(current_move(ai.pokemon()), beta);
		update_best_move(alpha, beta, first_turn, current_move(ai.pokemon()), best_move);
		// The AI cannot have a better move than a guaranteed win
		if (alpha == static_cast<double>(victory))
			break;
	}
	return alpha;
}


void update_best_move (double & alpha, double beta, bool first_turn, Moves new_move, Moves & best_move) {
	// If their best response isn't as good as their previous best
	// response, then this new move must be better than the
	// previous AI's best move
	if (beta > alpha) {
		alpha = beta;
		best_move = new_move;
		constexpr bool is_me = true;
		print_estimated_score (first_turn, is_me, alpha);
	}
}

void update_foe_best_move (Team & foe, MoveScores & foe_scores, double & beta, double const max_score, bool const first_turn) {
	if (beta > max_score) {
		beta = max_score;
		foe_scores.set(current_move(foe.pokemon()), beta);
	}
	constexpr bool is_me = false;
	print_estimated_score (first_turn, is_me, max_score);
}

double order_branch(Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	// Determine turn order
	auto teams = order(ai, foe, weather); 
	bool const speed_tie = (teams.first == nullptr);
	return speed_tie ?
		(accuracy_branch(ai, foe, weather, depth, evaluate) + accuracy_branch(foe, ai, weather, depth, evaluate)) / 2.0 :
		accuracy_branch(*teams.first, *teams.second, weather, depth, evaluate);
}

double accuracy_branch(Team & first, Team & last, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	auto const set_flag = [](MutableActivePokemon pokemon, bool const flag) {
		pokemon.set_miss(!flag);
	};
	auto const probability = [=](auto const & user, auto const & target, bool const target_moved, bool const hit) {
		auto const base = chance_to_hit(user, target, weather, target_moved);
		assert(base >= 0.0);
		assert(base <= 1.0);
		return hit ? base : (1.0 - base);
	};


	double average_score = 0.0;
	for (auto const first_flag : { true, false }) {
		constexpr bool last_moved = false;
		auto const p1 = probability(first.pokemon(), last.pokemon(), last_moved, first_flag);
		if (p1 == 0.0) {
			continue;
		}
		set_flag(first.pokemon(), first_flag);
		for (auto const last_flag : { true, false }) {
			constexpr bool first_moved = true;
			auto const p2 = probability(last.pokemon(), first.pokemon(), first_moved, last_flag);
			if (p2 == 0.0) {
				continue;
			}
			set_flag(last.pokemon(), last_flag);
			average_score += awaken_branch(first, last, weather, depth, evaluate) * p1 * p2;
		}
	}
	return average_score;
}


template<typename SetFlag, typename Probability, typename NextBranch>
double generic_flag_branch(Team & first, Team & last, Weather const weather, unsigned depth, Evaluate const & evaluate, SetFlag const & set_flag, Probability const & basic_probability, NextBranch const & next_branch) {
	auto const probability = [&](auto const & pokemon, bool const flag) {
		auto const base = basic_probability(pokemon);
		assert(base >= 0.0);
		assert(base <= 1.0);
		return flag ? base : (1.0 - base);
	};

	double average_score = 0.0;
	for (auto const first_flag : { true, false }) {
		set_flag(first.pokemon(), first_flag);
		auto const p1 = probability(first.pokemon(), first_flag);
		if (p1 == 0.0) {
			continue;
		}
		for (auto const last_flag : { true, false }) {
			set_flag(last.pokemon(), last_flag);
			auto const p2 = probability(last.pokemon(), last_flag);
			if (p2 == 0.0) {
				continue;
			}
			average_score += next_branch(first, last, weather, depth, evaluate) * p1 * p2;
		}
	}
	return average_score;
}


double awaken_branch(Team & first, Team & last, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	auto const set_flag = [](MutableActivePokemon pokemon, bool const flag) {
		pokemon.awaken(flag);
	};
	return generic_flag_branch(first, last, weather, depth, evaluate, set_flag, awaken_probability, random_move_effects_branch);
}


bool can_critical_hit(Moves const move) {
	auto const power = base_power(move);
	return power and *power != 0_bi;
}

double random_move_effects_branch(Team & first, Team & last, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	auto const set_flag = [](MutableActivePokemon pokemon, bool const flag) {
		pokemon.set_critical_hit(flag);
	};
	auto const probability = [](ActivePokemon const pokemon) {
		return can_critical_hit(current_move(pokemon)) ? (1.0 / 16.0) : 0.0;
	};
	double score = 0.0;

	for (auto const & first_variable : all_probabilities(first.pokemon(), last.size())) {
		for (auto const & last_variable : all_probabilities(last.pokemon(), first.size())) {
			auto const use_move_copy_branch = [&](Team first_, Team last_, Weather weather_, unsigned depth_, Evaluate const & evaluate_) {
				return use_move_branch(first_, last_, first_variable, last_variable, weather_, depth_, evaluate_);
			};
			score += generic_flag_branch(first, last, weather, depth, evaluate, set_flag, probability, use_move_copy_branch) * last_variable.probability * first_variable.probability;
		}
	}
	return score;
}


double use_move_branch(Team & first, Team & last, Variable const & first_variable, Variable const & last_variable, Weather & weather, unsigned depth, Evaluate const & evaluate) {
	auto value = use_move_and_follow_up(first, last, first_variable, last_variable, weather, depth, evaluate);
	if (value != static_cast<double>(victory + 1_bi))	// illegal value
		return value;
	// If first uses a phazing move before last gets a chance to move, the
	// newly brought out Pokemon would try to move without checking to see if
	// it has already moved. This check is also necessary for my Baton Pass and
	// U-turn implementation to function.
	value = use_move_and_follow_up (last, first, last_variable, first_variable, weather, depth, evaluate);
	if (value != static_cast<double>(victory + 1_bi))
		return value;

	// Find the expected return on all possible outcomes at the end of the turn
	
	auto const teams = faster_pokemon(first, last, weather);
	auto const set_flag = [](MutableActivePokemon pokemon, bool const flag) {
		pokemon.shed_skin(flag);
	};
	// Partially apply some arguments to the function so it has the expected
	// signature
	auto const end_of_turn_order = [teams](Team & team, Team & other, Weather const weather_, unsigned depth_, Evaluate const & evaluate_) {
		return end_of_turn_order_branch(team, other, teams.first, teams.second, weather_, depth_, evaluate_);
	};
	return generic_flag_branch(first, last, weather, depth, evaluate, set_flag, shed_skin_probability, end_of_turn_order);
}

bool has_follow_up_decision(Moves const move) {
	switch (move) {
		case Moves::Baton_Pass:
		case Moves::U_turn:
			return true;
		default:
			return false;
	}
}

double use_move_and_follow_up(Team & user, Team & other, Variable const & user_variable, Variable const & other_variable, Weather & weather, unsigned depth, Evaluate const & evaluate) {
	auto const original = static_cast<Species>(user.pokemon());
	if (!moved(user.pokemon())) {
		call_move(user, other, weather, user_variable);
		auto const user_win = Evaluate::win(user);
		auto const other_win = Evaluate::win(other);
		if (user_win != 0_bi or other_win != 0_bi) {
			return static_cast<double>(user_win + other_win);
		}
		auto const current = static_cast<Species>(user.pokemon());
		if (original == current and has_follow_up_decision(current_move(user.pokemon())) and size(user.all_pokemon()) > 1_bi) {
			Moves phony = Moves::END;
			return move_then_switch_branch(user, other, user_variable, other_variable, weather, depth, evaluate, phony);
		}
	}
	return static_cast<double>(victory + 1_bi);		// return an illegal value
}


auto deorder(Team & first, Team & last) {
	assert(first.is_me() or last.is_me());
	struct Deorder {
		Team & ai;
		Team & foe;
	};
	return Deorder{
		(first.is_me()) ? first : last,
		(!first.is_me()) ? first : last
	};
}




double end_of_turn_order_branch(Team & team, Team & other, Team * first, Team * last, Weather const weather, unsigned depth, Evaluate const & evaluate) {
	bool const speed_tie = (first == nullptr);
	return speed_tie ?
		(end_of_turn_branch(team, other, weather, depth, evaluate) + end_of_turn_branch(other, team, weather, depth, evaluate)) / 2.0 :
		end_of_turn_branch(*first, *last, weather, depth, evaluate);
}

double end_of_turn_branch (Team first, Team last, Weather weather, unsigned depth, Evaluate const & evaluate) {
	end_of_turn(first, last, weather);

	auto const last_violated = Evaluate::sleep_clause(first);
	auto const first_violated = Evaluate::sleep_clause(last);
	if (last_violated != 0_bi or first_violated != 0_bi) {
		return static_cast<double>(last_violated + first_violated);
	}

	auto const first_win = Evaluate::win(first);
	auto const last_win = Evaluate::win(last);
	if (first_win != 0_bi or last_win != 0_bi) {
		return static_cast<double>(first_win + last_win);
	}

	auto const teams = deorder(first, last);
	return transposition(teams.ai, teams.foe, weather, depth, evaluate);
}



double replace (Team & ai, Team & foe, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	auto const teams = faster_pokemon(ai, foe, weather);
	bool const speed_tie = (teams.first == nullptr);
	unsigned const tabs = first_turn ? 0 : 2;
	auto alpha = static_cast<double>(-victory - 1_bi);
	auto const ai_break_out = [& ai]() { return get_hp(ai.pokemon()) != 0_bi; };
	ai.all_pokemon().for_each_replacement(ai_break_out, [&]() {
		if (verbose or first_turn)
			std::cout << std::string(tabs, '\t') + "Evaluating switching to " + to_string(static_cast<Species>(ai.replacement())) + "\n";
		auto beta = static_cast<double>(victory + 1_bi);
		auto const foe_break_out = [& foe, & alpha, & beta]() {
			return beta <= alpha or get_hp(foe.pokemon()) != 0_bi;
		};
		foe.all_pokemon().for_each_replacement(foe_break_out, [&]() {
			beta = (speed_tie) ?
				std::min(beta, (fainted(ai, foe, weather, depth, evaluate) + fainted(foe, ai, weather, depth, evaluate)) / 2.0) :
				std::min(beta, fainted(*teams.first, *teams.second, weather, depth, evaluate));
		});
		update_best_move(alpha, beta, first_turn, to_switch(ai.all_pokemon().replacement()), best_move);
	});
	return alpha;
}

double fainted(Team first, Team last, Weather weather, unsigned depth, Evaluate const & evaluate) {
	// Use pokemon() instead of replacement() because it checks whether the
	// current Pokemon needs to be replaced because it fainted.
	if (get_hp(first.pokemon()) == 0_bi) {
		switch_pokemon(first, last, weather, first.all_pokemon().replacement());
		auto const first_won = Evaluate::win(first);
		auto const last_won = Evaluate::win(last);
		if (first_won != 0_bi or last_won != 0_bi) {
			return static_cast<double>(first_won + last_won);
		}
	}
	if (get_hp(last.pokemon()) == 0_bi) {
		switch_pokemon(last, first, weather, last.all_pokemon().replacement());
		auto const first_won = Evaluate::win(first);
		auto const last_won = Evaluate::win(last);
		if (first_won != 0_bi or last_won != 0_bi) {
			return static_cast<double>(first_won + last_won);
		}
	}

	auto const teams = deorder(first, last);
	return transposition(teams.ai, teams.foe, weather, depth, evaluate);
}

double initial_move_then_switch_branch(Team & switcher, Team const & other, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn) {
	return move_then_switch_branch(switcher, other, Variable(), Variable(), weather, depth, evaluate, best_switch, first_turn);
}

double move_then_switch_branch(Team & switcher, Team const & other, Variable const & switcher_variable, Variable const & other_variable, Weather const weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn) {
	unsigned tabs = first_turn ? 0 : 2;
	auto alpha = static_cast<double>(-victory - 1_bi);
	if (!switcher.is_me()) {
		alpha = -alpha;
		++tabs;
	}
	switcher.all_pokemon().for_each_replacement([&]() {
		if (first_turn)
			std::cout << std::string (tabs, '\t') + "Evaluating bringing in " + to_string(static_cast<Species>(switcher.replacement())) + "\n";
		auto const value = switch_after_move_branch(switcher, other, switcher_variable, other_variable, weather, depth, evaluate);
		if (switcher.is_me()) {
			update_best_move(alpha, value, first_turn, to_switch(switcher.all_pokemon().replacement()), best_switch);
		}
		else {
			MoveScores foe_scores(switcher.pokemon());
			update_foe_best_move(switcher, foe_scores, alpha, value, first_turn);
		}
	});
	return alpha;
}

double switch_after_move_branch(Team switcher, Team other, Variable const & switcher_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate) {
	switch_pokemon(switcher, other, weather, switcher.all_pokemon().replacement());
	assert(!empty(switcher.all_pokemon()));
	assert(!empty(other.all_pokemon()));
	// I don't have to correct for which of the Pokemon moved first because
	// there are only two options:
	
	// Option 1: only the switcher has moved. Then it obviously went first and
	// I'm passing them in the proper order.
	
	// Option 2: Both Pokemon have moved. use_move_branch then
	// recalculates which Pokemon is faster to properly account for end-of-turn
	// effects. In this case, it doesn't matter what order I pass them.
	
	// I also do not need to worry about the game ending due to entry hazards,
	// because at the very least the Pokemon that used Baton Pass / U-turn is
	// still alive.

	return use_move_branch(switcher, other, switcher_variable, other_variable, weather, depth, evaluate);
}


#if 0
Moves random_action (Team const & ai, Team const & foe, Weather const weather, std::mt19937 & random_engine) {
	return switch_decision_required(ai.pokemon()) ?
		random_switch(ai, random_engine) :
		random_move_or_switch(ai, foe, weather, random_engine);
}

Moves random_switch(Team const & ai, std::mt19937 & random_engine) {
	std::vector<Moves> const switches = all_switches (ai.all_pokemon().size(), ai.all_pokemon().index());
	assert(!switches.empty());
	std::uniform_int_distribution<size_t> distribution { 0, switches.size() - 1 };
	size_t const index = distribution(random_engine);
	return switches[index];
}

std::vector<Moves> all_switches(TeamSize const team_size, PokemonCollection::index_type const index) {
	std::vector<Moves> switches;
	for (auto const n : bounded::integer_range(team_size)) {
		if (n != index) {
			switches.emplace_back(to_switch(n));
		}
	}
	return switches;
}

Moves random_move_or_switch (Team const & ai, Team const & foe, Weather const weather, std::mt19937 & random_engine) {
	LegalSelections const moves(ai, foe.pokemon(), weather);
	assert(moves.size() != 0_bi);
	std::uniform_int_distribution<size_t> distribution { 0, moves.size() - 1 };
	auto const index = distribution(random_engine);
	return moves[index];
}
#endif

void print_best_move (Team const & team, Moves const best_move, double score) {
	if (is_switch(best_move))
		std::cout << "Switch to " + to_string(static_cast<Species>(team.pokemon(to_replacement(best_move))));
	else
		std::cout << "Use " + to_string(best_move);
	std::cout << " for a minimum expected score of " << static_cast<std::int64_t>(score) << "\n";
}

void print_action (Team const & team, bool first_turn) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!team.is_me())
			++tabs;
		std::cout << std::string (tabs, '\t') << "Evaluating ";
		if (is_switch(current_move(team.pokemon()))) {
			auto const replacement_index = to_replacement(current_move(team.pokemon()));
			std::cout << "switching to " << to_string(static_cast<Species>(team.pokemon(replacement_index))) << '\n';
		}
		else {
			std::cout << to_string(current_move(team.pokemon())) << '\n';
		}
	}
}

void print_estimated_score (bool const first_turn, bool const is_me, double const estimate) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!is_me)
			++tabs;
		std::cout << std::string (tabs, '\t') + "Estimated score is " << static_cast<std::int64_t>(estimate) << '\n';
	}
}

}	// namespace
}	// namespace technicalmachine
