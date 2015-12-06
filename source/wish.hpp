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

#include "hash.hpp"
#include "operators.hpp"

#include <bounded_integer/optional.hpp>
#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
struct MutableActivePokemon;
struct Pokemon;

struct Wish {
	auto activate() -> void;
	auto decrement(MutableActivePokemon pokemon) -> void;
	constexpr auto is_active() const {
		return static_cast<bool>(m_turns_until_activation);
	}
	friend auto operator==(Wish lhs, Wish rhs) -> bool;
private:
	using counter_type = bounded::checked_integer<0, 1>;
	bounded::optional<counter_type> m_turns_until_activation = bounded::none;
};

// This is sufficient because hashing only has to distinguish end-of-turn
// conditions.
constexpr auto hash(Wish const wish) noexcept {
	return hash(wish.is_active());
}

}	// namespace technicalmachine
