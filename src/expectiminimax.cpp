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

#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "ability.hpp"
#include "block.hpp"
#include "endofturn.hpp"
#include "evaluate.hpp"
#include "move.hpp"
#include "reorder_moves.hpp"
#include "pokemon.hpp"
#include "stat.hpp"
#include "switch.hpp"
#include "team.hpp"
#include "transposition.hpp"
#include "type.hpp"
#include "use_move.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

// This prints out search equal to the maximum depth normally, but any
// deeper searches will also print out with a single tab. This is not
// recommended for depth greater than 2.
constexpr bool verbose = false;

int64_t select_move_branch (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, Move::Moves & best_move, bool first_turn);
int64_t order_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score);
int64_t accuracy_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
int64_t random_move_effects_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
int64_t awaken_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
int64_t use_move_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score);
int64_t use_move_and_follow_up (Team & user, Team & other, Weather & weather, unsigned depth, Score const & score);
int64_t end_of_turn_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score);
int64_t end_of_turn_order_branch (Team & team, Team & other, Team * first, Team * last, Weather const & weather, unsigned depth, Score const & score);
int64_t replace (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn);
int64_t fainted (Team ai, Team foe, Weather weather, unsigned depth, Score const & score);
void deorder (Team & first, Team & last, Team* & ai, Team* & foe);
int64_t move_then_switch_branch (Team & switcher, Team const & other, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_switch, bool first_turn = false);
int64_t switch_after_move_branch (Team switcher, Team other, Weather weather, unsigned depth, Score const & score);
int get_awaken_numerator (Pokemon const & pokemon);

Move::Moves random_action (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine);
bool is_replacing (Team const & team);
Move::Moves random_switch (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine);
Move::Moves random_move_or_switch (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine);

void print_best_move (Team const & team, Move::Moves best_move, unsigned depth, int64_t score);
void print_action (Team const & team, bool first_turn);
void print_estimated_score (bool first_turn, bool is_me, int64_t estimate);
void update_best_move (Team & ai, int64_t & alpha, int64_t beta, bool first_turn, Move::Moves new_move, Move::Moves & best_move);
void update_foe_best_move (Team & foe, int64_t & beta, int64_t max_score, bool first_turn);

}	// unnamed namespace

Move::Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, std::mt19937 & random_engine) {
	std::cout << std::string (20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	// Set the score of all foe moves to an illegally high value, so that they get sorted last. If they didn't even need to be checked for their complete value before, they probably still don't need to be.
	for (Pokemon & pokemon : foe.pokemon.set) {
		for (Move & move : pokemon.move.set)
			move.score = Score::VICTORY + 1;
	}
	int64_t min_score = 0;
	
	Move::Moves best_move = random_action (ai, foe, weather, random_engine);
	try {
		for (unsigned deeper = 1; deeper <= depth; ++deeper) {
			bool const first_turn = (deeper == depth);
			min_score = select_type_of_move_branch (ai, foe, weather, deeper, score, best_move, first_turn);
		}
	}
	catch (InvalidActiveIndex const & ex) {
		// If there was an error, I just use the data from the deepest search I
		// could do that did not encounter that error.
		std::cerr << std::string (20, '=') + '\n';
		std::cerr << ex.what ();
		std::cerr << std::string (20, '=') + '\n';
	}

	print_best_move (ai, best_move, depth, min_score);
	return best_move;
}

int64_t select_type_of_move_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn) {

	// Working from the inside loop out:

	// The following begins by setting beta to the largest possible value. This
	// is the variable that the opposing player is trying to minimize. As long
	// as the opposing player has any move that won't guarantee their loss,
	// that move will score lower (more negative) than Score::VICTORY, and thus
	// the opponent will set that as their best response to the particular move
	// that the AI uses.

	// After looking at each response the opponent has to a given move, beta is
	// finally set to whatever the score will be if the AI uses that move.
	// alpha is initially set to the lowest possible value, so as long as the
	// AI has any move that won't guarantee its loss, that move will score
	// higher (more positive) than -Score::VICTORY, and thus the AI will set
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
	
	// I subtract 1 from -Score::VICTORY to make sure that even a guaranteed
	// loss is seen as better than not returning a result. This way, I can do
	// some things when my intermediate scores are strictly greater than alpha,
	// rather than greater than or equal to, which can save a few calculations.
	// This has the side-effect of limiting Score::VICTORY to be at least one
	// less than the greatest value representable by an int64_t, which in
	// practice shouldn't matter.
	
	// For a similar reason, I later set beta to Score::VICTORY + 1.
	
	// This change also has the advantage of making sure a move is always put
	// into best_move without any additional logic, such as pre-filling it with
	// some result.
	
	if (depth > 0)
		--depth;
	
	calculate_speed (ai, weather);
	calculate_speed (foe, weather);

	if (ai.pokemon().hp.stat == 0 or foe.pokemon().hp.stat == 0)
		return replace (ai, foe, weather, depth, score, best_move, first_turn);
	else if (ai.pass or ai.u_turning)
		return move_then_switch_branch (ai, foe, weather, depth, score, best_move, first_turn);
	else if (foe.pass or foe.u_turning)
		return move_then_switch_branch (foe, ai, weather, depth, score, best_move, first_turn);
	else
		return select_move_branch (ai, foe, weather, depth, score, best_move, first_turn);
}

