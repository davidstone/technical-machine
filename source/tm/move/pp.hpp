// Copyright (C) 2017 David Stone
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

#include <tm/move/moves.hpp>

#include <tm/ability.hpp>
#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <utility>

namespace technicalmachine {

using namespace bounded::literal;

struct PP {
	using pp_ups_type = bounded::checked_integer<0, 3>;

	PP(Generation const generation, Moves const move, pp_ups_type const pp_ups):
		m_max(calculate_max(base_pp(generation, move), pp_ups)),
		m_current(m_max ? bounded::optional<current_type>(*m_max) : bounded::none)
	{
	}

	auto is_empty() const {
		return m_current and *m_current == 0_bi;
	}

	auto restore(auto const value) & {
		static_assert(value >= 0_bi);
		BOUNDED_ASSERT(*m_current == 0_bi);
		*m_current = bounded::min(value, *m_max);
	}

	auto has_unlimited_pp() const {
		return !m_max;
	}

	auto decrement(Ability const foe_ability) & {
		if (has_unlimited_pp()) {
			return;
		}
		// I think it is always an error to try to decrement a move without PP.
		BOUNDED_ASSERT(*m_current != 0_bi);
		*m_current -= BOUNDED_CONDITIONAL(uses_extra_pp(foe_ability), 2_bi, 1_bi);
	}

	auto trump_card_power() const -> bounded::integer<40, 200> {
		// Should be safe because we assume we are using Trump Card
		switch (m_current->value()) {
			case 0: return 200_bi;
			case 1: return 80_bi;
			case 2: return 60_bi;
			case 3: return 50_bi;
			default: return 40_bi;
		}
	}

	// Assumes max PP is the same because it assumes the same Move on the same
	// Pokemon
	friend auto operator==(PP const lhs, PP const rhs) -> bool {
		return lhs.m_current == rhs.m_current;
	}

	// Assumes max PP is the same because it assumes the same Move on the same
	// Pokemon
	friend auto compress(PP const value) {
		return compress(value.m_current);
	}

private:
	using base_type = bounded::integer<1, 40>;
	
	// It is not allowed to use a static function with deduced return type to
	// define a typedef in the same class
	using max_type = decltype(std::declval<base_type>() * (std::declval<pp_ups_type>() + 5_bi) / 5_bi);

	static constexpr auto calculate_max(bounded::optional<base_type> const base, pp_ups_type const pp_ups) -> bounded::optional<max_type> {
		return base ? bounded::optional(*base * (pp_ups + 5_bi) / 5_bi) : bounded::none;
	}

	// clamped_integer simplifies situations like Pressure and Leppa
	using current_type = bounded::clamped_integer<0, static_cast<int>(bounded::max_value<max_type>)>;
	// TODO: Use optional<pair> instead of pair<optional>
	bounded::optional<max_type> m_max;
	bounded::optional<current_type> m_current;
	
	static auto base_pp(Generation const generation, Moves const move) -> bounded::optional<base_type>;
};

}	// namespace technicalmachine
