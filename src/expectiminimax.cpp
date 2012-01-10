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
#include "weather.hpp"

namespace technicalmachine {

static int64_t order_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score);
static int64_t accuracy_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
static int64_t random_move_effects_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
static int64_t awaken_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score);
static int64_t use_move_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score);
static int64_t end_of_turn_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score);
static int64_t end_of_turn_order_branch (Team & team, Team & other, Team * first, Team * last, Weather const & weather, unsigned depth, Score const & score);
static int64_t replace (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn, bool verbose);
static int64_t fainted (Team ai, Team foe, Weather weather, unsigned depth, Score const & score);
static void deorder (Team & first, Team & last, Team* & ai, Team* & foe);
static int64_t move_then_switch_branch (Team & switcher, Team const & other, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_switch, bool first_turn = false, bool verbose = false);
static int64_t switch_after_move_branch (Team switcher, Team other, Weather weather, unsigned depth, Score const & score);
static void print_best_move (Team const & team, Move::Moves best_move, unsigned depth, int64_t score);
static void print_action (Team const & team, bool verbose, bool first_turn, std::string indent);

Move::Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score) {
	std::cout << std::string (20, '=') + "\nEvaluating to a depth of " << depth << "...\n";
	// Set the score of all foe moves to an illegally high value, so that they get sorted last. If they didn't even need to be checked for their complete value before, they probably still don't need to be.
	for (Pokemon & pokemon : foe.pokemon.set) {
		for (Move & move : pokemon.move.set)
			move.score = Score::VICTORY + 1;
	}
	if (depth == 0)
		depth = 1;
	Move::Moves best_move;
	int64_t min_score;
	for (unsigned deeper = 1; deeper <= depth; ++deeper) {
		bool const first_turn = (deeper == depth);
		min_score = select_move_branch (ai, foe, weather, deeper, score, best_move, first_turn);
	}

	print_best_move (ai, best_move, depth, min_score);
	return best_move;
}


