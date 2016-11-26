// Pokemon Showdown incoming messages
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

#pragma once

#include <containers/vector/vector.hpp>

#include <string>

namespace technicalmachine {
namespace ps {

struct InMessage {
	using Room = std::string;
	using Type = std::string;
	using Data = containers::vector<std::string>;

	InMessage(Room room, Type type, Data data);

	auto const & type() const noexcept {
		return m_type;
	}

	friend auto begin(InMessage const & message) {
		return begin(message.m_data);
	}
	friend auto end(InMessage const & message) {
		return end(message.m_data);
	}

	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS
private:
	Room m_room;
	Type m_type;
	Data m_data;
};

}	// namespace ps
}	// namespace technicalmachine
