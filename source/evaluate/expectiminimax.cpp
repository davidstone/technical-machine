// Expectiminimax
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

#include "expectiminimax.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <boost/timer.hpp>

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

#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/pokemon.hpp"

#include "../stat/chance_to_hit.hpp"
#include "../stat/stat.hpp"

#include "../string_conversions/conversion.hpp"

namespace technicalmachine {
namespace {

// This prints out search equal to the maximum depth normally, but any deeper
// searches will also print out with a single tab. This is not recommended for
// depth greater than 2.
constexpr bool verbose = false;

int64_t select_move_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn);
int64_t order_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate);
int64_t accuracy_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate);
int64_t random_move_effects_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate);
int64_t awaken_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate);
int64_t use_move_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & first_variable, Variable const & last_variable, Weather & weather, unsigned depth, Evaluate const & evaluate);
int64_t use_move_and_follow_up (Team & user, Team & other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & user_variable, Variable const & other_variable, Weather & weather, unsigned depth, Evaluate const & evaluate);
int64_t end_of_turn_branch (Team first, Team last, MoveScores & ai_scores, MoveScores & foe_scores, Weather weather, unsigned depth, Evaluate const & evaluate);
int64_t end_of_turn_order_branch (Team & team, Team & other, MoveScores & ai_scores, MoveScores & foe_scores, Team * first, Team * last, Weather const & weather, unsigned depth, Evaluate const & evaluate);
int64_t replace (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn);
int64_t fainted (Team ai, Team foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather weather, unsigned depth, Evaluate const & evaluate);

void deorder (Team & first, Team & last, Team* & ai, Team* & foe);

int64_t initial_move_then_switch_branch(Team & switcher, Team const & other, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn = false);
int64_t move_then_switch_branch (Team & switcher, Team const & other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & user_variable, Variable const & other_variable, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn = false);
int64_t switch_after_move_branch (Team switcher, Team other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & user_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate);

int get_awaken_numerator (Pokemon const & pokemon);

Moves random_action (Team const & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine);
bool is_replacing (Team const & team);
Moves random_switch (Team const & ai, std::mt19937 & random_engine);
std::vector<Moves> all_switches (uint8_t team_size, uint8_t pokemon_index);
Moves random_move_or_switch (Team const & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine);

