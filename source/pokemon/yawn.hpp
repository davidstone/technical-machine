// Class to represent Yawn's counter
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

#ifndef YAWN_HPP_
#define YAWN_HPP_

#include <bounded_integer/optional.hpp>

namespace technicalmachine {

class Yawn {
public:
	auto activate() -> void;
	// return value represents whether Yawn activated
	auto advance_turn() -> bool;
	friend auto operator==(Yawn const & lhs, Yawn const & rhs) -> bool;
	typedef uint64_t hash_type;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	friend class Evaluate;
	// If present, value represents number of turns since Yawn has been used
	bounded::optional<bounded::integer<0, 1>> m_counter;
};
auto operator!= (Yawn const & lhs, Yawn const & rhs) -> bool;

}	// namespace technicalmachine
#endif	// YAWN_HPP_