namespace {

int64_t select_move_branch (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, Move::Moves & best_move, bool first_turn) {
	int64_t alpha = -Score::VICTORY - 1;

	std::vector <std::pair <int64_t, size_t>> const ai_index = reorder (ai.pokemon().move.set, true);
	std::vector <std::pair <int64_t, size_t>> const foe_index = reorder (foe.pokemon().move.set, false);
	
	// Determine which moves can be legally selected
	for (ai.pokemon().move.index = 0; ai.pokemon().move.index != ai.pokemon().move.set.size(); ++ai.pokemon().move.index)
		ai.pokemon().move().selectable = is_legal_selection (ai, foe, weather);
	for (foe.pokemon().move.index = 0; foe.pokemon().move.index != foe.pokemon().move.set.size(); ++foe.pokemon().move.index)
		foe.pokemon().move().selectable = is_legal_selection (foe, ai, weather);

	// Iterate through each move each Pokemon has in combination with each
	// move the other Pokemon has, and evaluate the score of each
	// combination.
	for (std::pair <int64_t, size_t> const & ai_move : ai_index) {
		ai.pokemon().move.index = ai_move.second;
		if (!ai.pokemon().move().selectable)
			continue;
		print_action (ai, first_turn);
		int64_t beta = Score::VICTORY + 1;
		for (std::pair <int64_t, size_t> const & foe_move : foe_index) {
			foe.pokemon().move.index = foe_move.second;
			if (!foe.pokemon().move().selectable)
				continue;
			print_action (foe, first_turn);
			int64_t const max_score = order_branch (ai, foe, weather, depth, score);
			update_foe_best_move (foe, beta, max_score, first_turn);
			// Alpha-Beta pruning
			if (beta <= alpha)
				break;
		}
		update_best_move (ai, alpha, beta, first_turn, ai.pokemon().move().name, best_move);
		// The AI cannot have a better move than a guaranteed win
		if (alpha == Score::VICTORY)
			break;
	}
	return alpha;
}

void update_best_move (Team & ai, int64_t & alpha, int64_t beta, bool first_turn, Move::Moves new_move, Move::Moves & best_move) {
	// If their best response isn't as good as their previous best
	// response, then this new move must be better than the
	// previous AI's best move
	if (beta > alpha) {
		alpha = beta;
		ai.at_replacement().move().score = alpha;
		best_move = new_move;
		constexpr bool is_me = true;
		print_estimated_score (first_turn, is_me, alpha);
	}
}

void update_foe_best_move (Team & foe, int64_t & beta, int64_t const max_score, bool const first_turn) {
	if (beta > max_score) {
		beta = max_score;
		foe.pokemon().move().score = beta;
	}
	constexpr bool is_me = false;
	print_estimated_score (first_turn, is_me, max_score);
}

int64_t order_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score) {
	// Determine turn order
	Team* first;
	Team* last;
	order (ai, foe, weather, first, last); 
	bool const speed_tie = (first == nullptr);
	return speed_tie ?
		(accuracy_branch (ai, foe, weather, depth, score) + accuracy_branch (foe, ai, weather, depth, score)) / 2 :
		accuracy_branch (*first, *last, weather, depth, score);
}


int64_t accuracy_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score) {
	constexpr int divisor = 100 * 100;
	chance_to_hit (first, last, weather);
	first.moved = true;
	chance_to_hit (last, first, weather);
	first.moved = false;
	int64_t average_score = first.chance_to_hit * last.chance_to_hit * random_move_effects_branch (first, last, weather, depth, score);
	if (first.chance_to_hit != 100) {
		first.miss = true;
		average_score += (100 - first.chance_to_hit) * last.chance_to_hit * random_move_effects_branch (first, last, weather, depth, score);
		if (last.chance_to_hit != 100) {
			last.miss = true;
			average_score += (100 - first.chance_to_hit) * (100 - last.chance_to_hit) * random_move_effects_branch (first, last, weather, depth, score);
			last.miss = false;
		}
		first.miss = false;
	}
	if (last.chance_to_hit != 100) {
		last.miss = true;
		average_score += first.chance_to_hit * (100 - last.chance_to_hit) * random_move_effects_branch (first, last, weather, depth, score);
		last.miss = false;
	}
	average_score /= divisor;
	return average_score;
}


