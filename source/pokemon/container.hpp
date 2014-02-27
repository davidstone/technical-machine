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

#ifndef POKEMON__CONTAINER_HPP_
#define POKEMON__CONTAINER_HPP_

#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include <algorithm>
#include <vector>

namespace technicalmachine {

class PokemonContainer {
private:
	using container_type = std::vector<Pokemon>;
public:
	using value_type = Pokemon;
	using size_type = TeamSize;
	using index_type = TeamIndex;
	using const_iterator = container_type::const_iterator;
	using iterator = container_type::iterator;

	Pokemon const & operator[](index_type index) const;

	size_type size() const;
	bool empty() const;
	const_iterator begin() const;
	iterator begin();
	const_iterator end() const;
	iterator end();
	void erase(iterator it);

	template<class... Args>
	void emplace_back(Args&&... args) {
		m_container.emplace_back(std::forward<Args>(args)...);
	}
	friend bool operator==(PokemonContainer const & lhs, PokemonContainer const & rhs);
private:
	std::vector<Pokemon> m_container;
};

}	// namespace technicalmachine
#endif	// POKEMON__CONTAINER_HPP_