void print_best_move (Team const & team, Moves best_move, int64_t score);
void print_action (Team const & team, bool first_turn);
void print_estimated_score (bool first_turn, bool is_me, int64_t estimate);
void update_best_move (int64_t & alpha, int64_t beta, bool first_turn, Moves new_move, Moves & best_move);
void update_foe_best_move (Team & foe, MoveScores & foe_scores, int64_t & beta, int64_t max_score, bool first_turn);

}	// unnamed namespace

Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, unsigned depth, Evaluate const & evaluate, std::mt19937 & random_engine) {
	std::cout << std::string (20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	MoveScores ai_scores;
	MoveScores foe_scores;
	int64_t min_score = 0;
	boost::timer timer;	
	Moves best_move = random_action (ai, foe, weather, random_engine);
	try {
		for (unsigned deeper = 1; deeper <= depth; ++deeper) {
			bool const full_evaluation = (deeper == depth);
			min_score = select_type_of_move_branch (ai, foe, ai_scores, foe_scores, weather, deeper, evaluate, best_move, full_evaluation);
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

int64_t select_type_of_move_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	ai_scores.update(ai.pokemon());
	foe_scores.update(foe.pokemon());
	if (depth > 0)
		--depth;
	
	calculate_speed (ai, weather);
	calculate_speed (foe, weather);

	if (ai.pokemon().stat(Stat::HP).stat == 0 or foe.pokemon().stat(Stat::HP).stat == 0)
		return replace (ai, foe, ai_scores, foe_scores, weather, depth, evaluate, best_move, first_turn);
	else if (ai.pokemon().switch_decision_required())
		return initial_move_then_switch_branch(ai, foe, ai_scores, foe_scores, weather, depth, evaluate, best_move, first_turn);
	else if (foe.pokemon().switch_decision_required()) {
		assert(false);
		return initial_move_then_switch_branch(foe, ai, ai_scores, foe_scores, weather, depth, evaluate, best_move, first_turn);
	}
	else
		return select_move_branch (ai, foe, ai_scores, foe_scores, weather, depth, evaluate, best_move, first_turn);
}

namespace {

int64_t select_move_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	// Working from the inside loop out:

	// The following begins by setting beta to the largest possible value. This
	// is the variable that the opposing player is trying to minimize. As long
	// as the opposing player has any move that won't guarantee their loss,
	// that move will score lower (more negative) than Evaluate::victory, and thus
	// the opponent will set that as their best response to the particular move
	// that the AI uses.

	// After looking at each response the opponent has to a given move, beta is
	// finally set to whatever the score will be if the AI uses that move.
	// alpha is initially set to the lowest possible value, so as long as the
	// AI has any move that won't guarantee its loss, that move will score
	// higher (more positive) than -Evaluate::victory, and thus the AI will set
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
	
	// I subtract 1 from -Evaluate::victory to make sure that even a guaranteed
	// loss is seen as better than not returning a result. This way, I can do
	// some things when my intermediate scores are strictly greater than alpha,
	// rather than greater than or equal to, which can save a few calculations.
	// This has the side-effect of limiting Evaluate::victory to be at least one
	// less than the greatest value representable by an int64_t, which in
	// practice shouldn't matter.
	
	// For a similar reason, I later set beta to Evaluate::victory + 1.
	
	// This change also has the advantage of making sure a move is always put
	// into best_move without any additional logic, such as pre-filling it with
	// some result.
	
	auto const ai_index = reorder(LegalSelections(ai.pokemon(), foe.pokemon(), weather), ai_scores, true);
	auto const foe_index = reorder(LegalSelections(foe.pokemon(), ai.pokemon(), weather), foe_scores, false);

	// Iterate through each move each Pokemon has in combination with each
	// move the other Pokemon has, and evaluate the score of each
	// combination.
	int64_t alpha = -Evaluate::victory - 1;
	for (RankedMove const & ai_move : ai_index) {
		ai.pokemon().all_moves().set_index(ai_move.name());
		print_action (ai, first_turn);
		int64_t beta = Evaluate::victory + 1;
		for (RankedMove const & foe_move : foe_index) {
			foe.pokemon().all_moves().set_index(foe_move.name());
			print_action (foe, first_turn);
			int64_t const max_score = order_branch (ai, foe, ai_scores, foe_scores, weather, depth, evaluate);
			update_foe_best_move (foe, foe_scores, beta, max_score, first_turn);
			// Alpha-Beta pruning
			if (beta <= alpha)
				break;
		}
		ai_scores.at(ai.pokemon().name(), ai.pokemon().move().name) = beta;
		update_best_move (alpha, beta, first_turn, ai.pokemon().move().name, best_move);
		// The AI cannot have a better move than a guaranteed win
		if (alpha == Evaluate::victory)
			break;
	}
	return alpha;
}

void update_best_move (int64_t & alpha, int64_t beta, bool first_turn, Moves new_move, Moves & best_move) {
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

void update_foe_best_move (Team & foe, MoveScores & foe_scores, int64_t & beta, int64_t const max_score, bool const first_turn) {
	if (beta > max_score) {
		beta = max_score;
		foe_scores.at(foe.pokemon().name(), foe.pokemon().move().name) = beta;
	}
	constexpr bool is_me = false;
	print_estimated_score (first_turn, is_me, max_score);
}

int64_t order_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	// Determine turn order
	Team* first;
	Team* last;
	order (ai, foe, weather, first, last); 
	bool const speed_tie = (first == nullptr);
	return speed_tie ?
		(accuracy_branch (ai, foe, ai_scores, foe_scores, weather, depth, evaluate) + accuracy_branch (foe, ai, ai_scores, foe_scores, weather, depth, evaluate)) / 2 :
		accuracy_branch (*first, *last, ai_scores, foe_scores, weather, depth, evaluate);
}

template<typename FlagPossible, typename SetFlag, typename Probability, typename NextBranch>
int64_t generic_flag_branch(Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, FlagPossible const & flag_possible, SetFlag const & set_flag, Probability const & probability, NextBranch const & next_branch) {
	int64_t average_score = 0;
	for (auto const first_flag : { true, false }) {
		if (first_flag and !flag_possible(first.pokemon())) {
			continue;
		}
		set_flag(first.pokemon(), first_flag);
		for (auto const last_flag : { true, false }) {
			if (last_flag and !flag_possible(last.pokemon())) {
				continue;
			}
			set_flag(last.pokemon(), last_flag);
			auto const p = probability(first.pokemon()) * probability(last.pokemon());
			average_score += next_branch(first, last, ai_scores, foe_scores, weather, depth, evaluate) * p;
		}
	}
	return average_score;
}

int64_t accuracy_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	constexpr int divisor = 100 * 100;
	constexpr bool first_moved = true;
	constexpr bool last_moved = false;
	first.update_chance_to_hit(last, weather, last_moved);
	last.update_chance_to_hit(first, weather, first_moved);
	auto const flag_possible = [](ActivePokemon const & pokemon) {
		return pokemon.can_miss();
	};
	auto const set_flag = [](ActivePokemon & pokemon, bool const flag) {
		pokemon.set_miss(flag);
	};
	auto const probability = [](ActivePokemon const & pokemon) {
		return pokemon.accuracy_probability();
	};
	return generic_flag_branch(first, last, ai_scores, foe_scores, weather, depth, evaluate, flag_possible, set_flag, probability, awaken_branch) / divisor;
}

int64_t awaken_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	auto const flag_possible = [](ActivePokemon const & pokemon) {
		return pokemon.can_awaken();
	};
	auto const set_flag = [](ActivePokemon & pokemon, bool const flag) {
		pokemon.awaken(flag);
	};
	auto const probability = [](ActivePokemon const & pokemon) {
		return pokemon.awaken_probability();
	};
	return generic_flag_branch(first, last, ai_scores, foe_scores, weather, depth, evaluate, flag_possible, set_flag, probability, random_move_effects_branch);
}


int64_t random_move_effects_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	auto const flag_possible = [](ActivePokemon const & pokemon) {
		return pokemon.move().can_critical_hit();
	};
	auto const set_flag = [](ActivePokemon & pokemon, bool const flag) {
		pokemon.set_critical_hit(flag);
	};
	auto const probability = [](ActivePokemon const & pokemon) {
		return pokemon.critical_probability();
	};
	int64_t score3 = 0;

	for (auto const & first_variable : all_probabilities(first.pokemon(), last.size())) {
		int64_t score2 = 0;
		for (auto const & last_variable : all_probabilities(last.pokemon(), first.size())) {
			auto const use_move_copy_branch = [&](Team first_, Team last_, MoveScores & ai_scores_, MoveScores & foe_scores_, Weather weather_, unsigned depth_, Evaluate const & evaluate_) {
				return use_move_branch(first_, last_, ai_scores_, foe_scores_, first_variable, last_variable, weather_, depth_, evaluate_);
			};
			auto const score1 = generic_flag_branch(first, last, ai_scores, foe_scores, weather, depth, evaluate, flag_possible, set_flag, probability, use_move_copy_branch);
			score2 += score1 * last_variable.probability();
		}
		score3 += score2 * first_variable.probability();
	}
	return score3;
}


