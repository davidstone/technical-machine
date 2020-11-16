// Copyright (C) 2018 David Stone
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

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/at.hpp>
#include <containers/erase.hpp>
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
		BOUNDED_ASSERT(it != end(m_container));
		std::swap(*it, containers::at(m_container, index));
		return it;
	}
	Container m_container;
};

}	// namespace ps
}	// namespace technicalmachine
