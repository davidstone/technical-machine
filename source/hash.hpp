// Calculate hashes
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

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::integer<minimum, maximum, policy> const & value) noexcept {
	return bounded::make<bounded::null_policy>(value) - bounded::make<minimum>();
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto max_hash(bounded::integer<minimum, maximum, policy> const &) noexcept {
	return bounded::make<maximum>() - bounded::make<minimum>() + 1_bi;
}


template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto hash(bounded::optional<bounded::integer<minimum, maximum, policy>> const & value) noexcept {
	return BOUNDED_CONDITIONAL(value, hash(*value) + 1_bi, 0_bi);
}

template<intmax_t minimum, intmax_t maximum, typename policy>
constexpr auto max_hash(bounded::optional<bounded::integer<minimum, maximum, policy>> const &) noexcept {
	return bounded::make<maximum>() - bounded::make<minimum>() + 1_bi + 1_bi;
}

template<typename T, typename... Ts>
constexpr auto hash(T const & t, Ts && ... ts) noexcept(noexcept(hash(t) + max_hash(t) * hash(ts...))) {
	return hash(t) + max_hash(t) * hash(ts...);
}

template<typename T, typename... Ts>
constexpr auto max_hash(T const & t, Ts && ... ts) noexcept(noexcept(max_hash(t) * max_hash(ts...))) {
	return max_hash(t) * max_hash(ts...);
}

template<typename Size, typename Iterator>
auto hash_range(Iterator first, Iterator last) {
	using single_type = decltype(hash(std::declval<decltype(*first)>()));
	using single_max_type = decltype(max_hash(std::declval<decltype(*first)>()));
	using hash_type = decltype(std::declval<single_type>() * std::declval<single_max_type>() * (std::declval<Size>() - 1_bi) + std::declval<single_type>());
	static constexpr hash_type initial(0_bi);
	return std::accumulate(std::move(first), std::move(last), initial, [](auto const & current, auto const & element) {
		return current * max_hash(element) + hash(element);
	});
}

template<typename Size, typename Iterator, enable_if_t<std::numeric_limits<Size>::min() == 0_bi> = clang_dummy>
auto max_hash_range(Iterator first, Iterator last) {
	using hash_type = decltype(max_hash(std::declval<decltype(*first)>()) * std::declval<Size>());
	static constexpr hash_type initial(1_bi);
	return std::accumulate(std::move(first), std::move(last), initial, [](auto const & current, auto const & element) {
		return current * max_hash(element);
	});
}

// Cannot use 0_bi because it causes an internal compiler error in gcc 4.9.0
template<typename Size, typename Iterator, enable_if_t<(std::numeric_limits<Size>::min() > 0)> = clang_dummy>
auto max_hash_range(Iterator first, Iterator last) {
	using individual_type = decltype(max_hash(*first));
	using result_type = decltype(std::declval<individual_type>() * std::declval<Size>());
	std::common_type_t<result_type, individual_type> const initial = max_hash(*first);
	auto const result = std::accumulate(bounded::next(first), std::move(last), initial, [](auto const & current, auto const & element) {
		return current * max_hash(element);
	});
	return static_cast<result_type>(result);
}

}	// namespace technicalmachine
#endif	// HASH_HPP_