int64_t use_move_branch (Team & first, Team & last, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & first_variable, Variable const & last_variable, Weather & weather, unsigned depth, Evaluate const & evaluate) {
	int64_t value = use_move_and_follow_up (first, last, ai_scores, foe_scores, first_variable, last_variable, weather, depth, evaluate);
	if (value != Evaluate::victory + 1)	// illegal value
		return value;
	// If first uses a phazing move before last gets a chance to move, the
	// newly brought out Pokemon would try to move without checking to see if
	// it has already moved. This check is also necessary for my Baton Pass and
	// U-turn implementation to function.
	value = use_move_and_follow_up (last, first, ai_scores, foe_scores, last_variable, first_variable, weather, depth, evaluate);
	if (value != Evaluate::victory + 1)
		return value;

	// Find the expected return on all possible outcomes at the end of the turn
	
	// Need to recalculate speed because end-of-turn effects occur in a
	// specified order based on Speed, and a that order can be changed within a
	// turn.
	calculate_speed (first, weather);
	calculate_speed (last, weather);
	Team * faster;
	Team * slower;
	faster_pokemon (first, last, weather, faster, slower);
	auto const flag_possible = [](ActivePokemon const & pokemon) {
		return pokemon.can_clear_status();
	};
	auto const set_flag = [](ActivePokemon & pokemon, bool const flag) {
		pokemon.shed_skin(flag);
	};
	auto const probability = [](ActivePokemon const & pokemon) {
		return pokemon.shed_skin_probability();
	};
	// Partially apply some arguments to the function so it has the expected
	// signature
	auto const end_of_turn_order = [faster, slower](Team & team, Team & other, MoveScores & ai_scores_, MoveScores & foe_scores_, Weather const & weather_, unsigned depth_, Evaluate const & evaluate_) {
		return end_of_turn_order_branch(team, other, ai_scores_, foe_scores_, faster, slower, weather_, depth_, evaluate_);
	};
	return generic_flag_branch(first, last, ai_scores, foe_scores, weather, depth, evaluate, flag_possible, set_flag, probability, end_of_turn_order);
}

