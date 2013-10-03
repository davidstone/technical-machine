// Which Move is disabled and for how long
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

#ifndef POKEMON__DISABLE_HPP_
#define POKEMON__DISABLE_HPP_

#include <cstdint>

namespace technicalmachine {

class Disable {
public:
	Disable();
	void activate(uint8_t index_of_disabled_move);
	void advance_one_turn();
	void reset();
	bool move_is_disabled(uint8_t index_of_move_to_check) const;
	uint64_t hash() const;
	static uint64_t max_hash();
	friend bool operator== (Disable lhs, Disable rhs);
private:
	uint8_t m_index_of_disabled_move;
	uint8_t m_turns_disabled;
};

bool operator!= (Disable lhs, Disable rhs);

}	// namespace technicalmachine
#endif	// POKEMON__DISABLE_HPP_
