// Pokemon Showdown outgoing messages
// Copyright (C) 2015 David Stone
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

#include "outmessage.hpp"

#include <cstdint>
#include <string>
#include <utility>

#include "../../team.hpp"

namespace technicalmachine {
namespace ps {

void OutMessage::write_string(std::string const & str) {
	static_cast<void>(str);
}

void OutMessage::write_team(Team const & team, std::string const & username) {
	static_cast<void>(team);
	static_cast<void>(username);
}

void OutMessage::write_move(uint32_t battle_id, uint8_t move_index, uint8_t target) {
	static_cast<void>(battle_id);
	static_cast<void>(move_index);
	static_cast<void>(target);
}

void OutMessage::write_switch(uint32_t battle_id, uint8_t slot) {
	static_cast<void>(battle_id);
	static_cast<void>(slot);
}

void OutMessage::finalize() {
}

} // namespace ps
} // namespace technicalmachine
