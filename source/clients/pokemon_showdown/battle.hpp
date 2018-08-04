// Pokemon Showdown battle logic
// Copyright (C) 2018 David Stone
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

#pragma once

#include "inmessage.hpp"

#include "../battle.hpp"

#include <bounded/optional.hpp>
#include <containers/vector/vector.hpp>

namespace technicalmachine {
namespace ps {

struct BattleParser {
	template<typename ... Args>
	BattleParser(Args &&... args):
		m_battle(std::forward<Args>(args)..., 100_bi)
	{
	}
	
	void handle_message(InMessage message);
private:
	Battle m_battle;
};

struct BattleFactory {
	explicit BattleFactory(std::string_view username):
		m_username(username)
	{
	}
	
	void handle_message(InMessage message);
	
	bool completed() const {
		return m_completed;
	}
	
	bounded::optional<Battle> make() const;

private:
	enum class Clause { };
	std::string_view m_username;
	bounded::optional<std::string> m_player_id;
	bounded::optional<std::string> m_type;	// singles, doubles, triples
	bounded::optional<std::string> m_tier;
	containers::vector<Clause> m_rules;
	bounded::optional<std::uint8_t> m_generation;
	bool m_completed = false;
};

}	// namespace ps
}	// namespace technicalmachine
