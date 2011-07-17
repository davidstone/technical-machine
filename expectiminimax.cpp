// Expectiminimax
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
#include <boost/lexical_cast.hpp>
#include "expectiminimax.h"
#include "block.h"
#include "endofturn.h"
#include "evaluate.h"
#include "move.h"
#include "reorder_moves.h"
#include "pokemon.h"
#include "stat.h"
#include "switch.h"
#include "team.h"
#include "transposition.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

moves_list expectiminimax (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, int64_t &score) {
	std::cout << "======================\nEvaluating to a depth of " << depth << "...\n";
	moves_list best_move;
	score = select_move_branch (ai, foe, weather, depth, sv, best_move, true);

	if (SWITCH0 <= best_move and best_move <= SWITCH5)
		std::cout << "Switch to " << ai.pokemon.set [best_move - SWITCH0].get_name ();
	else
		std::cout << "Use " << Move::name_to_string [best_move];
	if (depth == -1) {
		double probability = 100.0 * static_cast <double> (score + VICTORY) / static_cast <double> (2 * VICTORY);
		std::cout << " for ";
		if ((8 <= probability and probability < 9) or (11 <= probability and probability < 12) or (18 <= probability and probability < 19) or (80 <= probability and probability < 90))
			std::cout << "an ";
		else
			std::cout << "a ";
		std::cout << probability << "% chance to win.\n";
	}
	else
		std::cout << " for a minimum expected score of " << score << "\n";

	return best_move;
}


int64_t select_move_branch (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool first_turn) {

	/* Working from the inside loop out:

	The following begins by setting beta to the largest possible value. This is the variable that the opposing player is trying to minimize. As long as the opposing player has any move that won't guarantee their loss, that move will score lower (more negative) than VICTORY, and thus the opponent will set that as their best response to the particular move that the AI uses.

	After looking at each response the opponent has to a given move, beta is finally set to whatever the score will be if the AI uses that move. alpha is initially set to the lowest possible value, so as long as the AI has any move that won't guarantee its loss, that move will score higher (more positive) than -VICTORY, and thus the AI will set that as its best response. It then replaces that move if it finds a move for which the opponent's best response is more positive than the first move found. In other words, it finds the move for the AI for which the foe's best response is the weakest.
	
	Something to consider as a potential speed up at the cost of some accuracy (but would allow a deeper, thus more accurate, search) would be to pick from all random numbers randomly, rather than seeing the outcome of all of them and averaging it. In other words, do several trials assuming a particular (but different for each trial) set of random numbers are selected, and then average that result. This would give massive reductions to the branching factor, and with a large enough number of trials should be close enough to the average to potentially speed up the program enough to justify the loss in accuracy.
	
	I subtract 1 from -VICTORY to make sure that even a guaranteed loss is seen as better than not returning a result. This way, I can do some things when my intermediate scores are strictly greater than alpha, rather than greater than or equal to, which can save a few calculations. This has the side-effect of limiting VICTORY to be at least one less than the greatest value representable by a long, which in practice shouldn't matter.
	
	For a similar reason, I later set beta to VICTORY + 1.
	
	This change also has the advantage of making sure a move is always put into best_move without any additional logic, such as pre-filling it with some result.
	*/
	int64_t alpha = -VICTORY - 1;
	
	speed (ai, weather);
	speed (foe, weather);

	bool const verbose = false;		// This prints out search equal to the maximum depth normally, but any deeper searches will also print out with a single tab. This is not recommended for depth greater than 2.

	// This section is for replacing fainted Pokemon (and eventually Baton Pass and U-turn, theoretically).

	if (ai.pokemon->hp.stat == 0 or foe.pokemon->hp.stat == 0) {
		bool faint = true;
		alpha = replace (ai, foe, weather, depth, sv, best_move, faint, first_turn, verbose);
	}
	else if (ai.pass or foe.pass) {
		bool faint = false;
		alpha = replace (ai, foe, weather, depth, sv, best_move, faint, first_turn, verbose);
	}
	
	// This section is for selecting a move, including switches that aren't replacing a fainted Pokemon.

	else {
		if (depth > 0)
			--depth;
		
		std::string indent = "";
		if (verbose and !first_turn)
			indent += "\t\t";

		std::vector <std::pair <int64_t, size_t> > ai_index;
		reorder (ai.pokemon->move.set, ai_index);
		
		// Determine which moves can be legally selected
		for (ai.pokemon->move.index = 0; ai.pokemon->move.index != ai.pokemon->move.set.size(); ++ai.pokemon->move.index) {
			blockselection (ai, foe, weather);
			if (ai.pokemon->move->selectable) {
				if (verbose or first_turn) {
					std::cout << indent + "Evaluating ";
					if (ai.pokemon->move->is_switch())
						std::cout << "switching to " + ai.pokemon.set [ai.pokemon->move->name - SWITCH0].get_name () + "\n";
					else
						std::cout << ai.pokemon->move->get_name() + "\n";
				}
				int64_t beta = VICTORY + 1;
				for (foe.pokemon->move.index = 0; foe.pokemon->move.index != foe.pokemon->move.set.size(); ++foe.pokemon->move.index) {
					blockselection (foe, ai, weather);
					if (foe.pokemon->move->selectable) {
						if (verbose or first_turn) {
							std::cout << indent + "\tEvaluating the foe";
							if (foe.pokemon->move->is_switch())
								std::cout << " switching to " + foe.pokemon.set [foe.pokemon->move->name - SWITCH0].get_name() + "\n";
							else
								std::cout << "'s " + foe.pokemon->move->get_name() + "\n";
						}
						int64_t score = order_branch (ai, foe, weather, depth, sv);
						if (verbose or first_turn)
							std::cout << indent + "\tEstimated score is " << score << '\n';
						if (beta > score) {
							beta = score;
							foe.pokemon->move->score = beta;
						}
						if (beta <= alpha)	// Alpha-Beta pruning
							break;
					}
				}
				// If their best response isn't as good as their previous best response, then this new move must be better than the previous AI's best move
				if (beta > alpha) {
					alpha = beta;
					ai.pokemon->move->score = alpha;
					best_move = ai.pokemon->move->name;
					if (verbose or first_turn)
						std::cout << indent + "Estimated score is " << alpha << '\n';
				}
				if (alpha == VICTORY)	// There is no way the AI has a better move than a guaranteed win
					break;
			}
		}
	}
	return alpha;
}


