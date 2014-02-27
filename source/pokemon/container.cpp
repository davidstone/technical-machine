// Basic container for Pokemon that helps with bounds checking
// Copyright (C) 2014 David Stone
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
#include <cassert>

namespace technicalmachine {

auto PokemonContainer::operator[](index_type const index) const -> Pokemon const & {
	assert(index < size());
	return m_container[static_cast<container_type::size_type>(index)];
}

auto PokemonContainer::size() const -> size_type {
	return static_cast<size_type>(m_container.size());
}

auto PokemonContainer::empty() const -> bool {
	return m_container.empty();
}

auto PokemonContainer::begin() const -> const_iterator {
	return m_container.begin();
}
auto PokemonContainer::begin() -> iterator {
	return m_container.begin();
}
auto PokemonContainer::end() const -> const_iterator {
	return m_container.end();
}
auto PokemonContainer::end() -> iterator {
	return m_container.end();
}

void PokemonContainer::erase(iterator it) {
	m_container.erase(it);
}


bool operator==(PokemonContainer const & lhs, PokemonContainer const & rhs) {
	return lhs.m_container == rhs.m_container;
}

}	// namespace technicalmachine