int64_t use_move_and_follow_up (Team & user, Team & other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & user_variable, Variable const & other_variable, Weather & weather, unsigned depth, Evaluate const & evaluate) {
	if (!user.pokemon().moved()) {
		unsigned const damage = call_move(user, other, weather, user_variable);
		other.pokemon().direct_damage(damage);
		int64_t const user_win = Evaluate::win (user);
		int64_t const other_win = Evaluate::win (other);
		if (user_win or other_win)
			return user_win + other_win;
		if (user.pokemon().move().has_follow_up_decision() and user.all_pokemon().size() > 1) {
			Moves phony = Moves::END;
			return move_then_switch_branch(user, other, ai_scores, foe_scores, user_variable, other_variable, weather, depth, evaluate, phony);
		}
	}
	return Evaluate::victory + 1;		// return an illegal value
}

int64_t end_of_turn_order_branch (Team & team, Team & other, MoveScores & ai_scores, MoveScores & foe_scores, Team * first, Team * last, Weather const & weather, unsigned depth, Evaluate const & evaluate) {
	bool const speed_tie = (first == nullptr);
	return speed_tie ?
		(end_of_turn_branch (team, other, ai_scores, foe_scores, weather, depth, evaluate) + end_of_turn_branch (other, team, ai_scores, foe_scores, weather, depth, evaluate)) / 2 :
		end_of_turn_branch (*first, *last, ai_scores, foe_scores, weather, depth, evaluate);
}

int64_t end_of_turn_branch (Team first, Team last, MoveScores & ai_scores, MoveScores & foe_scores, Weather weather, unsigned depth, Evaluate const & evaluate) {
	endofturn (first, last, weather);

	int64_t const last_violated = Evaluate::sleep_clause (first);
	int64_t const first_violated = Evaluate::sleep_clause (last);
	if (last_violated or first_violated)
		return last_violated + first_violated;

	int64_t const first_win = Evaluate::win (first);
	int64_t const last_win = Evaluate::win (last);
	if (first_win or last_win)
		return first_win + last_win;

	Team* ai;
	Team* foe;
	deorder (first, last, ai, foe);
	return transposition (*ai, *foe, ai_scores, foe_scores, weather, depth, evaluate);
}



int64_t replace (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn) {
	Team * first;
	Team * last;
	faster_pokemon (ai, foe, weather, first, last);
	bool const speed_tie = (first == nullptr);
	unsigned const tabs = first_turn ? 0 : 2;
	int64_t alpha = -Evaluate::victory - 1;
	auto const ai_break_out = [& ai]() { return ai.pokemon().stat(Stat::HP).stat != 0; };
	ai.all_pokemon().for_each_replacement(ai_break_out, [&]() {
		if (verbose or first_turn)
			std::cout << std::string(tabs, '\t') + "Evaluating switching to " + ai.all_pokemon().at_replacement().to_string() + "\n";
		int64_t beta = Evaluate::victory + 1;
		auto const foe_break_out = [& foe, & alpha, & beta]() {
			return beta <= alpha or foe.pokemon().stat(Stat::HP).stat != 0;
		};
		foe.all_pokemon().for_each_replacement(foe_break_out, [&]() {
			beta = (speed_tie) ?
				std::min (beta, (fainted (ai, foe, ai_scores, foe_scores, weather, depth, evaluate) + fainted (foe, ai, ai_scores, foe_scores, weather, depth, evaluate)) / 2) :
				std::min (beta, fainted (*first, *last, ai_scores, foe_scores, weather, depth, evaluate));
		});
		update_best_move (alpha, beta, first_turn, ai.all_pokemon().replacement_to_switch(), best_move);
	});
	return alpha;
}

int64_t fainted (Team first, Team last, MoveScores & ai_scores, MoveScores & foe_scores, Weather weather, unsigned depth, Evaluate const & evaluate) {
	// Use pokemon() instead of at_replacement() because it checks whether the
	// current Pokemon needs to be replaced because it fainted.
	if (first.pokemon().stat(Stat::HP).stat == 0) {
		switchpokemon (first, last, weather);
		if (Evaluate::win (first) != 0 or Evaluate::win (last) != 0)
			return Evaluate::win (first) + Evaluate::win (last);
	}
	if (last.pokemon().stat(Stat::HP).stat == 0) {
		switchpokemon (last, first, weather);
		if (Evaluate::win (first) != 0 or Evaluate::win (last) != 0)
			return Evaluate::win (first) + Evaluate::win (last);
	}

	Team* ai;
	Team* foe;
	deorder (first, last, ai, foe);
	return (depth == 0) ?
		evaluate(*ai, *foe, weather) :
		transposition (*ai, *foe, ai_scores, foe_scores, weather, depth, evaluate);
}

