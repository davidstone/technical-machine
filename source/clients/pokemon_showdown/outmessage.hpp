// Pokemon Showdown outgoing messages
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

#pragma once

#include <cstdint>
#include <string>

#include "../network/outmessage.hpp"

namespace technicalmachine {
struct Team;
namespace ps {

struct BattleSettings;

struct OutMessage : network::OutMessage {
	void write_string(std::string const & str) override;
	void write_team(Team const & team, std::string const & username) override;
	void write_move(uint32_t field_id, uint8_t move_index, uint8_t target = 1) override;
	void write_switch(uint32_t field_id, uint8_t slot) override;
	void finalize() override;
	enum Message {
	};
};

}	// namespace ps
}	// namespace technicalmachine
