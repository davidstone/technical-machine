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

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "expectiminimax.h"
#include "block.h"
#include "endofturn.h"
#include "evaluate.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "statfunction.h"
#include "switch.h"
#include "team.h"
#include "transposition.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

moves_list expectiminimax (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, long &score) {
	std::cout << "======================\nEvaluating to a depth of " << depth << "...\n";
	moves_list best_move;
	std::map<long, State> transposition_table;
	score = tree1 (ai, foe, weather, depth, sv, best_move, transposition_table, true);

	if (SWITCH0 <= best_move and best_move <= SWITCH5)
		std::cout << "Switch to " << pokemon_name [ai.active.set [best_move - SWITCH0].name];
	else
		std::cout << "Use " << move_name [best_move];
	if (depth == -1) {
		long double probability = 100.0 * static_cast <long double> (score + VICTORY) / static_cast <long double> (2 * VICTORY);
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


long tree1 (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, moves_list &best_move, std::map<long, State> &transposition_table, bool first) {

	/* Working from the inside loop out:

	The following begins by setting beta to the largest possible value. This is the variable that the opposing player is trying to minimize. As long as the opposing player has any move that won't guarantee their loss, that move will score lower (more negative) than VICTORY, and thus the opponent will set that as their best response to the particular move that the AI uses.

	After looking at each response the opponent has to a given move, beta is finally set to whatever the score will be if the AI uses that move. alpha is initially set to the lowest possible value, so as long as the AI has any move that won't guarantee its loss, that move will score higher (more positive) than -VICTORY, and thus the AI will set that as its best response. It then replaces that move if it finds a move for which the opponent's best response is more positive than the first move found. In other words, it finds the move for the AI for which the foe's best response is the weakest.
	
	Something to consider as a potential speed up at the cost of some accuracy (but would allow a deeper, thus more accurate, search) would be to pick from all random numbers randomly, rather than seeing the outcome of all of them and averaging it. In other words, do several trials assuming a particular (but different for each trial) set of random numbers are selected, and then average that result. This would give massive reductions to the branching factor, and with a large enough number of trials should be close enough to the average to potentially speed up the program enough to justify the loss in accuracy.
	
	I subtract 1 from -VICTORY to make sure that even a guaranteed loss is seen as better than not returning a result. This way, I can do some things when my intermediate scores are strictly greater than alpha, rather than greater than or equal to, which can save a few calculations. This has the side-effect of limiting VICTORY to be at least one less than the greatest value representable by a long, which in practice shouldn't matter.
	
	For a similar reason, I later set beta to VICTORY + 1.
	
	This change also has the advantage of making sure a move is always put into best_move without any additional logic, like pre-filling it with some result.
	*/
	long alpha = -VICTORY - 1;

	// This section is for replacing fainted Pokemon (and eventually Baton Pass and U-turn, theoretically).

	if (ai.active->hp.stat == 0 or foe.active->hp.stat == 0) {
		Team* first;
		Team* last;
		faster_pokemon (ai, foe, weather, first, last);
		for (ai.replacement = 0; ai.replacement != ai.active.set.size(); ++ai.replacement) {
			if (ai.active.set [ai.replacement].name != ai.active->name or ai.active.set.size() == 1) {
				long beta = VICTORY + 1;
				for (foe.replacement = 0; foe.replacement != foe.active.set.size(); ++foe.replacement) {
					if (foe.active.set [foe.replacement].name != foe.active->name or foe.active.set.size() == 1) {
						if (first == NULL)
							beta = std::min (beta, (fainted (ai, foe, weather, depth, sv, transposition_table) + fainted (foe, ai, weather, depth, sv, transposition_table)) / 2);
						else
							beta = std::min (beta, fainted (*first, *last, weather, depth, sv, transposition_table));
						if (beta <= alpha	// Alpha-Beta pruning
								or foe.active->hp.stat != 0)	// Foe doesn't need replacement
							break;
					}
				}
				if (beta > alpha) {
					alpha = beta;
					best_move = static_cast<moves_list> (SWITCH0 + ai.replacement);
				}
				if (ai.active->hp.stat != 0)
					break;
			}
		}
	}
	
	// This section is for selecting a move, including switches that aren't replacing a fainted Pokemon.

	else {
		if (depth > 0)
			--depth;
		
//		std::string indent = "";
//		if (!first)
//			indent += "\t\t";
		// Determine which moves can be legally selected
		for (ai.active->move.index = 0; ai.active->move.index != ai.active->move.set.size(); ++ai.active->move.index) {
			blockselection (ai, foe, weather);
			if (ai.active->move->selectable) {
//				std::cout << indent + "Evaluating ";
				if (first) {
					std::cout << "Evaluating ";
					if (SWITCH0 <= ai.active->move->name and ai.active->move->name <= SWITCH5)
						std::cout << "switching to " + pokemon_name [ai.active.set [ai.active->move->name - SWITCH0].name] + "\n";
					else
						std::cout << move_name [ai.active->move->name] + "\n";
				}
				long beta = VICTORY + 1;
				for (foe.active->move.index = 0; foe.active->move.index != foe.active->move.set.size(); ++foe.active->move.index) {
					blockselection (foe, ai, weather);
					if (foe.active->move->selectable) {
//						std::cout << indent + "\tEvaluating the foe";
						if (first) {
							std::cout << "\tEvaluating the foe";
							if (SWITCH0 <= foe.active->move->name and foe.active->move->name <= SWITCH5)
								std::cout << " switching to " + pokemon_name [foe.active.set [foe.active->move->name - SWITCH0].name] + "\n";
							else
								std::cout << "'s " + move_name [foe.active->move->name] + "\n";
						}
						long score = tree2 (ai, foe, weather, depth, sv, transposition_table);
//						std::cout << indent + "\tEstimated score is " << score << '\n';
						if (first)
							std::cout << "\tEstimated score is " << score << '\n';
						if (beta > score)
							beta = score;
						if (beta <= alpha)	// Alpha-Beta pruning
							break;
					}
				}
				// If their best response isn't as good as their previous best response, then this new move must be better than the previous AI's best move
				if (beta > alpha) {
					alpha = beta;
					best_move = ai.active->move->name;
//					std::cout << indent + "Estimated score is " << alpha << '\n';
					if (first)
						std::cout << "Estimated score is " << alpha << '\n';
				}
				if (alpha == VICTORY)	// There is no way the AI has a better move than a guaranteed win
					break;
			}
		}
	}
	return alpha;
}


long tree2 (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, std::map<long, State> &transposition_table) {
	// Determine turn order
	Team* first;
	Team* last;
	order (ai, foe, weather, first, last);

	char ai_max;
	if (ai.active->move->name != FIRE_FANG and ai.active->move->name != ICE_FANG and ai.active->move->name != THUNDER_FANG)
		ai_max = 2;
	else
		ai_max = 4;
	char foe_max;
	if (foe.active->move->name != FIRE_FANG and foe.active->move->name != ICE_FANG and foe.active->move->name != THUNDER_FANG)
		foe_max = 2;
	else
		foe_max = 4;

	long score5 = 0;

	unsigned ai_range = 0;
	for (ai.active->move->variable.index = 0; ai.active->move->variable.index != ai.active->move->variable.set.size(); ++ai.active->move->variable.index) {
		if ((ai.active->move->name != ROAR and ai.active->move->name != WHIRLWIND) or *ai.active->move->variable != foe.active.index or foe.active.set.size() == 1) {
			++ai_range;
			long score4 = 0;
			unsigned foe_range = 0;
			for (foe.active->move->variable.index = 0; foe.active->move->variable.index != foe.active->move->variable.set.size(); ++foe.active->move->variable.index) {
				if ((foe.active->move->name != ROAR and foe.active->move->name != WHIRLWIND) or *foe.active->move->variable != ai.active.index or ai.active.set.size() == 1) {
					++foe_range;
					ai.active->move->effect = 0;
					long score3 = 0;		// Temporary variable for probability calculations
					do {
						long score2 = 0;		// Temporary variable for probability calculations
						foe.active->move->effect = 0;
						do {
							ai.active->move->ch = false;
							foe.active->move->ch = false;
							long score1 = tree3 (ai, foe, weather, depth, sv, first, last, transposition_table);
							if (ai.active->move->basepower > 0 and foe.active->move->basepower <= 0) {
								score1 *= 15;
								ai.active->move->ch = true;
								score1 += tree3 (ai, foe, weather, depth, sv, first, last, transposition_table);
								score1 /= 16;
							}
							else if (ai.active->move->basepower <= 0 and foe.active->move->basepower > 0) {
								score1 *= 15;
								foe.active->move->ch = true;
								score1 += tree3 (ai, foe, weather, depth, sv, first, last, transposition_table);
								score1 /= 16;
							}
							else if (ai.active->move->basepower > 0 and foe.active->move->basepower > 0) {
								score1 *= 225;
								ai.active->move->ch = true;
								score1 += tree3 (ai, foe, weather, depth, sv, first, last, transposition_table) * 15;
								foe.active->move->ch = true;
								score1 += tree3 (ai, foe, weather, depth, sv, first, last, transposition_table);
								ai.active->move->ch = false;
								score1 += tree3 (ai, foe, weather, depth, sv, first, last, transposition_table) * 15;
								score1 /= 256;
							}
							if (foe.active->move->effect == 0)
								score2 += score1 * (10 - foe.active->move->probability);
							else if (foe.active->move->effect == 1)
								score2 += score1 * foe.active->move->probability;
							++foe.active->move->effect;
						} while (foe.active->move->probability != 0 and foe.active->move->effect < foe_max);
						if (ai.active->move->effect == 0)
							score3 += score2 * (10 - ai.active->move->probability);
						else if (ai.active->move->effect == 1)
							score3 += score2 * ai.active->move->probability;
						++ai.active->move->effect;
					} while (ai.active->move->probability != 0 and ai.active->move->effect < ai_max);
					score4 += score3 / 100;
				}
			}
			score5 += score4 / foe_range;
		}
	}
	return score5 / ai_range;
}


long tree3 (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, Team* first, Team* last, std::map<long, State> &transposition_table) {
	int n;
	if (ai.active->ability == EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int ai_numerator = ai.active->sleep + n - 1;
	if (foe.active->ability == EARLY_BIRD)
		n = 2;
	else
		n = 1;
	int foe_numerator = foe.active->sleep + n - 1;
	long score;
	ai.awaken = false;
	foe.awaken = false;
	score = tree4 (ai, foe, weather, depth, sv, first, last, transposition_table);
	if (ai_numerator > 1) {
		score *= 4 - ai_numerator;
		ai.awaken = true;
		score += ai_numerator * tree4 (ai, foe, weather, depth, sv, first, last, transposition_table);
		if (ai_numerator > 1) {
			score *= 4 - foe_numerator;
			foe.awaken = true;
			score += foe_numerator * (4 - ai_numerator) * tree4 (ai, foe, weather, depth, sv, first, last, transposition_table);
			ai.awaken = false;
			score += foe_numerator * ai_numerator * tree4 (ai, foe, weather, depth, sv, first, last, transposition_table);
			score /= 4;
		}
		score /= 4;
	}
	else if (foe_numerator > 1) {
		score *= 4 - foe_numerator;
		foe.awaken = true;
		score += foe_numerator * tree4 (ai, foe, weather, depth, sv, first, last, transposition_table);
		score /= 4;
	}
	return score;
}

long tree4 (const Team &ai, const Team &foe, const Weather &weather, const int &depth, const score_variables &sv, Team* first, Team* last, std::map<long, State> &transposition_table) {
	long score;
	if (first == NULL)		// If both Pokemon are the same speed and moves are the same priority
		score = (tree5 (ai, foe, weather, depth, sv, transposition_table) + tree5 (foe, ai, weather, depth, sv, transposition_table)) / 2;
	else
		score = tree5 (*first, *last, weather, depth, sv, transposition_table);
	return score;
}


long tree5 (Team first, Team last, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table) {

	int old_damage = usemove (first, last, weather);

//	std::cout << "\t\t" + pokemon_name [first.active->name] + " uses " + move_name [first.active->move->name] + " leaving the foe at " << last.active->hp.stat << '\n';

	// win() already corrects for whether it's the AI or the foe that is passed as first vs. last
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);			// 0 if both Pokemon die (a draw), VICTORY if the AI wins, -VICTORY if the foe wins
	usemove (last, first, weather, old_damage);

//	std::cout << "\t\t" + pokemon_name [last.active->name] + " uses " + move_name [last.active->move->name] + " leaving the foe at " << first.active->hp.stat << '\n';
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);

	// Find the expected return on all possible outcomes at the end of the turn
	
	first.shed_skin = false;
	last.shed_skin = false;
	long score = 49 * tree6 (first, last, weather, depth, sv, transposition_table);
	long divisor = 49;
	if (first.active->ability == SHED_SKIN and first.active->status != NO_STATUS) {
		first.shed_skin = true;
		score += 21 * tree6 (first, last, weather, depth, sv, transposition_table);
		divisor += 21;
		if (last.active->ability == SHED_SKIN and last.active->status != NO_STATUS) {
			last.shed_skin = true;
			score += 9 * tree6 (first, last, weather, depth, sv, transposition_table);
			divisor += 9;
			first.shed_skin = false;
		}
	}
	if (last.active->ability == SHED_SKIN and last.active->status != NO_STATUS) {
		last.shed_skin = true;
		score += 21 * tree6 (first, last, weather, depth, sv, transposition_table);
		divisor += 21;
	}
	return score / divisor;
}


long tree6 (Team first, Team last, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table) {
	endofturn (first, last, weather);
	long score;
	if (win (first) != 0 or win (last) != 0)
		score = win (first) + win (last);
	else {
		Team* ai;
		Team* foe;
		if (first.me) {
			ai = &first;
			foe = &last;
		}
		else {
			foe = &first;
			ai = &last;
		}
		if (depth == 0)
		// and first.active->hp.stat > 0 and last.active->hp.stat > 0) // This line creates an infinite loop for some reason. Will look at fixing this later.
			score = evaluate (*ai, *foe, weather, sv);
		else {
			// I have to pass best_move by reference so tree1() can give information to expectiminimax(), but I don't want future calls to overwrite information
			moves_list phony = END_MOVE;

			score = tree1 (*ai, *foe, weather, depth, sv, phony, transposition_table);
		}
	}
	return score;
}

long fainted (Team first, Team last, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table) {
	if (first.active->hp.stat == 0) {
		switchpokemon (first, last, weather);
		if (win (first) != 0 or win (last) != 0)
			return win (first) + win (last);
	}
	if (last.active->hp.stat == 0) {
		switchpokemon (last, first, weather);
		if (win (first) != 0 or win (last) != 0)
			return win (first) + win (last);
	}

	long score;
	Team* ai;
	Team* foe;
	if (first.me) {
		ai = &first;
		foe = &last;
	}
	else {
		foe = &first;
		ai = &last;
	}
	if (depth == 0)
		score = evaluate (*ai, *foe, weather, sv);
	else {
		// I have to pass best_move by reference so tree1() can give information to expectiminimax(), but I don't want future calls to overwrite information
		moves_list phony = END_MOVE;
		score = tree1 (*ai, *foe, weather, depth, sv, phony, transposition_table);
//		return transposition (*ai, *foe, weather, depth, sv, transposition_table);
	}
	return score;
}

}
