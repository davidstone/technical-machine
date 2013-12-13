// Wish data structure
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

#ifndef WISH_HPP_
#define WISH_HPP_

#include <cstdint>
#include <bounded_integer/optional.hpp>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
class ActivePokemon;
class Pokemon;

class Wish {
public:
	void activate();
	void decrement(ActivePokemon & pokemon);
	typedef uint64_t hash_type;
	hash_type hash() const;
	static hash_type max_hash();
	friend bool operator== (Wish lhs, Wish rhs);
private:
	bool is_active() const;
	using counter_type = bounded_integer::checked_integer<0, 1>;
	bounded_integer::optional<counter_type> turns_until_activation;
	friend class Evaluate;
};

bool operator!= (Wish lhs, Wish rhs);

}	// namespace technicalmachine
#endif	// WISH_HPP_
