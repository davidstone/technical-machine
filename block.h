// Declaration of functions that block selection / execution
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef BLOCK_H_
#define BLOCK_H

namespace technicalmachine {

class Move;
class Team;
class Weather;

void blockexecution (Team & user, Team const & other, Weather const & weather);
void blockselection (Team & user, Team const & other, Weather const & weather);
bool block1 (Team const & user, Team const & other);
bool block2 (Team const & user, Weather const & weather);
bool imprison (Move const & move, Team const & other);

}

#endif
