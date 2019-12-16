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

#include <tm/generation.hpp>
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

	constexpr auto get(Generation const generation, bool const embargo, bool const magic_room) const {
		if (!m_active or (embargo and affected_by_embargo(generation)) or magic_room) {
			return Item::None;
		}
		return m_item;
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
	constexpr auto affected_by_embargo(Generation const generation) const -> bool {
		// Iron_Ball is disabled in Generation 4 for the Speed check but not for
		// the grounding check
		switch (m_item) {
			case Item::Iron_Ball:
			case Item::Macho_Brace:
			case Item::Power_Anklet:
			case Item::Power_Band:
			case Item::Power_Belt:
			case Item::Power_Bracer:
			case Item::Power_Lens:
			case Item::Power_Weight:
				return generation <= Generation::four;
			default:
				return true;
		}
	}
	Item m_item : bounded::representation_bits<Item>.value();
	bool m_active : 1;
};

auto fling_power(Item item) -> bounded::integer<0, 130>;
}	// namespace technicalmachine
