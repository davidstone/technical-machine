// std::vector-like interface around a bounded::array
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

#ifndef TECHNICALMACHINE_FIXED_CAPACITY_VECTOR_HPP_
#define TECHNICALMACHINE_FIXED_CAPACITY_VECTOR_HPP_

#include <bounded_integer/array.hpp>
#include <boost/optional.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

template<typename T, std::size_t capacity>
struct fixed_capacity_vector;

template<typename T, std::size_t capacity>
struct fixed_capacity_vector_iterator {
private:
	friend struct fixed_capacity_vector<std::remove_const_t<T>, capacity>;
	
	using container = bounded::array<boost::optional<std::remove_const_t<T>>, capacity>;
	using base_iterator = std::conditional_t<std::is_const<T>::value, typename container::const_iterator, typename container::iterator>;

public:
	using iterator_category = typename base_iterator::iterator_category;
	using value_type = typename base_iterator::value_type;
	using difference_type = typename base_iterator::difference_type;
	using pointer = typename base_iterator::pointer;
	using reference = typename base_iterator::reference;

	constexpr decltype(auto) operator*() const {
		return **m_it;
	}
	constexpr decltype(auto) operator->() const {
		return &operator*();
	}
	
	auto & operator++() {
		++m_it;
		return *this;
	}
	auto operator++(int) {
		auto temp = *this;
		++m_it;
		return temp;
	}
	template<typename Offset>
	friend constexpr auto operator+(fixed_capacity_vector_iterator const it, Offset const & offset) -> fixed_capacity_vector_iterator {
		return fixed_capacity_vector_iterator(it.m_it + offset);
	}
	friend constexpr auto operator-(fixed_capacity_vector_iterator const lhs, fixed_capacity_vector_iterator const rhs) -> difference_type {
		return lhs.m_it - rhs.m_it;
	}

	friend constexpr auto operator==(fixed_capacity_vector_iterator const lhs, fixed_capacity_vector_iterator const rhs) -> bool {
		return lhs.m_it == rhs.m_it;
	}
	friend constexpr auto operator<(fixed_capacity_vector_iterator const lhs, fixed_capacity_vector_iterator const rhs) -> bool {
		return lhs.m_it < rhs.m_it;
	}
	
	// Convert iterator to const_iterator
	constexpr operator fixed_capacity_vector_iterator<T const, capacity>() const {
		return fixed_capacity_vector_iterator<T const, capacity>(m_it);
	}
	
private:
	constexpr explicit fixed_capacity_vector_iterator(base_iterator const it):
		m_it(it) {
	}

	base_iterator m_it;
};

template<typename T, std::size_t capacity, typename Offset>
constexpr auto operator-(fixed_capacity_vector_iterator<T, capacity> const it, Offset const & offset) {
	return it + -offset;
}

template<typename T, std::size_t capacity, typename Offset>
auto operator+=(fixed_capacity_vector_iterator<T, capacity> & it, Offset const & offset) {
	return it = it + offset;
}

template<typename T, std::size_t capacity>
constexpr auto operator!=(fixed_capacity_vector_iterator<T, capacity> const lhs, fixed_capacity_vector_iterator<T, capacity> const rhs) {
	return !(lhs == rhs);
}


template<typename T, std::size_t capacity>
struct fixed_capacity_vector {
private:
	using container_type = bounded::array<boost::optional<T>, capacity>;

public:
	using const_iterator = fixed_capacity_vector_iterator<T const, capacity>;
	using iterator = fixed_capacity_vector_iterator<T, capacity>;
	using index_type = typename container_type::index_type;

	constexpr decltype(auto) operator[](index_type index) const {
		return *m_container[index];
	}
	decltype(auto) operator[](index_type index) {
		return *m_container[index];
	}

	constexpr auto size() const {
		return static_cast<typename container_type::size_type>(base_end() - m_container.begin());
	}
	constexpr auto empty() const {
		return size() == 0_bi;
	}

	constexpr auto begin() const {
		return const_iterator(m_container.begin());
	}
	auto begin() {
		return iterator(m_container.begin());
	}
	constexpr auto end() const {
		return const_iterator(base_end());
	}
	auto end() {
		return iterator(base_end());
	}

	auto erase(iterator it) {
		// The iterator passed in is valid, so it is before the first
		// boost::none element. Skip scanning anything before it.
		auto const e = bounded::find(it.m_it, m_container.end(), boost::none);
		assert(it.m_it != e);
		auto const to_clear = std::move(bounded::next(it.m_it), e, it.m_it);
		*to_clear = boost::none;
	}

	template<class... Args>
	auto emplace_back(Args&&... args) {
		auto const it = base_end();
		assert(it != m_container.end());
		it->emplace(std::forward<Args>(args)...);
	}
	friend constexpr auto operator==(fixed_capacity_vector const & lhs, fixed_capacity_vector const & rhs) -> bool {
		return lhs.m_container == rhs.m_container;
	}
private:
	constexpr auto base_end() const {
		return bounded::find(m_container.begin(), m_container.end(), boost::none);
	}
	auto base_end() {
		return bounded::find(m_container.begin(), m_container.end(), boost::none);
	}
	container_type m_container;
};

}	// namespace technicalmachine
#endif	// TECHNICALMACHINE_FIXED_CAPACITY_VECTOR_HPP_
