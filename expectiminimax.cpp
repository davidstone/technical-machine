// Expectiminimax
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <climits>
#include "expectiminimax.h"
#include "evaluate.h"
#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "team.h"
#include "type.h"
#include "weather.h"

moves_list expectiminimax (teams &ai, teams &foe, const weathers &weather, int depth, int &score) {
	moves_list best_move;
	score = tree1 (ai, foe, weather, depth, best_move);
	return best_move;
}

int tree1_base (const teams &ai, const teams &foe, const weathers &weather, const int &depth, moves_list &best_move, teams* first, teams* last, int beta) {
	if (first == NULL)		// If both Pokemon are the same speed and moves are the same priority
		beta = std::min (beta, (tree2 (ai, foe, weather, depth, best_move) + tree2 (foe, ai, weather, depth, best_move)) / 2);
	else
		beta = std::min (beta, tree2 (*first, *last, weather, depth, best_move));
	return beta;
}


int tree1 (teams ai, teams foe, weathers weather, int depth, moves_list &best_move) {
	reset_iterators (ai);
	reset_iterators (foe);
	--depth;
	
	/* Working from the inside loop out:

	The following begins by setting beta to the largest possible value. This is the variable that the opposing player is trying to minimize. As long as the opposing player has any move that won't guarantee their loss, that move will score lower (more negative) than INT_MAX, and thus the opponent will set that as their best response to the particular move that the AI uses.

	After looking at each response the opponent has to a given move, beta is finally set to whatever the score will be if the AI uses that move. alpha is initially set to the lowest possible value, so as long as the AI has any move that won't guarantee its loss, that move will score higher (more positive) than -INT_MAX, and thus the AI will set that as its best response. It then replaces that move if it finds a move for which the opponent's best response is more positive than the first move found. In other words, it finds the move for the AI for which the foe's best response is the weakest.
	*/
	
	int alpha = -INT_MAX;		// -INT_MAX is used in place of INT_MIN because I want max value + min value == 0. This helps when averaging scores.

	// Determine which moves can be legally selected
	for (ai.active->move = ai.active->moveset.begin(); ai.active->move != ai.active->moveset.end(); ++ai.active->move) {
		blockselection (ai, foe, weather);
		if (ai.active->move->selectable) {
			int beta = INT_MAX;
			for (foe.active->move = foe.active->moveset.begin(); foe.active->move != foe.active->moveset.end(); ++foe.active->move) {
				blockselection (foe, ai, weather);
				if (foe.active->move->selectable) {
					// Determine turn order
					teams* first;
					teams* last;
					order (ai, foe, weather, first, last);
					ai.active->move->ch = false;
					foe.active->move->ch = false;
					tree1_base (ai, foe, weather, depth, best_move, first, last, beta);
					if (ai.active->move->basepower != 0) {
						ai.active->move->ch = true;
						tree1_base (ai, foe, weather, depth, best_move, first, last, beta);
						ai.active->move->ch = false;
					}
					if (foe.active->move->basepower != 0) {
						foe.active->move->ch = true;
						tree1_base (ai, foe, weather, depth, best_move, first, last, beta);
						if (ai.active->move->basepower != 0) {
							ai.active->move->ch = true;
							tree1_base (ai, foe, weather, depth, best_move, first, last, beta);
						}
					}

					if (beta <= alpha)	// Alpha-Beta pruning
						break;
				}
			}
			// If their best response still isn't as good as their previous best response, then this new move must be better than the previous AI's best move
			if (beta > alpha) {
				alpha = beta;
				best_move = ai.active->move->name;
			}
			if (alpha == INT_MAX)
				break;
		}
	}
	return alpha;
}

int tree2 (teams first, teams last, weathers weather, int depth, moves_list &best_move) {
	reset_iterators (first);
	reset_iterators (last);
	bool hitself = false;
	usemove (first, last, weather, hitself);
	// win() already corrects for whether it's the AI or the foe that is passed as first vs. last
	if (win (first) != 0)
		return win (first);
	else if (win (last) != 0)
		return win (last);
	usemove (last, first, weather, hitself);
	if (win (last) != 0)
		return win (last);
	else if (win (first) != 0)
		return win (first);
	int score = 0;

	// Find the expected return on all possible outcomes at the end of the turn
	
	int length_first = 2;		// Length of sleep caused by Yawn is a value between 2 and 5
	do {
		int length_last = 2;
		do {
			if (first.active->ability != SHED_SKIN or first.active->status == NO_STATUS) {
				if (last.active->ability != SHED_SKIN or last.active->status == NO_STATUS)
					score += tree3 (first, last, weather, length_first, length_last, false, false, depth, best_move);
				else {
					score += (3 * tree3 (first, last, weather, length_first, length_last, false, true, depth, best_move)
						+ 7 * tree3 (first, last, weather, length_first, length_last, false, false, depth, best_move)) / 10;
				}
			}
			else {
				if (last.active->ability != SHED_SKIN or last.active->status == NO_STATUS) {
					score += (3 * tree3 (first, last, weather, length_first, length_last, true, false, depth, best_move)
						+ 7 * tree3 (first, last, weather, length_first, length_last, false, false, depth, best_move)) / 10;
				}
				else {
					score += (9 * tree3 (first, last, weather, length_first, length_last, true, true, depth, best_move)
						+ 21 * tree3 (first, last, weather, length_first, length_last, false, true, depth, best_move)
						+ 21 * tree3 (first, last, weather, length_first, length_last, true, false, depth, best_move)
						+ 49 * tree3 (first, last, weather, length_first, length_last, false, false, depth, best_move)) / 100;
				}
			}
			++length_last;
		} while (length_last != 6 and last.active->yawn != 0);
		score /= length_last - 2;
		++length_first;
	} while (length_first != 6 and first.active->yawn != 0);	// Don't find out the possible outcomes from Yawn if Yawn isn't active
	score /= length_first - 2;
	return score;
}

int tree3 (teams first, teams last, weathers weather, int length_first, int length_last, bool shed_skin_first, bool shed_skin_last, int depth, moves_list &best_move) {
	reset_iterators (first);
	reset_iterators (last);
	endofturn (first, last, weather, length_first, length_last, shed_skin_first, shed_skin_last);
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);			// 0 if both Pokemon die (a draw), INT_MAX if the AI wins, -INT_MAX if the foe wins
	teams* ai;
	teams* foe;
	if (first.me) {
		ai = &first;
		foe = &last;
	}
	else {
		foe = &first;
		ai = &last;
	}

	int score;
	if (depth == 0)
		score = evaluate (*ai, *foe, weather);
	else
		score = tree1 (*ai, *foe, weather, depth, best_move);
	return score;
}

void reset_iterators (teams &team) {
	species member = team.active->pokemon;
	for (std::vector<pokemon>::iterator it = team.member.begin(); ; ++it) {
		if (it->pokemon == member) {
			team.active = it;
			break;
		}
	}
	moves_list move = team.active->move->name;
	for (std::vector<moves>::iterator it = team.active->moveset.begin(); ; ++it) {
		if (it->name == move) {
			team.active->move = it;
			break;
		}
	}
}
