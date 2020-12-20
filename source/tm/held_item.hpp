// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
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

	friend auto operator==(HeldItem, HeldItem) -> bool = default;
	friend constexpr auto compress(HeldItem const value) {
		return compress_combine(value.m_item, value.m_active);
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
