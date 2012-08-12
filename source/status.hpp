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

class Ability;
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
		Status ();
		Statuses name() const;
		void clear ();
		bool is_clear() const;
		bool is_frozen() const;
		bool is_sleeping () const;
		bool is_sleeping_due_to_other() const;
		bool lowers_speed (Ability const & ability) const;
		bool weakens_physical_attacks() const;
		bool boosts_facade() const;
		bool boosts_smellingsalt() const;

		static std::string to_string (Statuses name);
		std::string to_string () const;
		static Statuses from_string (std::string const & str);

		void rest ();
		template<Statuses real_status, Statuses base_status = real_status>
		static void apply(Pokemon & user, Pokemon & target, Weather const & weather);
		template<Statuses real_status>
		static void apply(Pokemon & target, Weather const & weather);
		static void shift (Pokemon & user, Pokemon & target, Weather const & weather);

		friend bool operator== (Status lhs, Status rhs);
		friend bool operator!= (Status lhs, Status rhs);

		void increase_sleep_counter (Ability const & ability, bool awaken);
		bool can_awaken(Ability const & ability) const;
		unsigned awaken_numerator (Ability const & ability) const;
		static unsigned min_sleep_turns();
		static unsigned max_sleep_turns();

		uint64_t hash() const;
		static uint64_t max_hash();
	private:
		Statuses status;
		uint8_t turns_already_slept;
};

}	// namespace technicalmachine
#endif	// STATUS_HPP_
