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

template<typename Size, typename Iterator>
class range_result {
private:
	using max_hash_type = decltype(max_hash(*std::declval<Iterator>()));
	using size_type = std::conditional_t<std::is_same<max_hash_type, uint64_t>::value, uint64_t, Size>;
public:
	using type = decltype(std::declval<max_hash_type>() * std::declval<size_type>());
}; 

// Cannot use 0_bi because it causes an internal compiler error in gcc 4.9.0
template<typename Size, typename Iterator, enable_if_t<std::numeric_limits<Size>::min() == 0> = clang_dummy>
auto max_hash_range(Iterator first, Iterator last) {
	using result_type = typename range_result<Size, Iterator>::type;
	static constexpr result_type initial(1_bi);
	return std::accumulate(first, last, initial, [](auto const current, auto const & element) {
		return current * max_hash(element);
	});
}

// Cannot use 0_bi because it causes an internal compiler error in gcc 4.9.0
template<typename Size, typename Iterator, enable_if_t<(std::numeric_limits<Size>::min() > 0)> = clang_dummy>
auto max_hash_range(Iterator first, Iterator last) {
	using individual_type = decltype(max_hash(*first));
	using result_type = typename range_result<Size, Iterator>::type;
	std::common_type_t<result_type, individual_type> const initial = max_hash(*first);
	return static_cast<result_type>(std::accumulate(bounded::next(first), last, initial, [](auto const current, auto const & element) {
		return current * max_hash(element);
	}));
}


namespace hash_detail {

template<typename T, intmax_t size>
class range_bounds {
public:
	static_assert(size > 1, "Incorrect size.");
	using type = decltype(hash(std::declval<T>()) + max_hash(std::declval<T>()) * std::declval<typename range_bounds<T, size - 1>::type>());
};
template<typename T>
class range_bounds<T, 1> {
public:
	using type = decltype(hash(std::declval<T>()));
};
template<typename T, intmax_t size>
using range_bounds_t = typename range_bounds<T, size>::type;

constexpr auto max_range_bound(intmax_t const value, intmax_t const counter) noexcept -> intmax_t {
	return
		(counter == 0) ? 0 :
		(counter == 1) ? value :
		value * max_range_bound(value, counter - 1);
}

template<typename T, typename max_hash_type, intmax_t min_size, intmax_t max_size>
class max_range_bounds_impl {
private:
	static constexpr auto min = max_range_bound(static_cast<intmax_t>(std::numeric_limits<max_hash_type>::min()), min_size);
	static constexpr auto max = max_range_bound(static_cast<intmax_t>(std::numeric_limits<max_hash_type>::max()), max_size);
public:
	using type = bounded::integer<min, max>;
};
template<typename T, intmax_t min_size, intmax_t max_size>
class max_range_bounds_impl<T, uint64_t, min_size, max_size> {
public:
	using type = uint64_t;
};
template<typename T, intmax_t min_size, intmax_t max_size>
class max_range_bounds {
private:
	static_assert(min_size <= max_size, "Incorrect range.");
	static_assert(min_size >= 0, "Incorrect size.");
	using max_hash_type = decltype(max_hash(std::declval<T>()));
public:
	using type = typename max_range_bounds_impl<T, max_hash_type, min_size, max_size>::type;
};
template<typename T, intmax_t min_size, intmax_t max_size>
using max_range_bounds_t = typename max_range_bounds<T, min_size, max_size>::type;

}	// namespace hash_detail

template<typename Size, typename Iterator>
auto hash_range(Iterator first, Iterator last) {
	using T = decltype(*std::declval<Iterator>());
	using hash_type = hash_detail::range_bounds_t<T, static_cast<intmax_t>(std::numeric_limits<Size>::max())>;
	using max_hash_type = hash_detail::max_range_bounds_t<T, static_cast<intmax_t>(std::numeric_limits<Size>::min()), static_cast<intmax_t>(std::numeric_limits<Size>::max())>;

	VerifyHashBoundsImpl<hash_type, max_hash_type>{};

	static constexpr hash_type initial(0_bi);
	return std::accumulate(std::move(first), std::move(last), initial, [](auto const current, auto const & element) {
		// TODO: change when bounded::integer supports large integers
		return static_cast<hash_type>(static_cast<uint64_t>(current) * static_cast<uint64_t>(max_hash(element)) + static_cast<uint64_t>(hash(element)));
	});
}

}	// namespace technicalmachine
#endif	// HASH_HPP_
