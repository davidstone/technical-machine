// Status class
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

#ifndef STATUS_HPP_
#define STATUS_HPP_

#include <cstdint>
#include <string>

namespace technicalmachine {

class Pokemon;
class Weather;

class Status {
	public:
		enum Statuses : uint8_t {
			NO_STATUS,
			BURN,
			FREEZE,
			PARALYSIS,
			POISON,
			POISON_TOXIC,
			REST,
			SLEEP,
			END
		};
		Statuses name;
		Status ();
		void clear ();
		bool is_clear() const;
		bool is_sleeping () const;
		static std::string to_string (Statuses name);
		std::string to_string () const;
		static Statuses from_string (std::string const & str);
		static void burn (Pokemon & user, Pokemon & target, Weather const & weather);
		static void freeze (Pokemon const & user, Pokemon & target, Weather const & weather);
		static void paralyze (Pokemon & user, Pokemon & target, Weather const & weather);
		static void sleep (Pokemon const & user, Pokemon & target, Weather const & weather);
		static void poison (Pokemon & user, Pokemon & target, Weather const & weather);
		static void poison_toxic (Pokemon & user, Pokemon & target, Weather const & weather);
		friend bool operator== (Status lhs, Status rhs);
		friend bool operator!= (Status lhs, Status rhs);
};

}	// namespace technicalmachine
#endif	// STATUS_HPP_
