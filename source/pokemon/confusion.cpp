// Class that represents the duration left on Confusion
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

#include "confusion.hpp"
#include "pokemon.hpp"

namespace technicalmachine {
// min_duration = 2;
// max_duration = 5;

namespace {
auto register_self_hit(Pokemon &) -> void {
	// TODO: write this
}
}	// namespace

auto Confusion::do_turn(Pokemon & pokemon) -> void {
	increment();
	if (is_active() and m_is_hitting_self) {
		register_self_hit(pokemon);
	}
}

auto Confusion::increment() -> void {
	if (!is_active()) {
		return;
	}
	if (*m_turns_spent_confused == max_duration) {
		m_turns_spent_confused = bounded::none;
	} else {
		++*m_turns_spent_confused;
	}
}

}	// namespace technicalmachine
