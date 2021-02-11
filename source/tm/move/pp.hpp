// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/moves.hpp>

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/saturating_add.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <utility>

namespace technicalmachine {

using namespace bounded::literal;

struct PP {
	using pp_ups_type = bounded::integer<0, 3>;

	PP(Generation const generation, Moves const move, pp_ups_type const pp_ups):
		m_max(calculate_max(base_pp(generation, move), pp_ups)),
		m_current(m_max ? bounded::optional<current_type>(*m_max) : bounded::none)
	{
	}

	auto remaining() const {
		return m_current;
	}

	auto restore(auto const value) & {
		static_assert(value >= 0_bi);
		BOUNDED_ASSERT(*m_current == 0_bi);
		*m_current = bounded::min(value, *m_max);
	}

	auto reduce(auto const value) & {
		static_assert(bounded::min_value<decltype(value)> >= 0_bi);
		if (!m_current) {
			return;
		}
		saturating_add(*m_current, -value);
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
	using max_type = bounded::integer<1, 64>;

	static constexpr auto calculate_max(bounded::optional<base_type> const base, pp_ups_type const pp_ups) -> bounded::optional<max_type> {
		if (!base) {
			return bounded::none;
		}
		auto const result = *base * (pp_ups + 5_bi) / 5_bi;
		static_assert(std::is_same_v<decltype(result), max_type const>);
		return result;
	}

	using current_type = bounded::integer<0, static_cast<int>(bounded::max_value<max_type>)>;
	// TODO: Use optional<pair> instead of pair<optional>
	bounded::optional<max_type> m_max;
	bounded::optional<current_type> m_current;
	
	static auto base_pp(Generation const generation, Moves const move) -> bounded::optional<base_type>;
};

} // namespace technicalmachine
