// Effectiveness of a type
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

#ifndef TYPE__EFFECTIVENESS_HPP_
#define TYPE__EFFECTIVENESS_HPP_

#include "type.hpp"
#include <vector>
#include "../rational.hpp"

namespace technicalmachine {
class Pokemon;

class Effectiveness {
public:
	Effectiveness(Type::Types type, Pokemon const & defender);
	Effectiveness(Type::Types attacking, Type::Types defending);
	static Effectiveness stealth_rock_effectiveness(Pokemon const & pokemon);
	bool is_super_effective() const;
	bool is_not_very_effective() const;
	bool has_no_effect() const;

	template<typename Integer>
	friend Integer operator*=(Integer & number, Effectiveness const & e) {
		for (Rational const & rational : e.effectiveness)
			number *= rational;
		return number;
	}
private:
	std::vector<Rational> effectiveness;
};

template<typename Integer>
inline Integer operator*(Integer number, Effectiveness const & effectiveness) {
	return number *= effectiveness;
}
template<typename Integer>
inline Integer operator*(Effectiveness const & effectiveness, Integer number) {
	return number *= effectiveness;
}

}	// namespace technicalmachine
#endif	// TYPE__EFFECTIVENESS_HPP_
