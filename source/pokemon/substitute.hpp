// Substitute class
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

#ifndef SUBSTITUTE_HPP_
#define SUBSTITUTE_HPP_

#include "../stat/hp.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {
using namespace bounded_integer::literal;

class Substitute {
private:
	using hp_type = bounded_integer::equivalent_type<decltype(std::declval<HP::current_type>() / 4_bi), bounded_integer::null_policy>;
public:
	Substitute();
	auto create(HP::current_type total_hp) -> hp_type;
	template<typename Damage>
	auto damage(Damage const damage_done) -> void {
		m_hp -= damage_done;
	}
	auto hp() const {
		return m_hp;
	}
	explicit operator bool() const;
	typedef uint64_t hash_type;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	bounded_integer::equivalent_type<hp_type, bounded_integer::clamp_policy> m_hp;
};
bool operator== (Substitute const & lhs, Substitute const & rhs);
bool operator!= (Substitute const & lhs, Substitute const & rhs);

}	// namespace technicalmachine
#endif	// SUBSTITUTE_HPP_
