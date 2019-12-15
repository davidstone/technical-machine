// Copyright (C) 2019 David Stone
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

#include <tm/item.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/representation_digits.hpp>

namespace technicalmachine {

auto cannot_be_lost(Item item) -> bool;

struct HeldItem {
	constexpr explicit HeldItem(Item const item):
		m_item(item),
		m_active(true)
	{
	}

	constexpr auto get() const {
		return m_active ? m_item : Item::None;
	}

	// Remove the item in a way that can be recovered with Recycle
	constexpr auto remove() & -> bounded::optional<Item> {
		if (!m_active or cannot_be_lost(m_item)) {
			return bounded::none;
		}
		m_active = false;
		// Need to explicitly copy m_item because otherwise we would bind a
		// reference to a bitfield
		return Item(m_item);
	}
	// Remove the item in a way that cannot be recovered with Recycle
	constexpr auto destroy() & -> bool {
		if (!m_active or m_item == Item::None) {
			return false;
		}
		m_item = Item::None;
		return true;
	}

	constexpr auto recycle() & -> void {
		m_active = true;
	}
private:
	Item m_item : bounded::representation_bits<Item>.value();
	bool m_active : 1;
};

auto fling_power(Item item) -> bounded::integer<0, 130>;
}	// namespace technicalmachine
