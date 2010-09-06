#ifndef EXPECTIMINIMAX_H_
#define EXPECTIMINIMAX_H_

#include "move.h"
#include "team.h"
#include "weather.h"

void reset_iterators (teams &team);
moves_list expectiminimax (teams &ai, teams &foe, const weathers &weather, int depth, int &score);
int tree1 (teams ai, teams foe, weathers weather, int depth, moves_list &best_move);
int tree2 (teams first, teams last, weathers weather, int depth, moves_list &best_move);
int tree3 (teams first, teams last, weathers weather, int length_first, int length_last, bool shed_skin_first, bool shed_skin_last, int depth, moves_list &best_move);

#endif