int64_t select_move_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn) {

	/* Working from the inside loop out:

	The following begins by setting beta to the largest possible value. This is the variable that the opposing player is trying to minimize. As long as the opposing player has any move that won't guarantee their loss, that move will score lower (more negative) than Score::VICTORY, and thus the opponent will set that as their best response to the particular move that the AI uses.

	After looking at each response the opponent has to a given move, beta is finally set to whatever the score will be if the AI uses that move. alpha is initially set to the lowest possible value, so as long as the AI has any move that won't guarantee its loss, that move will score higher (more positive) than -Score::VICTORY, and thus the AI will set that as its best response. It then replaces that move if it finds a move for which the opponent's best response is more positive than the first move found. In other words, it finds the move for the AI for which the foe's best response is the weakest.
	
	Something to consider as a potential speed up at the cost of some accuracy (but would allow a deeper, thus more accurate, search) would be to pick from all random numbers randomly, rather than seeing the outcome of all of them and averaging it. In other words, do several trials assuming a particular (but different for each trial) set of random numbers are selected, and then average that result. This would give massive reductions to the branching factor, and with a large enough number of trials should be close enough to the average to potentially speed up the program enough to justify the loss in accuracy.
	
	I subtract 1 from -Score::VICTORY to make sure that even a guaranteed loss is seen as better than not returning a result. This way, I can do some things when my intermediate scores are strictly greater than alpha, rather than greater than or equal to, which can save a few calculations. This has the side-effect of limiting Score::VICTORY to be at least one less than the greatest value representable by an int64_t, which in practice shouldn't matter.
	
	For a similar reason, I later set beta to Score::VICTORY + 1.
	
	This change also has the advantage of making sure a move is always put into best_move without any additional logic, such as pre-filling it with some result.
	*/
	int64_t alpha = -Score::VICTORY - 1;
	
	calculate_speed (ai, weather);
	calculate_speed (foe, weather);

	// This prints out search equal to the maximum depth normally, but any deeper searches will also print out with a single tab. This is not recommended for depth greater than 2.
	constexpr bool verbose = false;
	
	// This section is for replacing fainted Pokemon as well as Baton Pass and U-turn replacements.

	if (ai.pokemon().hp.stat == 0 or foe.pokemon().hp.stat == 0)
		alpha = replace (ai, foe, weather, depth, score, best_move, first_turn, verbose);
	else if (ai.pass or ai.u_turning)
		alpha = move_then_switch_branch (ai, foe, weather, depth, score, best_move, first_turn, verbose);
	else if (foe.pass or foe.u_turning)
		alpha = move_then_switch_branch (foe, ai, weather, depth, score, best_move, first_turn, verbose);

	// This section is for selecting a move, including switches that aren't replacing a fainted Pokemon.
	else {
		if (depth > 0)
			--depth;
		
		std::string indent = "";
		if (verbose and !first_turn)
			indent += "\t\t";

		std::vector <std::pair <int64_t, size_t> > ai_index;
		reorder (ai.pokemon().move.set, ai_index, true);
		std::vector <std::pair <int64_t, size_t> > foe_index;
		reorder (foe.pokemon().move.set, foe_index, false);
		
		// Determine which moves can be legally selected
		for (ai.pokemon().move.index = 0; ai.pokemon().move.index != ai.pokemon().move.set.size(); ++ai.pokemon().move.index)
			block_selection (ai, foe, weather);
		for (foe.pokemon().move.index = 0; foe.pokemon().move.index != foe.pokemon().move.set.size(); ++foe.pokemon().move.index)
			block_selection (foe, ai, weather);

		// Iterate through each move each Pokemon has in combination with each move the other Pokemon has, and evaluate the score of each combination.
		for (std::pair <int64_t, size_t> const & ai_move : ai_index) {
			ai.pokemon().move.index = ai_move.second;
			if (ai.pokemon().move().selectable) {
				print_action (ai, verbose, first_turn, indent);
				int64_t beta = Score::VICTORY + 1;
				for (std::pair <int64_t, size_t> const & foe_move : foe_index) {
					foe.pokemon().move.index = foe_move.second;
					if (foe.pokemon().move().selectable) {
						print_action (foe, verbose, first_turn, indent);
						int64_t max_score = order_branch (ai, foe, weather, depth, score);
						if (verbose or first_turn)
							std::cout << indent + "\tEstimated score is " << max_score << '\n';
						if (beta > max_score) {
							beta = max_score;
							foe.pokemon().move().score = beta;
						}
						if (beta <= alpha)	// Alpha-Beta pruning
							break;
					}
				}
				// If their best response isn't as good as their previous best response, then this new move must be better than the previous AI's best move
				if (beta > alpha) {
					alpha = beta;
					ai.pokemon().move().score = alpha;
					best_move = ai.pokemon().move().name;
					if (verbose or first_turn)
						std::cout << indent + "Estimated score is " << alpha << '\n';
				}
				if (alpha == Score::VICTORY)	// There is no way the AI has a better move than a guaranteed win
					break;
			}
		}
	}
	return alpha;
}


int64_t order_branch (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score) {
	// Determine turn order
	Team* first;
	Team* last;
	order (ai, foe, weather, first, last); 
	int64_t value;
	if (first == nullptr)		// If both Pokemon are the same speed and moves are the same priority
		value = (accuracy_branch (ai, foe, weather, depth, score) + accuracy_branch (foe, ai, weather, depth, score)) / 2;
	else
		value = accuracy_branch (*first, *last, weather, depth, score);
	return value;
}


