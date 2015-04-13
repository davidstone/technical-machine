// Basic container for Pokemon that helps with bounds checking
// Copyright (C) 2015 David Stone
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

#include "container.hpp"

#include <algorithm>
#include <cassert>

namespace technicalmachine {

auto PokemonContainer::operator[](index_type const index) const -> Pokemon const & {
	assert(index < size());
	return *m_container[index];
}

auto PokemonContainer::erase(iterator it) -> void {
	// The iterator passed in is valid, so it is before the first boost::none
	// element. Skip scanning anything before it.
	auto const e = bounded::find(it.m_it, m_container.end(), boost::none);
	assert(it.m_it != e);
	auto const to_clear = std::move(bounded::next(it.m_it), e, it.m_it);
	*to_clear = boost::none;
}

auto PokemonContainer::base_end() const -> container_type::const_iterator {
	return bounded::find(m_container.begin(), m_container.end(), boost::none);
}

auto PokemonContainer::base_end() -> container_type::iterator {
	return bounded::find(m_container.begin(), m_container.end(), boost::none);
}

auto operator==(PokemonContainer const & lhs, PokemonContainer const & rhs) -> bool {
	return lhs.m_container == rhs.m_container;
}

}	// namespace technicalmachine
