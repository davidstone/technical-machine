// Class to represent Uproar's counter
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

#ifndef UPROAR_HPP_
#define UPROAR_HPP_

#include <bounded_integer/optional.hpp>

namespace technicalmachine {

class Uproar {
public:
	auto increment() -> void;
	friend auto operator== (Uproar const & lhs, Uproar const & rhs) -> bool;
	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	friend class Evaluate;
	bounded::optional<bounded::integer<1, 4>> m_counter;
};
auto operator!= (Uproar const & lhs, Uproar const & rhs) -> bool;

}	// namespace technicalmachine
#endif	// UPROAR_HPP_