int64_t accuracy_branch (Team & first, Team & last, Weather const & weather, unsigned depth, Score const & score) {
	int divisor = 100 * 100;
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
	int n;
	if (first.pokemon().ability.name == Ability::EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int first_numerator = first.pokemon().sleep + n - 1;
	if (last.pokemon().ability.name == Ability::EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int last_numerator = last.pokemon().sleep + n - 1;
	int64_t average_score;
	first.awaken = false;
	last.awaken = false;
	average_score = use_move_branch (first, last, weather, depth, score);
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
	if (!first.moved) {
		Move::Moves move = first.pokemon().move().name;
		last.damage = usemove (first, last, weather);
		if (Score::win (first) != 0 or Score::win (last) != 0)
			return Score::win (first) + Score::win (last);
		switch (move) {
			case Move::BATON_PASS:
			case Move::U_TURN:
				if (first.pokemon.set.size () > 1) {
					Move::Moves phony = Move::END;
					return move_then_switch_branch (first, last, weather, depth, score, phony);
				}
				break;
			default:
				break;
		}
	}
	// If first uses a phazing move before last gets a chance to move, the newly brought out Pokemon would try to move without checking to see if it's already moved. This check is also necessary for my Baton Pass and U-turn implementation to function.
	if (!last.moved) {
		Move::Moves move = last.pokemon().move().name;
		first.damage = usemove (last, first, weather);
		if (Score::win (first) != 0 or Score::win (last) != 0)
			return Score::win (first) + Score::win (last);
		switch (move) {
			case Move::BATON_PASS:
			case Move::U_TURN:
				if (last.pokemon.set.size () > 1) {
					Move::Moves phony = Move::END;
					return move_then_switch_branch (last, first, weather, depth, score, phony);
				}
				break;
			default:
				break;
		}
	}

	// Find the expected return on all possible outcomes at the end of the turn
	
	// Need to recalculate speed because end-of-turn effects occur in a specified order based on Speed, and a that order can be changed within a turn.
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

int64_t end_of_turn_order_branch (Team & team, Team & other, Team * first, Team * last, Weather const & weather, unsigned depth, Score const & score) {
	int64_t value;
	if (first == nullptr)		// If both Pokemon are the same speed
		value = (end_of_turn_branch (team, other, weather, depth, score) + end_of_turn_branch (other, team, weather, depth, score)) / 2;
	else
		value = end_of_turn_branch (*first, *last, weather, depth, score);
	return value;
}

int64_t end_of_turn_branch (Team first, Team last, Weather weather, unsigned depth, Score const & score) {
	endofturn (first, last, weather);
	int64_t value;
	if (Score::win (first) != 0 or Score::win (last) != 0)
		value = Score::win (first) + Score::win (last);
	else {
		Team* ai;
		Team* foe;
		deorder (first, last, ai, foe);

		value = transposition (*ai, *foe, weather, depth, score);
	}
	return value;
}

int64_t replace (Team & ai, Team & foe, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_move, bool first_turn, bool verbose) {
	Team* first;
	Team* last;
	faster_pokemon (ai, foe, weather, first, last);
	std::string indent = "";
	if (verbose and !first_turn)
		indent += "\t\t";
	int64_t alpha = -Score::VICTORY - 1;
	for (ai.replacement = 0; ai.replacement != ai.pokemon.set.size(); ++ai.replacement) {
		if (ai.at_replacement ().name != ai.pokemon().name or ai.pokemon.set.size() == 1) {
			if (verbose or first_turn)
				std::cout << indent + "Evaluating switching to " + ai.at_replacement().to_string() + "\n";
			int64_t beta = Score::VICTORY + 1;
			for (foe.replacement = 0; foe.replacement != foe.pokemon.set.size(); ++foe.replacement) {
				if (foe.at_replacement ().name != foe.pokemon().name or foe.pokemon.set.size() == 1) {
					if (first == nullptr)
						beta = std::min (beta, (fainted (ai, foe, weather, depth, score) + fainted (foe, ai, weather, depth, score)) / 2);
					else
						beta = std::min (beta, fainted (*first, *last, weather, depth, score));
					if (beta <= alpha	or foe.pokemon().hp.stat != 0)
						break;
				}
			}
			if (beta > alpha) {
				alpha = beta;
				best_move = static_cast<Move::Moves> (Move::SWITCH0 + ai.replacement);
				if (verbose or first_turn)
					std::cout << indent + "Estimated score is " << alpha << '\n';
			}
			if (ai.pokemon().hp.stat != 0)
				break;
		}
	}
	return alpha;
}

int64_t fainted (Team first, Team last, Weather weather, unsigned depth, Score const & score) {
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

	int64_t value;
	Team* ai;
	Team* foe;
	deorder (first, last, ai, foe);
	if (depth == 0)
		value = score.evaluate (*ai, *foe, weather);
	else
		value = transposition (*ai, *foe, weather, depth, score);
	return value;
}

int64_t move_then_switch_branch (Team & switcher, Team const & other, Weather const & weather, unsigned depth, Score const & score, Move::Moves & best_switch, bool first_turn, bool verbose) {
	std::string indent = "";
	if (!first_turn)
		indent += "\t\t";
	int64_t alpha = -Score::VICTORY - 1;
	if (!switcher.me) {
		alpha = -alpha;
		indent += "\t";
	}
	for (switcher.replacement = 0; switcher.replacement != switcher.pokemon.set.size(); ++switcher.replacement) {
		if (switcher.at_replacement ().name != switcher.pokemon().name) {
			if (first_turn)
				std::cout << indent + "Evaluating bringing in " + switcher.at_replacement ().to_string () + "\n";
			int64_t value = switch_after_move_branch (switcher, other, weather, depth, score);
			if (first_turn)
				std::cout << indent + "Estimated score is " << value << '\n';
			if (switcher.me) {
				if (value > alpha) {
					alpha = value;
					best_switch = static_cast <Move::Moves> (Move::SWITCH0 + switcher.replacement);
				}
			}
			else {
				if (value < alpha) {
					alpha = value;
					best_switch = static_cast <Move::Moves> (Move::SWITCH0 + switcher.replacement);
				}
			}
		}
	}
	return alpha;
}

int64_t switch_after_move_branch (Team switcher, Team other, Weather weather, unsigned depth, Score const & score) {
	switchpokemon (switcher, other, weather);
	/*
	I don't have to correct for which of the Pokemon moved first because there are only two options:
	
	Option 1: only the switcher has moved. Then it obviously went first and I'm passing them in the proper order.
	
	Option 2: Both Pokemon have moved. use_move_branch then recalculates which Pokemon is faster to properly account for end-of-turn effects. In this case, it doesn't matter what order I pass them.
	*/
	return use_move_branch (switcher, other, weather, depth, score);
}

void deorder (Team & first, Team & last, Team* & ai, Team* & foe) {
	if (first.me) {
		ai = & first;
		foe = & last;
	}
	else {
		foe = & first;
		ai = & last;
	}
}

void print_best_move (Team const & team, Move::Moves best_move, unsigned depth, int64_t score) {
	if (Move::is_switch (best_move))
		std::cout << "Switch to " << team.pokemon.set [best_move - Move::SWITCH0].to_string ();
	else
		std::cout << "Use " << Move::to_string (best_move);
	if (depth == static_cast <unsigned> (-1)) {
		double probability = 100.0 * static_cast <double> (score + Score::VICTORY) / static_cast <double> (2 * Score::VICTORY);
		std::cout << " for ";
		if ((8 <= probability and probability < 9) or (11 <= probability and probability < 12) or (18 <= probability and probability < 19) or (80 <= probability and probability < 90))
			std::cout << "an ";
		else
			std::cout << "a ";
		std::cout << probability << "% chance to win.\n";
	}
	else
		std::cout << " for a minimum expected score of " << score << "\n";
}

void print_action (Team const & team, bool verbose, bool first_turn, std::string indent) {
	if (!team.me)
		indent += "\t";
	if (verbose or first_turn) {
		std::cout << indent + "Evaluating ";
		if (team.pokemon().move().is_switch())
			std::cout << "switching to " + team.pokemon.set [team.pokemon().move().name - Move::SWITCH0].to_string () + "\n";
		else
			std::cout << team.pokemon().move().to_string() + "\n";
	}
}

}
