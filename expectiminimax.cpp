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
#include "endofturn.h"
#include "evaluate.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "simple.h"
#include "statfunction.h"
#include "team.h"
#include "transposition.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

moves_list expectiminimax (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, long &score) {
	moves_list best_move = END_MOVE;
	std::string output = "";
	std::map<long, State> transposition_table;
	score = tree1 (ai, foe, weather, depth, sv, best_move, output, transposition_table, true);

//	std::cout << output + "\n";
	return best_move;
}


long tree1 (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table, bool first) {
	reset_iterators (ai);		// I'm not sure why these two lines are needed when I'm passing both teams by reference instead of value
	reset_iterators (foe);
	if (depth != -1)
		--depth;
	
	/* Working from the inside loop out:

	The following begins by setting beta to the largest possible value. This is the variable that the opposing player is trying to minimize. As long as the opposing player has any move that won't guarantee their loss, that move will score lower (more negative) than VICTORY, and thus the opponent will set that as their best response to the particular move that the AI uses.

	After looking at each response the opponent has to a given move, beta is finally set to whatever the score will be if the AI uses that move. alpha is initially set to the lowest possible value, so as long as the AI has any move that won't guarantee its loss, that move will score higher (more positive) than -VICTORY, and thus the AI will set that as its best response. It then replaces that move if it finds a move for which the opponent's best response is more positive than the first move found. In other words, it finds the move for the AI for which the foe's best response is the weakest.
	
	Something to consider as a potential speed up at the cost of some accuracy (but would allow a deeper, thus more accurate, search) would be to pick from all random numbers randomly, rather than seeing the outcome of all of them and averaging it. In other words, do several trials assuming a particular (but different for each trial) set of random numbers are selected, and then average that result. This would give massive reductions to the branching factor, and with a large enough number of trials should be close enough to the average to potentially speed up the program enough to justify the loss in accuracy.
	*/
	
	std::string output2 = "";
	long alpha = -VICTORY;
	
	// Determine which moves can be legally selected
	for (ai.active->move = ai.active->moveset.begin(); ai.active->move != ai.active->moveset.end(); ++ai.active->move) {
		if (first)
			std::cout << "Evaluating " << move_name [ai.active->move->name] << '\n';
		blockselection (ai, foe, weather);
		if (ai.active->move->selectable) {
			if (best_move == END_MOVE)
				best_move = ai.active->moveset.front().name;		// Makes sure that even if all moves lead to a guaranteed loss, the program still decides that some move is the best move instead of crashing
			for (ai.replacement = 0; ai.replacement != ai.member.size(); ++ai.replacement) {
				if (ai.member.at (ai.replacement).name == ai.active->name and ai.member.size() > 1)
					continue;
				long beta = VICTORY;
				for (foe.active->move = foe.active->moveset.begin(); foe.active->move != foe.active->moveset.end(); ++foe.active->move) {
					blockselection (foe, ai, weather);
					if (foe.active->move->selectable) {
						for (foe.replacement = 0; foe.replacement != foe.member.size(); ++foe.replacement) {
							if (foe.member.at (foe.replacement).name == foe.active->name and foe.member.size() > 1)
								continue;
							std::string maybe_output;
							long score = tree2 (ai, foe, weather, depth, sv, best_move, maybe_output, transposition_table);
							if (beta >= score) {	// Test for equality to make sure a move is the foe's best move
								beta = score;
								foe.active->move->score = beta;
								output2 = pokemon_name [foe.active->name];
								if (SWITCH1 <= foe.active->move->name and foe.active->move->name <= SWITCH6)
									output2 += " switched to " + pokemon_name [foe.member.at (foe.active->move->name - SWITCH1).name];
								else
									output2 += " used " + move_name [foe.active->move->name];
								output = maybe_output;
							}
							if (beta <= alpha)	// Alpha-Beta pruning
								break;
						}
					}
				}
				// If their best response still isn't as good as their previous best response, then this new move must be better than the previous AI's best move
				if (beta > alpha) {
					alpha = beta;
					ai.active->move->score = alpha;
					best_move = ai.active->move->name;
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


long tree2 (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
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
	for (ai.active->move->variable = ai.active->move->range.begin(); ai.active->move->variable != ai.active->move->range.end(); ++ai.active->move->variable) {
		long score4 = 0;
		for (foe.active->move->variable = foe.active->move->range.begin(); foe.active->move->variable != foe.active->move->range.end(); ++foe.active->move->variable) {
			ai.active->move->effect = 0;
			long score3 = 0;		// Temporary variable for probability calculations
			do {
				long score2 = 0;		// Temporary variable for probability calculations
				foe.active->move->effect = 0;
				do {
					ai.active->move->ch = false;
					foe.active->move->ch = false;
					long score1 = tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table);
					if (ai.active->move->basepower > 0 and foe.active->move->basepower <= 0) {
						score1 *= 15;
						ai.active->move->ch = true;
						score1 += tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table);
						score1 /= 16;
					}
					else if (ai.active->move->basepower <= 0 and foe.active->move->basepower > 0) {
						score1 *= 15;
						foe.active->move->ch = true;
						score1 += tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table);
						score1 /= 16;
					}
					else if (ai.active->move->basepower > 0 and foe.active->move->basepower > 0) {
						score1 *= 225;
						ai.active->move->ch = true;
						score1 += tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table) * 15;
						foe.active->move->ch = true;
						score1 += tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table);
						ai.active->move->ch = false;
						score1 += tree3 (ai, foe, weather, depth, sv, best_move, first, last, output, transposition_table) * 15;
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
		score5 += score4 / foe.active->move->range.size();
	}
	return score5 / ai.active->move->range.size();
}


long tree3 (const Team &ai, const Team &foe, const Weather &weather, const int &depth, const score_variables &sv, moves_list &best_move, Team* first, Team* last, std::string &output, std::map<long, State> &transposition_table) {
	if (first == NULL)		// If both Pokemon are the same speed and moves are the same priority
		return (tree4 (ai, foe, weather, depth, sv, best_move, output, transposition_table) + tree4 (foe, ai, weather, depth, sv, best_move, output, transposition_table)) / 2;
	return tree4 (*first, *last, weather, depth, sv, best_move, output, transposition_table);
}


long tree4 (Team first, Team last, Weather weather, int depth, const score_variables &sv, moves_list &old_move, std::string &output, std::map<long, State> &transposition_table) {
	reset_iterators (first);
	reset_iterators (last);

	// I have to pass best_move by reference so tree1() can give information to expectiminimax(), but I don't want future calls to overwrite information
	moves_list best_move = old_move;

	bool hitself = false;
	int old_damage = usemove (first, last, weather, hitself);

//	std::cout << "Remaining depth is " << depth << " as " << pokemon_name [first.active->name] << " uses " << move_name [first.active->move->name] << " leaving the foe at " << last.active->hp.stat << '\n';

	// win() already corrects for whether it's the AI or the foe that is passed as first vs. last
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);			// 0 if both Pokemon die (a draw), VICTORY if the AI wins, -VICTORY if the foe wins
	usemove (last, first, weather, hitself, old_damage);

//	std::cout << "Remaining depth is " << depth << " as " << pokemon_name [last.active->name] << " uses " << move_name [last.active->move->name] << " leaving the foe at " << first.active->hp.stat << '\n';
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);

	// Find the expected return on all possible outcomes at the end of the turn
	
	Random random;
	
	long score3 = 0;
	// Length of sleep caused by Yawn is a value between 2 and 5
	for (random.first.length = 2; random.first.length != 6;) {
		long score2 = 0;
		for (random.last.length = 2; random.last.length != 6;) {
			random.first.shed_skin = false;
			random.last.shed_skin = false;
			long score1 = 49 * tree5 (first, last, weather, random, depth, sv, best_move, output, transposition_table);
			long divisor = 49;
			if (first.active->ability == SHED_SKIN and first.active->status != NO_STATUS) {
				random.first.shed_skin = true;
				score1 += 21 * tree5 (first, last, weather, random, depth, sv, best_move, output, transposition_table);
				divisor += 21;
				if (last.active->ability == SHED_SKIN and last.active->status != NO_STATUS) {
					random.last.shed_skin = true;
					score1 += 9 * tree5 (first, last, weather, random, depth, sv, best_move, output, transposition_table);
					divisor += 9;
					random.first.shed_skin = false;
				}
			}
			if (last.active->ability == SHED_SKIN and last.active->status != NO_STATUS) {
				random.last.shed_skin = true;
				score1 += 21 * tree5 (first, last, weather, random, depth, sv, best_move, output, transposition_table);
				divisor += 21;
			}
			score2 += score1 / divisor;
			++random.last.length;
			// Don't find out the possible outcomes from Yawn if Yawn isn't active
			if (last.active->yawn == 0)
				break;
		}
		score3 += score2 / (random.last.length - 2);
		++random.first.length;
		if (first.active->yawn == 0)
			break;
	}
	return score3 / (random.first.length - 2);
}


long tree5 (Team first, Team last, Weather weather, const Random &random, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
	reset_iterators_pokemon (first);
	reset_iterators_pokemon (last);
	endofturn (first, last, weather, random);
	if (win (first) != 0 or win (last) != 0)
		return win (first) + win (last);			// 0 if both Pokemon die (a draw), VICTORY if the AI wins, -VICTORY if the foe wins
	return tree6 (first, last, weather, depth, sv, best_move, output, transposition_table);
}

long tree6 (Team &first, Team &last, const Weather &weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
	long score;
	if (first.active->hp.stat == 0) {
		output += "hi";
		long alpha = -VICTORY;
		for (first.replacement = 0; first.replacement != first.member.size(); ++first.replacement) {
			if (first.member.at (first.replacement).name != first.active->name or first.member.size() == 1) {
				long beta = VICTORY;
				for (last.replacement = 0; last.replacement != last.member.size(); ++last.replacement) {
					if (last.member.at (last.replacement).name != last.active->name or last.member.size() == 1) {
						if (last.active->hp.stat == 0) {
							output += "hello";
							beta = std::min (beta, tree7 (first, last, weather, depth, sv, best_move, output, transposition_table));
							if (beta <= alpha)	// Alpha-Beta pruning
								break;
						}
						else
							break;
					}
				}
				if (beta >= alpha)
					alpha = beta;
			}
		}
		score = alpha;
	}
	else if (last.active->hp.stat == 0) {
		output += "hola";
		long beta = VICTORY;
		for (last.replacement = 0; last.replacement != last.member.size(); ++last.replacement) {
			if (last.member.at (last.replacement).name != last.active->name or last.member.size() == 1)
				beta = std::min (beta, tree7 (first, last, weather, depth, sv, best_move, output, transposition_table));
		}
		score = beta;
	}
	else
		score = tree7 (first, last, weather, depth, sv, best_move, output, transposition_table);

	return score;
}

long tree7 (Team first, Team last, Weather weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
	reset_iterators_pokemon (first);
	reset_iterators_pokemon (last);
	if (first.active->hp.stat == 0)
		switchpokemon (first, *last.active, weather);
	if (last.active->hp.stat == 0)
		switchpokemon (last, *first.active, weather);
	if (first.active->hp.stat != 0 and last.active->hp.stat != 0) {
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
			return evaluate (*ai, *foe, weather, sv);
		return tree1 (*ai, *foe, weather, depth, sv, best_move, output, transposition_table);
//		return transposition (*ai, *foe, weather, depth, sv, best_move, output, transposition_table);
	}
	return tree6 (first, last, weather, depth, sv, best_move, output, transposition_table);
}

}
