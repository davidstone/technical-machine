// PP class
// Copyright (C) 2013 David Stone
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

#ifndef MOVE__PP_HPP_
#define MOVE__PP_HPP_

#include <cstdint>
#include <utility>
#include <ranged_integer/optional.hpp>
#include <ranged_integer/ranged_integer.hpp>
#include "moves_forward.hpp"

namespace technicalmachine {
class Ability;

class Pp {
public:
	using pp_ups_type = checked_integer<0, 3>;
	Pp(Moves move, pp_ups_type pp_ups);
	uint64_t hash () const;
	uint64_t max_hash () const;
	bool is_empty() const;
	bool has_unlimited_pp() const;
	void decrement(Ability const & foe_ability);
	native_integer<40, 200> trump_card_power() const;
	// Assumes max PP is the same because it assumes the same Move on the same
	// Pokemon
	friend bool operator== (Pp const & lhs, Pp const & rhs);
private:
	using base_type = checked_integer<1, 40>;
	using max_type = decltype(std::declval<base_type>() * (std::declval<pp_ups_type>() + 5_ri) / 5_ri);
	// clamped_integer simplifies situations like Pressure and Leppa
	using current_type = clamped_integer<0, static_cast<intmax_t>(std::numeric_limits<max_type>::max())>;
	static optional<max_type> calculate_max(optional<base_type> base, pp_ups_type pp_ups);
	static optional<base_type> get_base_pp(Moves move);
	optional<max_type> max;
	optional<current_type> current;
};
bool operator!= (Pp const & lhs, Pp const & rhs);

}	// namespace technicalmachine
#endif	// MOVE__PP_HPP_
