// Calculate hashes. Specializations should return a bounded::integer
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

#ifndef HASH_HPP_
#define HASH_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

#include <tuple>
#include <type_traits>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto hash(bool const value) noexcept {
	return BOUNDED_CONDITIONAL(value, 1_bi, 0_bi);
}
constexpr auto max_hash(bool) noexcept {
	return 2_bi;
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::integer<minimum, maximum, policy> const value) noexcept {
	return bounded::make<bounded::null_policy>(value) - bounded::make<minimum>();
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto max_hash(bounded::integer<minimum, maximum, policy>) noexcept {
	return bounded::make<maximum>() - bounded::make<minimum>() + 1_bi;
}


template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::optional<bounded::integer<minimum, maximum, policy>> const value) noexcept {
	return BOUNDED_CONDITIONAL(value, hash(*value) + 1_bi, 0_bi);
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto max_hash(bounded::optional<bounded::integer<minimum, maximum, policy>>) noexcept {
	return bounded::make<maximum>() - bounded::make<minimum>() + 1_bi + 1_bi;
}

template<typename T>
class overload_hash_and_max_hash_for_your_type;
template<typename T>
constexpr auto hash(T const & t) {
	return overload_hash_and_max_hash_for_your_type<T>{};
}
template<typename T>
constexpr auto max_hash(T const & t) {
	return overload_hash_and_max_hash_for_your_type<T>{};
}

template<typename hash_type, typename max_hash_type>
class VerifyHashBoundsImpl {
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
class VerifyHashBoundsImpl<uint64_t, uint64_t> {
};
template<typename T>
class VerifyHashBounds {
	using hash_type = decltype(hash(std::declval<T>()));
	using max_hash_type = decltype(max_hash(std::declval<T>()));
	using type = VerifyHashBoundsImpl<hash_type, max_hash_type>;
};

template<typename ... Ts>
constexpr auto noexcept_hashable() noexcept -> bool;
template<typename ... Ts>
constexpr auto noexcept_max_hashable() noexcept -> bool;

template<typename T, typename... Ts>
constexpr auto hash(T const & t, Ts && ... ts) noexcept(noexcept(hash(t)) and noexcept(max_hash(t)) and noexcept_hashable<Ts...>()) {
	return VerifyHashBounds<T>{}, hash(t) + max_hash(t) * hash(ts...);
}

template<typename T, typename... Ts>
constexpr auto max_hash(T const & t, Ts && ... ts) noexcept(noexcept(max_hash(t)) and noexcept_max_hashable<Ts...>()) {
	return VerifyHashBounds<T>{}, max_hash(t) * max_hash(ts...);
}


// Workaround for lack of bignum support in bounded::integer
template<typename T>
constexpr auto big_hash(T const & t) noexcept(noexcept_hashable<T>()) {
	return static_cast<uint64_t>(hash(t));
}
template<typename T, typename... Ts>
constexpr auto big_hash(T const & t, Ts && ... ts) noexcept(noexcept_hashable<T, Ts...>()) {
	return static_cast<uint64_t>(hash(t)) + static_cast<uint64_t>(max_hash(t)) * static_cast<uint64_t>(big_hash(ts...));
}

template<typename T>
constexpr auto big_max_hash(T const & t) noexcept(noexcept_max_hashable<T>()) {
	return static_cast<uint64_t>(max_hash(t));
}
template<typename T, typename... Ts>
constexpr auto big_max_hash(T const & t, Ts && ... ts) noexcept(noexcept_max_hashable<T, Ts...>()) {
	return static_cast<uint64_t>(max_hash(t)) * static_cast<uint64_t>(big_max_hash(ts...));
}

template<typename... Ts>
struct NoexceptMaxHashable;
template<typename T, typename... Ts>
struct NoexceptMaxHashable<T, Ts...> : std::integral_constant<bool, NoexceptMaxHashable<T>::value and NoexceptMaxHashable<Ts...>::value> {};
template<typename T>
struct NoexceptMaxHashable<T> : std::integral_constant<bool, noexcept(max_hash(std::declval<T>()))> {};

template<typename ... Ts>
constexpr auto noexcept_max_hashable() noexcept -> bool {
	return NoexceptMaxHashable<Ts...>::value;
}

template<typename... Ts>
struct NoexceptHashable;
template<typename T, typename... Ts>
struct NoexceptHashable<T, Ts...> : std::integral_constant<bool, NoexceptHashable<T>::value and NoexceptMaxHashable<T>::value and NoexceptHashable<Ts...>::value> {};
template<typename T>
struct NoexceptHashable<T> : std::integral_constant<bool, noexcept(hash(std::declval<T>()))> {};

template<typename ... Ts>
constexpr auto noexcept_hashable() noexcept -> bool {
	return NoexceptHashable<Ts...>::value;
}




template<typename... Ts, std::size_t... indexes>
constexpr auto tuple_hash(std::tuple<Ts...> const & values, std::index_sequence<indexes...>) noexcept(noexcept_hashable<Ts...>()) {
	return big_hash(std::get<indexes>(values)...);
}
template<typename... Ts>
constexpr auto hash(std::tuple<Ts...> const & values) noexcept(noexcept_hashable<Ts...>()) {
	return tuple_hash(values, std::make_index_sequence<sizeof...(Ts)>{});
}
template<typename... Ts, std::size_t... indexes>
constexpr auto tuple_max_hash(std::tuple<Ts...> const & values, std::index_sequence<indexes...>) noexcept(noexcept_hashable<Ts...>()) {
	return big_max_hash(std::get<indexes>(values)...);
}
template<typename... Ts>
constexpr auto max_hash(std::tuple<Ts...> const & values) noexcept(noexcept_hashable<Ts...>()) {
	return tuple_max_hash(values, std::make_index_sequence<sizeof...(Ts)>{});
}





// TODO: change when bounded::integer supports large integers
template<typename Size, typename Iterator>
auto max_hash_range(Iterator first, Iterator last) {
	constexpr uint64_t initial = 1;
	return std::accumulate(first, last, initial, [](auto const current, auto const & element) {
		return static_cast<uint64_t>(current) * static_cast<uint64_t>(max_hash(element));
	});
}

template<typename Size, typename Iterator>
auto hash_range(Iterator first, Iterator last) {
	constexpr uint64_t initial = 0;
	return std::accumulate(first, last, initial, [](auto const current, auto const & element) {
		return static_cast<uint64_t>(current) * static_cast<uint64_t>(max_hash(element)) + static_cast<uint64_t>(hash(element));
	});
}

}	// namespace technicalmachine
#endif	// HASH_HPP_
