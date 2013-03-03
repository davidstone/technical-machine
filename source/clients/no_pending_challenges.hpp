// Exception thrown when attempting to work with a challenge that does not exist
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

#ifndef CLIENTS__NO_PENDING_CHALLENGES_HPP_
#define CLIENTS__NO_PENDING_CHALLENGES_HPP_

#include <cstdint>
#include <stdexcept>
#include <string>

namespace technicalmachine {

class NoPendingChallenges : public std::runtime_error {
	public:
		NoPendingChallenges();
		explicit NoPendingChallenges(std::string const & name);
		explicit NoPendingChallenges(uint32_t const id);
};

}	// namespace technicalmachine
#endif	// CLIENTS__NO_PENDING_CHALLENGES_HPP_