int64_t order_branch (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv) {
	// Determine turn order
	Team* first;
	Team* last;
	order (ai, foe, weather, first, last);
	int64_t score;
	if (first == NULL)		// If both Pokemon are the same speed and moves are the same priority
		score = (accuracy_branch (ai, foe, weather, depth, sv) + accuracy_branch (foe, ai, weather, depth, sv)) / 2;
	else
		score = accuracy_branch (*first, *last, weather, depth, sv);
	return score;
}


int64_t accuracy_branch (Team &first, Team &last, Weather const &weather, int depth, score_variables const &sv) {
	int divisor = 100 * 100;
	chance_to_hit (first, last, weather);
	first.moved = true;
	chance_to_hit (last, first, weather);
	first.moved = false;
	int64_t score = first.chance_to_hit * last.chance_to_hit * random_move_effects_branch (first, last, weather, depth, sv);
	if (first.chance_to_hit != 100) {
		first.miss = true;
		score += (100 - first.chance_to_hit) * last.chance_to_hit * random_move_effects_branch (first, last, weather, depth, sv);
		if (last.chance_to_hit != 100) {
			last.miss = true;
			score += (100 - first.chance_to_hit) * (100 - last.chance_to_hit) * random_move_effects_branch (first, last, weather, depth, sv);
			last.miss = false;
		}
		first.miss = false;
	}
	if (last.chance_to_hit != 100) {
		last.miss = true;
		score += first.chance_to_hit * (100 - last.chance_to_hit) * random_move_effects_branch (first, last, weather, depth, sv);
		last.miss = false;
	}
	score /= divisor;
	return score;
}


