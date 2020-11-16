// Copyright (C) 2020 David Stone
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

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/detail/tuple.hpp>
#include <bounded/detail/variant/variant.hpp>
#include <containers/front_back.hpp>
#include <containers/is_range.hpp>
#include <tm/generation.hpp>

namespace technicalmachine {

using namespace bounded::literal;

template<typename T>
concept explicitly_convertible_to_integer = requires {
	requires !std::is_same_v<decltype(bounded::min_value<T>), bounded::detail::incomplete>;
	requires !std::is_same_v<decltype(bounded::max_value<T>), bounded::detail::incomplete>;
};

// Returns an integer with a min value of 0.
constexpr auto compress(explicitly_convertible_to_integer auto const value) {
	auto const temp = bounded::integer(value);
	return temp - bounded::min_value<decltype(temp)>;
}

template<typename T>
constexpr auto compress(bounded::optional<T> const & value) {
	return BOUNDED_CONDITIONAL(value, compress(*value) + 1_bi, 0_bi);
}

template<typename... Ts>
constexpr auto compress(bounded::variant<Ts...> const & variant) {
	using common = std::common_type_t<decltype(compress(std::declval<Ts>()))...>;
	auto const index = variant.index();
	return bounded::visit(variant, [=](auto const & value) {
		return index + (bounded::max_value<decltype(index)> + 1_bi) * common(compress(value));
	});
}

constexpr auto compress(containers::range auto const & range) {
	using single_value = decltype(compress(containers::front(range)));
	static_assert(bounded::min_value<single_value> == 0_bi);
	constexpr auto base = bounded::max_value<single_value> + 1_bi;
	constexpr auto max = bounded::pow(base, bounded::max_value<decltype(size(range))>) - 1_bi;
	using result_t = bounded::integer<0, bounded::detail::normalize<max.value()>>;
	auto result = result_t(0_bi);
	for (auto const & value : range) {
		result = result_t(result.value() * base.value());
		result += compress(value);
	}
	return result;
}

constexpr auto compress_combine(auto const & value, auto const & ... values) {
	auto const lhs = compress(value);
	static_assert(bounded::bounded_integer<decltype(lhs)>);
	static_assert(bounded::min_value<decltype(lhs)> == 0_bi);
	if constexpr (sizeof...(values) == 0) {
		return lhs;
	} else {
		auto const rhs = compress_combine(values...);
		static_assert(bounded::min_value<decltype(rhs)> == 0_bi);
		return lhs * (bounded::max_value<decltype(rhs)> + 1_bi) + rhs;
	}
}

} // namespace technicalmachine
