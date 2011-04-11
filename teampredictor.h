// Predict foe's team header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TEAMPREDICTOR_H_
#define TEAMPREDICTOR_H_

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>
#include <vector>
#include "team.h"
#include "pokemon.h"

namespace technicalmachine {

void overall_stats (std::vector<double> &overall);

void team_stats (const std::vector<double> &overall, double total, double multiplier [END_SPECIES][END_SPECIES]);

void lead_stats (std::vector<double> &lead);

void detailed_stats (const Map &map, int detailed [][7]);

void predict (int detailed [][7], Team &team);

void predict (int detailed [][7], Team &team, bool using_lead);	// For the stand-alone team predictor

void predict_pokemon (Team &team, std::vector<double> estimate, int detailed [][7], double multiplier [END_SPECIES][END_SPECIES]);

void predict_move (pokemon &member, int detailed [][7]);

}
#endif