int64_t random_move_effects_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score) {
	int64_t score3 = 0;
	for (first.pokemon().move().variable.index = 0; first.pokemon().move().variable.index != first.pokemon().move().variable.set.size(); ++first.pokemon().move().variable.index) {
		if ((!first.pokemon().move().is_phaze ()) or first.pokemon().move().variable().first != last.pokemon.index or last.pokemon.set.size() == 1) {
			int64_t score2 = 0;
			for (last.pokemon().move().variable.index = 0; last.pokemon().move().variable.index != last.pokemon().move().variable.set.size(); ++last.pokemon().move().variable.index) {
				if ((!last.pokemon().move().is_phaze ()) or last.pokemon().move().variable().first != first.pokemon.index or first.pokemon.set.size() == 1) {
					first.ch = false;
					last.ch = false;
					int64_t score1 = awaken_branch (first, last, weather, depth, score);
					if (first.pokemon().move().basepower > 0 and last.pokemon().move().basepower <= 0) {
						score1 *= 15;
						first.ch = true;
						score1 += awaken_branch (first, last, weather, depth, score);
						score1 /= 16;
					}
					else if (first.pokemon().move().basepower <= 0 and last.pokemon().move().basepower > 0) {
						score1 *= 15;
						last.ch = true;
						score1 += awaken_branch (first, last, weather, depth, score);
						score1 /= 16;
					}
					else if (first.pokemon().move().basepower > 0 and last.pokemon().move().basepower > 0) {
						score1 *= 225;
						first.ch = true;
						score1 += awaken_branch (first, last, weather, depth, score) * 15;
						last.ch = true;
						score1 += awaken_branch (first, last, weather, depth, score);
						first.ch = false;
						score1 += awaken_branch (first, last, weather, depth, score) * 15;
						score1 /= 256;
					}
					score2 += score1 * last.pokemon().move().variable().second;
				}
			}
			score3 += score2 * first.pokemon().move().variable().second / Move::max_probability;
		}
	}
	return score3 / Move::max_probability;
}


int64_t awaken_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score) {
	int const first_numerator = get_awaken_numerator (first.pokemon());
	int const last_numerator = get_awaken_numerator (last.pokemon());

	first.awaken = false;
	last.awaken = false;
	int64_t average_score = use_move_branch (first, last, weather, depth, score);
	if (first_numerator > 1) {
		average_score *= 4 - first_numerator;
		first.awaken = true;
		average_score += first_numerator * use_move_branch (first, last, weather, depth, score);
		if (last_numerator > 1) {
			average_score *= 4 - last_numerator;
			last.awaken = true;
			average_score += last_numerator * (4 - first_numerator) * use_move_branch (first, last, weather, depth, score);
			first.awaken = false;
			average_score += last_numerator * first_numerator * use_move_branch (first, last, weather, depth, score);
			average_score /= 4;
		}
		average_score /= 4;
	}
	else if (last_numerator > 1) {
		average_score *= 4 - last_numerator;
		last.awaken = true;
		average_score += last_numerator * use_move_branch (first, last, weather, depth, score);
		average_score /= 4;
	}
	return average_score;
}

int64_t use_move_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score) {
	int64_t value = use_move_and_follow_up (first, last, weather, depth, score);
	if (value != Score::VICTORY + 1)	// illegal value
		return value;
	// If first uses a phazing move before last gets a chance to move, the
	// newly brought out Pokemon would try to move without checking to see if
	// it has already moved. This check is also necessary for my Baton Pass and
	// U-turn implementation to function.
	value = use_move_and_follow_up (last, first, weather, depth, score);
	if (value != Score::VICTORY + 1)
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
	first.shed_skin = false;
	last.shed_skin = false;
	int64_t average_score = 49 * end_of_turn_order_branch (first, last, faster, slower, weather, depth, score);
	int64_t divisor = 49;
	if (first.pokemon().ability.name == Ability::SHED_SKIN and first.pokemon().status.name != Status::NO_STATUS) {
		first.shed_skin = true;
		average_score += 21 * end_of_turn_order_branch (first, last, faster, slower, weather, depth, score);
		divisor += 21;
		if (last.pokemon().ability.name == Ability::SHED_SKIN and last.pokemon().status.name != Status::NO_STATUS) {
			last.shed_skin = true;
			average_score += 9 * end_of_turn_order_branch (first, last, faster, slower, weather, depth, score);
			divisor += 9;
			first.shed_skin = false;
		}
	}
	if (last.pokemon().ability.name == Ability::SHED_SKIN and last.pokemon().status.name != Status::NO_STATUS) {
		last.shed_skin = true;
		average_score += 21 * end_of_turn_order_branch (first, last, faster, slower, weather, depth, score);
		divisor += 21;
	}
	return average_score / divisor;
}

