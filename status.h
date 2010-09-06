// Status enum
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATUS_H_
#define STATUS_H_

enum statuses { NO_STATUS, BURN, FREEZE, PARALYSIS, POISON_NORMAL, POISON_TOXIC, SLEEP };

// I'm not including pseudo-status like Leech Seed and Confusion here, because those have dramatically different effects in the game.

#endif
