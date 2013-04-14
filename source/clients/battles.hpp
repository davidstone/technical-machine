// Handles challenges / current battles
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

#ifndef CLIENTS__BATTLES_HPP_
#define CLIENTS__BATTLES_HPP_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "battle.hpp"

namespace technicalmachine {

class Battles {
public:
	template<typename SpecificBattle, typename ... Args>
	SpecificBattle const & add_pending_challenge(std::string const & opponent, Args && ... args) {
		Pointer battle_ptr(new SpecificBattle(opponent, std::forward<Args>(args)...));
		auto const & battle = static_cast<SpecificBattle const &>(*battle_ptr);
		challenges.insert(std::make_pair(opponent, std::move(battle_ptr)));
		return battle;
	}
	void handle_challenge_withdrawn();
	void handle_challenge_withdrawn(std::string const & opponent);
	Battle & handle_begin(uint32_t battle_id, std::string const & opponent);
	void handle_end(uint32_t battle_id);
	Battle const & find(uint32_t battle_id) const;
	Battle & find(uint32_t battle_id);
	bool challenges_are_queued() const;
	std::string const & first_challenger() const;
private:
	typedef std::unique_ptr<Battle> Pointer;
	// Battles that have not yet begun
	typedef std::map<std::string, Pointer> Challenges;
	// Battles currently underway
	typedef std::map<uint32_t, Pointer> Active;
	Challenges challenges;
	Active active;
};

}	// namespace technicalmachine
#endif	// CLIENTS__BATTLES_HPP_
