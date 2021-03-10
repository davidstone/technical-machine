// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/algorithms/erase.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/at.hpp>
#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>
#include <containers/push_back.hpp>

#include <stdexcept>
#include <utility>

namespace technicalmachine {
namespace ps {

struct SlotMemory {
	using Index = decltype(std::declval<TeamIndex>() + 1_bi);
	
	constexpr explicit SlotMemory(TeamSize const size) {
		if (size == 0_bi) {
			throw std::runtime_error("Pokemon Showdown told us to construct an empty team.");
		}
		for (auto const index : containers::integer_range(size)) {
			containers::push_back(m_container, index + 1_bi);
		}
	}
	
	constexpr auto operator[](TeamIndex const index) const -> Index {
		return containers::at(m_container, index);
	}
	void switch_to(TeamIndex const index) {
		swap_to_front(index);
	}
	void replace_fainted(TeamIndex const index) {
		if (containers::at(m_container, index) == 1_bi) {
			throw std::runtime_error("Pokemon Showdown told us to switch to currently active Pokemon.");
		}
		auto const it = swap_to_front(index);
		containers::erase(m_container, it);
	}
private:
	using Container = containers::static_vector<Index, max_pokemon_per_team.value()>;
	auto swap_to_front(TeamIndex const index) -> Container::iterator {
		auto const it = containers::find(m_container, 1_bi);
		BOUNDED_ASSERT(it != containers::end(m_container));
		std::swap(*it, containers::at(m_container, index));
		return it;
	}
	Container m_container;
};

}	// namespace ps
}	// namespace technicalmachine
