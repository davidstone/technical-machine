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

template<typename T>
using aligned_t = std::aligned_storage_t<sizeof(T), alignof(T)>;

template<typename T, std::size_t capacity>
struct fixed_capacity_vector;

template<typename T, std::size_t capacity>
struct fixed_capacity_vector_iterator {
private:
	friend struct fixed_capacity_vector<std::remove_const_t<T>, capacity>;
	
	using container = bounded::array<aligned_t<std::remove_const_t<T>>, capacity>;
	using base_iterator = std::conditional_t<std::is_const<T>::value, typename container::const_iterator, typename container::iterator>;

public:
	using iterator_category = typename base_iterator::iterator_category;
	using value_type = T;
	using difference_type = typename base_iterator::difference_type;
	using pointer = T *;
	using reference = T &;

	constexpr decltype(auto) operator*() const {
		using void_star = std::conditional_t<std::is_const<T>::value, void const *, void *>;
		return *static_cast<T *>(static_cast<void_star>(&*m_it));
	}
	constexpr decltype(auto) operator->() const {
		return &operator*();
	}
	
	auto & operator++() {
		++m_it;
		return *this;
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

template<typename T, std::size_t capacity>
auto operator++(fixed_capacity_vector<T, capacity> it, int) {
	auto temp = it;
	++it;
	return temp;
}

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
	using container_type = bounded::array<aligned_t<T>, capacity>;
public:
	using const_iterator = fixed_capacity_vector_iterator<T const, capacity>;
	using iterator = fixed_capacity_vector_iterator<T, capacity>;
	using index_type = typename container_type::index_type;
	
	constexpr fixed_capacity_vector() = default;
	fixed_capacity_vector(fixed_capacity_vector const & other) noexcept(std::is_nothrow_copy_constructible<T>::value) {
		for (auto const & value : other) {
			emplace_back(value);
		}
	}
	fixed_capacity_vector(fixed_capacity_vector && other) noexcept(std::is_nothrow_move_constructible<T>::value) {
		for (auto && value : other) {
			emplace_back(std::move(value));
		}
	}
	fixed_capacity_vector & operator=(fixed_capacity_vector const & other) & noexcept(std::is_nothrow_copy_assignable<T>::value) {
		assign(other.begin(), other.end());
		return *this;
	}
	fixed_capacity_vector & operator=(fixed_capacity_vector && other) & noexcept(std::is_nothrow_move_assignable<T>::value) {
		assign(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()));
		return *this;
	}
	~fixed_capacity_vector() {
		for (auto & value : *this) {
			value.~T();
		}
	}

	constexpr decltype(auto) operator[](index_type index) const {
		return *(begin() + index);
	}
	decltype(auto) operator[](index_type index) {
		return *(begin() + index);
	}

	constexpr auto size() const {
		return m_size;
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
		return begin() + m_size;
	}
	auto end() {
		return begin() + m_size;
	}

	auto erase(iterator it) {
		assert(it != end());
		auto const to_clear = std::move(bounded::next(it), end(), it);
		to_clear->~T();
		--m_size;
	}

	template<typename... Args>
	auto emplace_back(Args && ... args) {
		assert(m_size != bounded::make<capacity>());
		new(m_container.data() + m_size) T(std::forward<Args>(args)...);
		++m_size;
	}
private:
	template<typename InputIterator>
	void assign(InputIterator first, InputIterator const last) {
		std::copy_n(first, size(), begin());
		std::advance(first, size());
		for (; first != last; ++first) {
			emplace_back(*first);
		}
	}
	container_type m_container;
	bounded::integer<0, capacity> m_size = 0_bi;
};

template<typename T, std::size_t capacity>
constexpr auto operator==(fixed_capacity_vector<T, capacity> const & lhs, fixed_capacity_vector<T, capacity> const & rhs) {
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

}	// namespace technicalmachine
#endif	// TECHNICALMACHINE_FIXED_CAPACITY_VECTOR_HPP_
