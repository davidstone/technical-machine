// Handles challenges / current battles
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

#ifndef NETWORK__BATTLES_HPP_
#define NETWORK__BATTLES_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace technicalmachine {
class GenericBattle;
namespace network {

class Battles {
	public:
		// Takes ownership of the battle
		void add_pending_challenge(GenericBattle * battle);
		void handle_challenge_withdrawn();
		void handle_challenge_withdrawn(std::string const & opponent);
		GenericBattle & handle_begin(uint32_t battle_id, std::string const & opponent);
		void handle_end(uint32_t battle_id);
		GenericBattle const & find(uint32_t battle_id) const;
		GenericBattle & find(uint32_t battle_id);
		bool challenges_are_queued() const;
		std::string first_challenger() const;
	private:
		typedef std::unique_ptr<GenericBattle> Pointer;
		// Battles that have not yet begun
		typedef std::map<std::string, Pointer> Challenges;
		// Battles currently underway
		typedef std::map<uint32_t, Pointer> Active;
		Challenges challenges;
		Active active;
};

}	// namespace network
}	// namespace technicalmachine
#endif	// NETWORK__BATTLES_HPP_