int64_t use_move_and_follow_up (Team & user, Team & other, Weather & weather, unsigned depth, Score const & score) {
	if (!user.moved) {
		other.damage = call_move (user, other, weather);
		int64_t const user_win = Score::win (user);
		int64_t const other_win = Score::win (other);
		if (user_win or other_win)
			return user_win + other_win;
		switch (user.pokemon().move().name) {
			case Move::BATON_PASS:
			case Move::U_TURN:
				if (user.pokemon.set.size () > 1) {
					Move::Moves phony = Move::END;
					return move_then_switch_branch (user, other, weather, depth, score, phony);
				}
				break;
			default:
				break;
		}
	}
	return Score::VICTORY + 1;		// return an illegal value
}

int64_t end_of_turn_order_branch (Team & team, Team & other, Team * first, Team * last, Weather const & weather, unsigned depth, Score const & score) {
	bool const speed_tie = (first == nullptr);
	return speed_tie ?
		(end_of_turn_branch (team, other, weather, depth, score) + end_of_turn_branch (other, team, weather, depth, score)) / 2 :
		end_of_turn_branch (*first, *last, weather, depth, score);
}

int64_t end_of_turn_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score) {
	endofturn (first, last, weather);
	int64_t value;
	int64_t const first_win = Score::win (first);
	int64_t const last_win = Score::win (last);
	if (first_win or last_win)
		value = first_win + last_win;
	else {
		Team* ai;
		Team* foe;
		deorder (first, last, ai, foe);

		value = transposition (*ai, *foe, weather, depth, score);
	}
	return value;
}



int64_t replace (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn) {
	Team* first;
	Team* last;
	faster_pokemon (ai, foe, weather, first, last);
	unsigned const tabs = first_turn ? 0 : 2;
	int64_t alpha = -Score::VICTORY - 1;
	for (ai.replacement = 0; ai.replacement != ai.pokemon.set.size(); ++ai.replacement) {
		if (ai.at_replacement ().name == ai.pokemon().name and ai.pokemon.set.size() != 1)
			continue;
		if (verbose or first_turn)
			std::cout << std::string (tabs, '\t') + "Evaluating switching to " + ai.at_replacement().to_string() + "\n";
		int64_t beta = Score::VICTORY + 1;
		for (foe.replacement = 0; foe.replacement != foe.pokemon.set.size(); ++foe.replacement) {
			if (foe.at_replacement ().name == foe.pokemon().name and foe.pokemon.set.size() != 1)
				continue;
			bool const speed_tie = (first == nullptr);
			beta = (speed_tie) ?
				std::min (beta, (fainted (ai, foe, weather, depth, score) + fainted (foe, ai, weather, depth, score)) / 2) :
				std::min (beta, fainted (*first, *last, weather, depth, score));
			if (beta <= alpha or foe.pokemon().hp.stat != 0)
				break;
		}
		update_best_move (ai, alpha, beta, first_turn, Move::from_replacement (ai.replacement), best_move);
		if (ai.pokemon().hp.stat != 0)
			break;
	}
	return alpha;
}

int64_t fainted (Team first, Team last, Weather weather, unsigned depth, Score const & score) {
	// Use pokemon() instead of at_replacement() because it checks whether the
	// current Pokemon needs to be replaced because it fainted.
	if (first.pokemon().hp.stat == 0) {
		switchpokemon (first, last, weather);
		if (Score::win (first) != 0 or Score::win (last) != 0)
			return Score::win (first) + Score::win (last);
	}
	if (last.pokemon().hp.stat == 0) {
		switchpokemon (last, first, weather);
		if (Score::win (first) != 0 or Score::win (last) != 0)
			return Score::win (first) + Score::win (last);
	}

	Team* ai;
	Team* foe;
	deorder (first, last, ai, foe);
	return (depth == 0) ?
		score.evaluate (*ai, *foe, weather) :
		transposition (*ai, *foe, weather, depth, score);
}