int64_t initial_move_then_switch_branch(Team & switcher, Team const & other, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn) {
	return move_then_switch_branch(switcher, other, ai_scores, foe_scores, Variable(), Variable(), weather, depth, evaluate, best_switch, first_turn);
}

int64_t move_then_switch_branch(Team & switcher, Team const & other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & switcher_variable, Variable const & other_variable, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_switch, bool first_turn) {
	unsigned tabs = first_turn ? 0 : 2;
	int64_t alpha = -Evaluate::victory - 1;
	if (!switcher.is_me()) {
		alpha = -alpha;
		++tabs;
	}
	switcher.all_pokemon().for_each_replacement([&]() {
		if (first_turn)
			std::cout << std::string (tabs, '\t') + "Evaluating bringing in " + switcher.all_pokemon().at_replacement ().to_string () + "\n";
		int64_t const value = switch_after_move_branch(switcher, other, ai_scores, foe_scores, switcher_variable, other_variable, weather, depth, evaluate);
		if (switcher.is_me())
			update_best_move (alpha, value, first_turn, switcher.all_pokemon().replacement_to_switch(), best_switch);
		else
			update_foe_best_move (switcher, foe_scores, alpha, value, first_turn);
	});
	return alpha;
}

int64_t switch_after_move_branch(Team switcher, Team other, MoveScores & ai_scores, MoveScores & foe_scores, Variable const & switcher_variable, Variable const & other_variable, Weather weather, unsigned depth, Evaluate const & evaluate) {
	switchpokemon (switcher, other, weather);
	assert(!switcher.all_pokemon().is_empty());
	assert(!other.all_pokemon().is_empty());
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

	return use_move_branch(switcher, other, ai_scores, foe_scores, switcher_variable, other_variable, weather, depth, evaluate);
}



void deorder (Team & first, Team & last, Team* & ai, Team* & foe) {
	assert(first.is_me() or last.is_me());
	ai = (first.is_me()) ? & first : & last;
	foe = (!first.is_me()) ? & first : & last;
}


Moves random_action (Team const & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine) {
	return ai.pokemon().switch_decision_required() ?
			random_switch(ai, random_engine) :
			random_move_or_switch(ai, foe, weather, random_engine);
}

Moves random_switch (Team const & ai, std::mt19937 & random_engine) {
	std::vector<Moves> const switches = all_switches (ai.all_pokemon().size(), ai.pokemon().index());
	std::uniform_int_distribution<size_t> distribution { 0, switches.size() - 1 };
	size_t const index = distribution (random_engine);
	return switches [index];
}

std::vector<Moves> all_switches (uint8_t const team_size, uint8_t const pokemon_index) {
	std::vector<Moves> switches;
	for (unsigned n = 0; n != team_size; ++n) {
		if (n != pokemon_index)
			switches.emplace_back(Move::from_replacement(n));
	}
	return switches;
}

Moves random_move_or_switch (Team const & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine) {
	LegalSelections const moves(ai.pokemon(), foe.pokemon(), weather);
	std::uniform_int_distribution<size_t> distribution { 0, moves.size() - 1 };
	auto const index = distribution(random_engine);
	return moves[index]->name;
}

void print_best_move (Team const & team, Moves const best_move, int64_t score) {
	if (Move::is_switch (best_move))
		std::cout << "Switch to " + team.pokemon(Move::to_replacement(best_move)).to_string();
	else
		std::cout << "Use " + to_string(best_move);
	std::cout << " for a minimum expected score of " << score << "\n";
}

void print_action (Team const & team, bool first_turn) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!team.is_me())
			++tabs;
		std::cout << std::string (tabs, '\t') + "Evaluating ";
		if (team.pokemon().move().is_switch())
			std::cout << "switching to " + team.pokemon(team.pokemon().move().to_replacement()).to_string () + "\n";
		else
			std::cout << team.pokemon().move().to_string() + "\n";
	}
}

void print_estimated_score (bool const first_turn, bool const is_me, int64_t const estimate) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!is_me)
			++tabs;
		std::cout << std::string (tabs, '\t') + "Estimated score is " << estimate << '\n';
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
