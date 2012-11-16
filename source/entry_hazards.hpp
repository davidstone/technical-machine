// Entry hazards
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

#ifndef ENTRY_HAZARDS_HPP_
#define ENTRY_HAZARDS_HPP_

#include <cstdint>

namespace technicalmachine {
class Team;
class Weather;

// Using an entry hazard puts them down on the opponent's side of the field.

class EntryHazards {
	public:
		EntryHazards();
		void clear();
		void add_spikes();
		void add_toxic_spikes();
		void add_stealth_rock();
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
		static void apply(Team & switcher, Weather const & weather);
		friend bool operator== (EntryHazards lhs, EntryHazards rhs);
	private:
		static void apply_toxic_spikes(Team & switcher, Weather const & weather);
		uint8_t spikes;
		uint8_t toxic_spikes;
		bool stealth_rock;
		friend class Evaluate;
};

bool operator!= (EntryHazards lhs, EntryHazards rhs);

}	// namespace technicalmachine
#endif	// ENTRY_HAZARDS_HPP_
