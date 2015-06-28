// Calculate hashes. Specializations should return a pair of integers
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

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace technicalmachine {
using namespace bounded::literal;


constexpr auto hash(bool const value) noexcept {
	return std::make_pair(BOUNDED_CONDITIONAL(value, 1_bi, 0_bi), 2_bi);
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::integer<minimum, maximum, policy> const value) noexcept {
	return std::make_pair(
		bounded::make<bounded::null_policy>(value) - bounded::make<minimum>(),
		bounded::make<maximum>() - bounded::make<minimum>() + 1_bi
	);
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::optional<bounded::integer<minimum, maximum, policy>> const value) noexcept {
	return std::make_pair(
		BOUNDED_CONDITIONAL(value, hash(*value).first + 1_bi, 0_bi),
		bounded::make<maximum>() - bounded::make<minimum>() + 1_bi + 1_bi
	);
}




template<typename T>
struct VerifyHashBoundsImpl;
template<typename hash_type, typename max_hash_type>
struct VerifyHashBoundsImpl<std::pair<hash_type, max_hash_type>> {
	static_assert(!std::is_same<hash_type, uint64_t>::value, "Mismatched hash types.");
	static_assert(!std::is_same<max_hash_type, uint64_t>::value, "Mismatched hash types.");
	static_assert(
		std::numeric_limits<hash_type>::min() == 0_bi,
		"Incorrect minimum for hash."
	);
	static_assert(
		std::numeric_limits<hash_type>::max() + 1_bi == std::numeric_limits<max_hash_type>::max(),
		"Incorrect maximum for hash."
	);
};
template<>
struct VerifyHashBoundsImpl<std::pair<uint64_t, uint64_t>> {
};
template<typename T>
struct VerifyHashBounds {
private:
	using type = VerifyHashBoundsImpl<decltype(hash(std::declval<T>()))>;
};



// VerifyHashBounds already checks these types.
template<typename LHS, typename RHS>
constexpr auto hash_combine(LHS const lhs, RHS const rhs) noexcept {
	// This loses the most significant bits. That will be fixed when
	// bounded::integer has bignum support.
	return std::make_pair(
		static_cast<uint64_t>(lhs.first) + static_cast<uint64_t>(lhs.second) * static_cast<uint64_t>(rhs.first),
		static_cast<uint64_t>(lhs.second) * static_cast<uint64_t>(rhs.second)
	);
}


// noexcept_hashable is used because recursion cannot appear in the noexcept
template<typename ... Ts>
constexpr auto noexcept_hashable() noexcept -> bool;



template<typename T, typename... Ts>
constexpr auto hash(T const & t, Ts && ... ts) noexcept(noexcept(hash(t)) and noexcept_hashable<Ts...>()) {
	static_cast<void>(VerifyHashBounds<T>{});
	return hash_combine(hash(t), hash(ts...));
}




template<typename... Ts>
struct NoexceptHashable;
template<typename T, typename... Ts>
struct NoexceptHashable<T, Ts...> : std::integral_constant<bool, NoexceptHashable<T>::value and NoexceptHashable<Ts...>::value> {};
template<typename T>
struct NoexceptHashable<T> : std::integral_constant<bool, noexcept(hash(std::declval<T>()))> {};

template<typename ... Ts>
constexpr auto noexcept_hashable() noexcept -> bool {
	return NoexceptHashable<Ts...>::value;
}




template<typename... Ts, std::size_t... indexes>
constexpr auto tuple_hash(std::tuple<Ts...> const & values, std::index_sequence<indexes...>) noexcept(noexcept_hashable<Ts...>()) {
	return hash(std::get<indexes>(values)...);
}
template<typename... Ts>
constexpr auto hash(std::tuple<Ts...> const & values) noexcept(noexcept_hashable<Ts...>()) {
	return tuple_hash(values, std::make_index_sequence<sizeof...(Ts)>{});
}




template<typename Size, typename Iterator>
auto hash_range(Iterator first, Iterator last) {
	constexpr auto initial = std::make_pair(static_cast<uint64_t>(0), static_cast<uint64_t>(0));
	return std::accumulate(first, last, initial, [](auto const current, auto const & element) {
		return hash_combine(current, hash(element));
	});
}

}	// namespace technicalmachine
