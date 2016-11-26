// Allows safe access to the active Pokemon / move
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

#pragma once

#include "operators.hpp"
#include "range.hpp"

#include <bounded/integer.hpp>

#include <containers/common_container_functions.hpp>
#include <containers/index_type.hpp>

#include <stdexcept>
#include <string>

namespace technicalmachine {
using namespace bounded::literal;

struct Move;
struct Pokemon;

struct InvalidCollectionIndex final : std::out_of_range {
	template<typename Index, typename Size>
	InvalidCollectionIndex(Index const index, Size const size, std::string const & name):
		out_of_range("Attempted to access element " + bounded::to_string(index) + " in a container of size " + bounded::to_string(size) + " with elements of type " + name + "\n")
		{
	}
};

namespace detail {

template<typename Container>
struct Collection : protected Container {
	using typename Container::value_type;
	using typename Container::size_type;
	using typename Container::const_iterator;

	using Container::Container;

	friend decltype(auto) begin(Collection const & collection) BOUNDED_NOEXCEPT(
		begin(static_cast<Container const &>(collection))
	)
	friend decltype(auto) begin(Collection & collection) BOUNDED_NOEXCEPT(
		begin(static_cast<Container &>(collection))
	)
	friend decltype(auto) end(Collection const & collection) BOUNDED_NOEXCEPT(
		end(static_cast<Container const &>(collection))
	)
	friend decltype(auto) end(Collection & collection) BOUNDED_NOEXCEPT(
		end(static_cast<Container &>(collection))
	)
	
	constexpr decltype(auto) operator()(containers::index_type<Collection> const specified_index) const {
		return operator[](check_range(specified_index));
	}
	constexpr decltype(auto) operator()() const {
		return operator[](index());
	}

	void set_index(containers::index_type<Collection> const new_index) {
		m_current_index = check_range(new_index);
	}
	void reset_index() {
		m_current_index = 0_bi;
	}
	constexpr auto index() const {
		return m_current_index;
	}
protected:
	constexpr auto check_range(containers::index_type<Collection> const new_index) const {
		return (new_index < containers::size(*this)) ?
			new_index :
			throw InvalidCollectionIndex(new_index, containers::size(*this), value_type::class_name);
	}
	using Container::operator[];
private:
	containers::index_type<Collection> m_current_index = 0_bi;
};


CONTAINERS_COMMON_USING_DECLARATIONS

}	// namespace detail

CONTAINERS_COMMON_USING_DECLARATIONS

}	// namespace technicalmachine
