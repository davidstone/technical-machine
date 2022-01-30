// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/saturating_add.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Moves : std::uint16_t;

using namespace bounded::literal;

struct PP {
	using pp_ups_type = bounded::integer<0, 3>;

	PP(Generation, Moves, pp_ups_type);

	auto remaining() const {
		return m_current;
	}

	auto restore(auto const value) & {
		static_assert(value >= 0_bi);
		BOUNDED_ASSERT(*m_current == 0_bi);
		*m_current = bounded::min(value, *m_max);
	}

	auto reduce(auto const value) & {
		static_assert(numeric_traits::min_value<decltype(value)> >= 0_bi);
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

	using max_type = bounded::integer<1, 64>;

private:
	using current_type = bounded::integer<0, bounded::normalize<numeric_traits::max_value<max_type>>>;
	// TODO: Use optional<pair> instead of pair<optional>
	bounded::optional<max_type> m_max;
	bounded::optional<current_type> m_current;
};

inline auto no_pp(PP const pp) {
	auto const remaining = pp.remaining();
	return remaining and *remaining == 0_bi;
}

} // namespace technicalmachine