int64_t random_move_effects_branch (Team &first, Team &last, Weather const &weather, int depth, score_variables const &sv) {
	int64_t score3 = 0;
	for (first.pokemon->move->variable.index = 0; first.pokemon->move->variable.index != first.pokemon->move->variable.set.size(); ++first.pokemon->move->variable.index) {
		if ((first.pokemon->move->name != ROAR and first.pokemon->move->name != WHIRLWIND) or first.pokemon->move->variable->first != last.pokemon.index or last.pokemon.set.size() == 1) {
			int64_t score2 = 0;
			for (last.pokemon->move->variable.index = 0; last.pokemon->move->variable.index != last.pokemon->move->variable.set.size(); ++last.pokemon->move->variable.index) {
				if ((last.pokemon->move->name != ROAR and last.pokemon->move->name != WHIRLWIND) or last.pokemon->move->variable->first != first.pokemon.index or first.pokemon.set.size() == 1) {
					first.ch = false;
					last.ch = false;
					int64_t score1 = awaken_branch (first, last, weather, depth, sv);
					if (first.pokemon->move->basepower > 0 and last.pokemon->move->basepower <= 0) {
						score1 *= 15;
						first.ch = true;
						score1 += awaken_branch (first, last, weather, depth, sv);
						score1 /= 16;
					}
					else if (first.pokemon->move->basepower <= 0 and last.pokemon->move->basepower > 0) {
						score1 *= 15;
						last.ch = true;
						score1 += awaken_branch (first, last, weather, depth, sv);
						score1 /= 16;
					}
					else if (first.pokemon->move->basepower > 0 and last.pokemon->move->basepower > 0) {
						score1 *= 225;
						first.ch = true;
						score1 += awaken_branch (first, last, weather, depth, sv) * 15;
						last.ch = true;
						score1 += awaken_branch (first, last, weather, depth, sv);
						first.ch = false;
						score1 += awaken_branch (first, last, weather, depth, sv) * 15;
						score1 /= 256;
					}
					score2 += score1 * last.pokemon->move->variable->second;
				}
			}
			score3 += score2 * first.pokemon->move->variable->second / Move::max_probability;
		}
	}
	return score3 / Move::max_probability;
}


int64_t awaken_branch (Team &first, Team &last, Weather const &weather, int depth, score_variables const &sv) {
	int n;
	if (first.pokemon->ability == EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int first_numerator = first.pokemon->sleep + n - 1;
	if (last.pokemon->ability == EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int last_numerator = last.pokemon->sleep + n - 1;
	int64_t score;
	first.awaken = false;
	last.awaken = false;
	score = use_move_branch (first, last, weather, depth, sv);
	if (first_numerator > 1) {
		score *= 4 - first_numerator;
		first.awaken = true;
		score += first_numerator * use_move_branch (first, last, weather, depth, sv);
		if (last_numerator > 1) {
			score *= 4 - last_numerator;
			last.awaken = true;
			score += last_numerator * (4 - first_numerator) * use_move_branch (first, last, weather, depth, sv);
			first.awaken = false;
			score += last_numerator * first_numerator * use_move_branch (first, last, weather, depth, sv);
			score /= 4;
		}
		score /= 4;
	}
	else if (last_numerator > 1) {
		score *= 4 - last_numerator;
		last.awaken = true;
		score += last_numerator * use_move_branch (first, last, weather, depth, sv);
		score /= 4;
	}
	return score;
}


int64_t use_move_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv) {
	if (!last.pass) {
		if (!first.pass) {
			if (first.pokemon->move->name == BATON_PASS and false) {
				first.pass = true;
				Team* ai;
				Team* foe;
				deorder (first, last, ai, foe);
				moves_list phony = END_MOVE;
				return select_move_branch (*ai, *foe, weather, depth, sv, phony);
			}
		}
		last.damage = usemove (first, last, weather);
		first.pass = false;
		// win() already corrects for whether it's the AI or the foe that is passed as first vs. last
		if (win (first) != 0 or win (last) != 0)
			return win (first) + win (last);
		
		if (last.pokemon->move->name == BATON_PASS and false) {
			last.pass = true;
			Team* ai;
			Team* foe;
			deorder (first, last, ai, foe);
			moves_list phony = END_MOVE;
			return select_move_branch (*ai, *foe, weather, depth, sv, phony);
		}
	}
	
	// If first uses a phazing move before last gets a chance to move, the newly brought out Pokemon would try to move without checking to see if it's already moved.
	if (!last.moved)
		usemove (last, first, weather);
	last.pass = false;
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);

	// Find the expected return on all possible outcomes at the end of the turn
	
	first.shed_skin = false;
	last.shed_skin = false;
	int64_t score = 49 * end_of_turn_branch (first, last, weather, depth, sv);
	int64_t divisor = 49;
	if (first.pokemon->ability == SHED_SKIN and first.pokemon->status != NO_STATUS) {
		first.shed_skin = true;
		score += 21 * end_of_turn_branch (first, last, weather, depth, sv);
		divisor += 21;
		if (last.pokemon->ability == SHED_SKIN and last.pokemon->status != NO_STATUS) {
			last.shed_skin = true;
			score += 9 * end_of_turn_branch (first, last, weather, depth, sv);
			divisor += 9;
			first.shed_skin = false;
		}
	}
	if (last.pokemon->ability == SHED_SKIN and last.pokemon->status != NO_STATUS) {
		last.shed_skin = true;
		score += 21 * end_of_turn_branch (first, last, weather, depth, sv);
		divisor += 21;
	}
	return score / divisor;
}


