// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.clients.ps.slot_memory;

import tm.pokemon.max_pokemon_per_team;

import bounded;
import containers;
import numeric_traits;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;

// TODO: Should TeamSize be at least 1? Should this accept a different type?
constexpr auto validate_size(TeamSize const size) {
	return bounded::check_in_range(size, 1_bi, numeric_traits::max_value<TeamSize>);
}

export struct SlotMemory {
	using Index = decltype(bounded::declval<TeamIndex>() + 1_bi);

	constexpr explicit SlotMemory(TeamSize const size):
		m_container(containers::inclusive_integer_range(1_bi, validate_size(size)))
	{
	}

	constexpr auto operator[](TeamIndex const index) const -> Index {
		return containers::at(m_container, index);
	}
	constexpr auto switch_to(TeamIndex const index) -> void {
		swap_to_front(index);
	}
	constexpr auto replace_fainted(TeamIndex const index) -> void {
		if (containers::at(m_container, index) == 1_bi) {
			throw std::runtime_error("Pokemon Showdown told us to switch to currently active Pokemon.");
		}
		auto const it = swap_to_front(index);
		containers::erase(m_container, it);
	}

private:
	using Container = containers::static_vector<Index, max_pokemon_per_team>;
	constexpr auto swap_to_front(TeamIndex const index) -> containers::iterator_t<Container &> {
		auto const it = containers::find(m_container, 1_bi);
		BOUNDED_ASSERT(it != containers::end(m_container));
		std::swap(*it, containers::at(m_container, index));
		return it;
	}
	Container m_container;
};

} // namespace technicalmachine::ps
