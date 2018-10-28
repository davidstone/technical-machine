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

#include <tm/clients/no_pending_challenges.hpp>

namespace technicalmachine {

NoPendingChallenges::NoPendingChallenges():
	std::runtime_error("No pending challenges.")
	{
}
NoPendingChallenges::NoPendingChallenges(std::string const & name):
	std::runtime_error("No pending challenges from " + name + ".")
	{
}
NoPendingChallenges::NoPendingChallenges(uint32_t const id):
	std::runtime_error("No pending challenges from user id " + std::to_string(id) + ".")
	{
}

}	// namespace technicalmachine
