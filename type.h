// Type information
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TYPE_H_
#define TYPE_H_

#include <vector>

namespace technicalmachine {

enum Type { BUG, DARK, DRAGON, ELECTRIC, FIGHTING, FIRE, FLYING, GHOST, GRASS, GROUND, ICE, NORMAL, POISON, PSYCHIC_TYPE, ROCK, STEEL, WATER, TYPELESS };

class Pokemon;
class Team;
class Weather;

bool is_type (Team const & team, Type type);
unsigned get_effectiveness (Type type, Pokemon const & pokemon);
std::vector <unsigned> get_effectiveness_variables (Type type, Pokemon const & pokemon);
bool grounded (Team const & team, Weather const & weather);

}
#endif		// TYPE_H_