int64_t end_of_turn_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv) {
	endofturn (first, last, weather);
	int64_t score;
	if (win (first) != 0 or win (last) != 0)
		score = win (first) + win (last);
	else {
		Team* ai;
		Team* foe;
		deorder (first, last, ai, foe);

		score = transposition (*ai, *foe, weather, depth, sv);
	}
	return score;
}

int64_t replace (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool faint, bool first_turn, bool verbose) {

	int64_t (*function) (Team first, Team last, Weather weather, int depth, score_variables const &sv);
	if (faint)
		function = &fainted;
	else
		function = &use_move_branch;
	Team* first;
	Team* last;
	faster_pokemon (ai, foe, weather, first, last);
	std::string indent = "";
	if (verbose and !first_turn)
		indent += "\t\t";
	int64_t alpha = -VICTORY - 1;
	for (ai.replacement = 0; ai.replacement != ai.pokemon.set.size(); ++ai.replacement) {
		if (ai.pokemon.set [ai.replacement].name != ai.pokemon->name or ai.pokemon.set.size() == 1) {
			if (verbose or first_turn)
				std::cout << indent + "Evaluating switching to " + ai.at_replacement().get_name() + "\n";
			int64_t beta = VICTORY + 1;
			for (foe.replacement = 0; foe.replacement != foe.pokemon.set.size(); ++foe.replacement) {
				if (foe.pokemon.set [foe.replacement].name != foe.pokemon->name or foe.pokemon.set.size() == 1) {
					if (first == NULL)
						beta = std::min (beta, ((*function) (ai, foe, weather, depth, sv) + (*function) (foe, ai, weather, depth, sv)) / 2);
					else
						beta = std::min (beta, (*function) (*first, *last, weather, depth, sv));
					if (beta <= alpha	// Alpha-Beta pruning
							or (foe.pokemon->hp.stat != 0 and faint)
							or (!foe.pass and !faint))
						break;
				}
			}
			if (beta > alpha) {
				alpha = beta;
				best_move = static_cast<moves_list> (SWITCH0 + ai.replacement);
				if (verbose or first_turn)
					std::cout << indent + "Estimated score is " << alpha << '\n';
			}
			if ((ai.pokemon->hp.stat != 0 and faint) or (!ai.pass and !faint))
				break;
		}
	}
	return alpha;
}

int64_t fainted (Team first, Team last, Weather weather, int depth, score_variables const &sv) {
	if (first.pokemon->hp.stat == 0) {
		switchpokemon (first, last, weather);
		if (win (first) != 0 or win (last) != 0)
			return win (first) + win (last);
	}
	if (last.pokemon->hp.stat == 0) {
		switchpokemon (last, first, weather);
		if (win (first) != 0 or win (last) != 0)
			return win (first) + win (last);
	}

	int64_t score;
	Team* ai;
	Team* foe;
	deorder (first, last, ai, foe);
	if (depth == 0)
		score = evaluate (*ai, *foe, weather, sv);
	else
		score = transposition (*ai, *foe, weather, depth, sv);
	return score;
}

void deorder (Team &first, Team &last, Team* &ai, Team* &foe) {
	if (first.me) {
		ai = &first;
		foe = &last;
	}
	else {
		foe = &first;
		ai = &last;
	}
}

}
