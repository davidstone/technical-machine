// Convert to / from PL's format
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

#ifndef POKEMON_LAB__CONVERSION_HPP_
#define POKEMON_LAB__CONVERSION_HPP_

namespace technicalmachine {
namespace pl {

template<typename Destination, typename Source>
Destination simulator_cast(Source const & source);

}	// namespace pl
}	// namespace technicalmachine
#endif	// POKEMON_LAB__CONVERSION_HPP_
