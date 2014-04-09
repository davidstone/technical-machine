// Nature data structure
// Copyright (C) 2014 David Stone
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

#ifndef STAT__NATURE_HPP_
#define STAT__NATURE_HPP_

#include <cstdint>
#include <bounded_integer/bounded_integer.hpp>

#include "stat_names.hpp"

#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded::literal;

class Nature {
public:
	enum Natures : uint8_t {
		ADAMANT, BASHFUL, BOLD, BRAVE, CALM,
		CAREFUL, DOCILE, GENTLE, HARDY, HASTY,
		IMPISH, JOLLY, LAX, LONELY, MILD, MODEST,
		NAIVE, NAUGHTY, QUIET, QUIRKY, RASH,
		RELAXED, SASSY, SERIOUS, TIMID, END
	};
	Nature();
	// intentionally implicit
	Nature(Natures nature);
	Nature(StatNames boosted, StatNames dropped);
	bool is_set () const;
	void set_if_unknown (Natures nature);

	template<StatNames stat>
	auto boost() const {
		auto const numerator = BOUNDED_INTEGER_CONDITIONAL(
			boosts_stat<stat>(), 11_bi,
			BOUNDED_INTEGER_CONDITIONAL(lowers_stat<stat>(), 9_bi, 10_bi)
		);
		static constexpr auto denominator = 10_bi;
		return make_rational(numerator, denominator);
	}

	template<StatNames>
	bool boosts_stat() const;
	template<StatNames>
	bool lowers_stat() const;
	friend bool operator== (Nature lhs, Nature rhs);

	Natures name;
};
bool operator!= (Nature lhs, Nature rhs);

bool boosts_attacking_stat(Nature nature);
bool boosts_defending_stat(Nature nature);
bool lowers_attacking_stat(Nature nature);
bool lowers_defending_stat(Nature nature);


}	// namespace technicalmachine
#endif	// STAT__NATURE_HPP_
