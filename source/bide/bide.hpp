// Handles bide damage and when it activates
// Copyright (C) 2014 David Stone
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

#include "damage.hpp"
#include "duration.hpp"

#include "../hash.hpp"
#include "../stat/hp.hpp"

namespace technicalmachine {

struct Bide {
	auto is_active() const -> bool ;
	auto activate() -> void;
	auto add_damage(damage_type damage) -> void;
	auto decrement() -> damage_type;
	constexpr auto hash() const noexcept {
		return ::technicalmachine::hash(m_damage, m_duration);
	}
	friend auto operator== (Bide lhs, Bide rhs) -> bool ;
private:
	BideDamage m_damage;
	BideDuration m_duration;
};

auto operator!= (Bide lhs, Bide rhs) -> bool ;

constexpr auto hash(Bide const bide) noexcept {
	return bide.hash();
}

}	// namespace technicalmachine