int64_t move_then_switch_branch (Team & switcher, Team const & other, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_switch, bool first_turn) {
	unsigned tabs = first_turn ? 0 : 2;
	int64_t alpha = -Score::VICTORY - 1;
	if (!switcher.me) {
		alpha = -alpha;
		++tabs;
	}
	for (switcher.replacement = 0; switcher.replacement != switcher.pokemon.set.size(); ++switcher.replacement) {
		if (switcher.at_replacement ().name == switcher.pokemon().name)
			continue;
		if (first_turn)
			std::cout << std::string (tabs, '\t') + "Evaluating bringing in " + switcher.at_replacement ().to_string () + "\n";
		int64_t const value = switch_after_move_branch (switcher, other, weather, depth, score);
		if (switcher.me)
			update_best_move (switcher, alpha, value, first_turn, Move::from_replacement (switcher.replacement), best_switch);
		else
			update_foe_best_move (switcher, alpha, value, first_turn);
	}
	return alpha;
}

int64_t switch_after_move_branch (Team switcher, Team other, Weather weather, unsigned depth, Score const & score) {
	switchpokemon (switcher, other, weather);
	// I don't have to correct for which of the Pokemon moved first because
	// there are only two options:
	
	// Option 1: only the switcher has moved. Then it obviously went first and
	// I'm passing them in the proper order.
	
	// Option 2: Both Pokemon have moved. use_move_branch then recalculates
	// which Pokemon is faster to properly account for end-of-turn effects. In
	// this case, it doesn't matter what order I pass them.
	
	// I also do not need to worry about the game ending due to entry hazards,
	// because at the very least the Pokemon that used Baton Pass / U-turn is
	// still alive.

	return use_move_branch (switcher, other, weather, depth, score);
}



void deorder (Team & first, Team & last, Team* & ai, Team* & foe) {
	ai = (first.me) ? & first : & last;
	foe = (!first.me) ? & first : & last;
}

int get_awaken_numerator (Pokemon const & pokemon) {
	return pokemon.sleep + (pokemon.ability.name == Ability::EARLY_BIRD);
}



Move::Moves random_action (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine) {
	return is_replacing (ai) ? random_switch (ai, foe, weather, random_engine) : random_move_or_switch (ai, foe, weather, random_engine);
}

bool is_replacing (Team const & team) {
	return team.pokemon().hp.stat == 0 or team.pass or team.u_turning;
}

Move::Moves random_switch (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine) {
	ai.pokemon().move.index = 0;
	while (!ai.pokemon().move().is_switch())
		++ai.pokemon().move.index;

	std::vector <Move::Moves> switches;
	for (; ai.pokemon().move.index != ai.pokemon().move.set.size(); ++ai.pokemon().move.index) {
		if (!ai.is_switching_to_self ()) {
			switches.push_back (ai.pokemon().move().name);
		}
	}
	std::uniform_int_distribution <size_t> distribution { 0, switches.size() - 1 };
	size_t const index = distribution (random_engine);
	return switches [index];
}

Move::Moves random_move_or_switch (Team & ai, Team const & foe, Weather const & weather, std::mt19937 & random_engine) {
	std::vector <Move::Moves> moves;
	for (ai.pokemon().move.index = 0; ai.pokemon().move.index != ai.pokemon().move.set.size(); ++ai.pokemon().move.index) {
		ai.pokemon().move().selectable = is_legal_selection (ai, foe, weather);
		if (ai.pokemon().move().selectable) {
			moves.push_back (ai.pokemon().move().name);
		}
	}
	std::uniform_int_distribution <size_t> distribution { 0, moves.size() - 1 };
	size_t const index = distribution (random_engine);
	return moves [index];
}


void print_best_move (Team const & team, Move::Moves best_move, unsigned depth, int64_t score) {
	if (Move::is_switch (best_move))
		std::cout << "Switch to " + team.pokemon.set [Move::to_replacement (best_move)].to_string ();
	else
		std::cout << "Use " + Move::to_string (best_move);
	std::cout << " for a minimum expected score of " << score << "\n";
}

void print_action (Team const & team, bool first_turn) {
	if (verbose or first_turn) {
		unsigned tabs = first_turn ? 0 : 2;
		if (!team.me)
			++tabs;
		std::cout << std::string (tabs, '\t') + "Evaluating ";
		if (team.pokemon().move().is_switch())
			std::cout << "switching to " + team.pokemon.set [team.pokemon().move().to_replacement ()].to_string () + "\n";
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
