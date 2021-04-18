// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>
#include <bounded/pow.hpp>
#include <bounded/detail/tuple.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <containers/front_back.hpp>
#include <containers/is_range.hpp>
#include <containers/size.hpp>

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
	constexpr auto max = bounded::pow(base, bounded::max_value<decltype(containers::size(range))>) - 1_bi;
	using result_t = bounded::integer<0, bounded::normalize<max>>;
	auto result = result_t(0_bi);
	for (auto const & value : range) {
		result = result_t(result.value() * base.value());
		result += compress(value);
	}
	return result;
}

namespace compress_detail {

constexpr auto operator->*(auto const & lhs, auto const & rhs) {
	auto const compressed_lhs = compress(lhs);
	auto const compressed_rhs = compress(rhs);
	return compressed_lhs * (bounded::max_value<decltype(compressed_rhs)> + 1_bi) + compressed_rhs;
}

} // namespace compress_detail

constexpr auto compress_combine(auto const & ... values) {
	using compress_detail::operator->*;
	return (values ->* ... ->* 0_bi);
}

} // namespace technicalmachine
