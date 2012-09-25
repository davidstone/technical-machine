// Moves classification (physical, special, or other)
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

#ifndef MOVE__CLASSIFICATION_HPP_
#define MOVE__CLASSIFICATION_HPP_

#include <cstdint>
#include "moves_forward.hpp"

namespace technicalmachine {

class Classification {
	public:
		Classification(Moves move);
		bool is_physical() const;
		bool is_special() const;
	private:
		enum class Cached : int8_t;
		static Cached classification (Moves move);
		Cached cached;
};

}	// namespace technicalmachine
#endif	// MOVE__CLASSIFICATION_HPP_
