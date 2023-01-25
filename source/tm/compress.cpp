// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.compress;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {

using namespace bounded::literal;

template<typename T>
concept explicitly_convertible_to_integer = numeric_traits::has_min_value<T> and numeric_traits::has_max_value<T>;

// Returns an integer with a min value of 0.
export constexpr auto compress(explicitly_convertible_to_integer auto const value) {
	auto const temp = bounded::integer(value);
	return temp - numeric_traits::min_value<decltype(temp)>;
}

export template<typename T>
constexpr auto compress(tv::optional<T> const & value) {
	return BOUNDED_CONDITIONAL(value, compress(*value) + 1_bi, 0_bi);
}

export template<typename... Ts>
constexpr auto compress(tv::variant<Ts...> const & variant) {
	using common = std::common_type_t<decltype(compress(bounded::declval<Ts>()))...>;
	auto const index = variant.index().integer();
	return tv::visit(variant, [=](auto const & value) {
		return index + (numeric_traits::max_value<decltype(index)> + 1_bi) * common(compress(value));
	});
}

export constexpr auto compress(containers::range auto const & range) {
	using single_value = decltype(compress(containers::front(range)));
	static_assert(numeric_traits::min_value<single_value> == 0_bi);
	constexpr auto base = numeric_traits::max_value<single_value> + 1_bi;
	constexpr auto max = bounded::pow(base, numeric_traits::max_value<decltype(containers::size(range))>) - 1_bi;
	using result_t = bounded::integer<0, bounded::normalize<max>>;
	auto result = result_t(0_bi);
	for (auto const & value : range) {
		result = ::bounded::assume_in_range<result_t>(result.value() * base.value());
		result += compress(value);
	}
	return result;
}

namespace compress_detail {

constexpr auto operator->*(auto const & lhs, auto const & rhs) {
	auto const compressed_lhs = compress(lhs);
	auto const compressed_rhs = compress(rhs);
	return compressed_lhs * (numeric_traits::max_value<decltype(compressed_rhs)> + 1_bi) + compressed_rhs;
}

} // namespace compress_detail

export constexpr auto compress_combine(auto const & ... values) {
	using compress_detail::operator->*;
	return (values ->* ... ->* 0_bi);
}

namespace {

using bounded::integer;
using numeric_traits::int128_t;
using numeric_traits::uint128_t;

template<typename T>
constexpr bool equal(T const lhs, T const rhs) {
	return lhs == rhs;
}

static_assert(equal(compress(false), integer<0, 1>(0_bi)));
static_assert(equal(compress(true), integer<0, 1>(1_bi)));

static_assert(equal(compress_combine(false, false), integer<0, 3>(0_bi)));
static_assert(equal(compress_combine(false, true), integer<0, 3>(1_bi)));
static_assert(equal(compress_combine(true, false), integer<0, 3>(2_bi)));
static_assert(equal(compress_combine(true, true), integer<0, 3>(3_bi)));

static_assert(equal(compress_combine(integer<0, 9>(4_bi), integer<0, 9>(6_bi)), integer<0, 99>(46_bi)));

static_assert(equal(
	compress_combine(
		integer<0, 9>(1_bi),
		integer<0, 99>(29_bi)
	),
	integer<0, 999>(129_bi)
));

static_assert(equal(
	compress_combine(
		integer<0, 4>(1_bi),
		integer<0, 1>(1_bi)
	),
	integer<0, 9>(3_bi)
));

static_assert(equal(
	compress_combine(
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi)
	),
	integer(numeric_traits::max_value<std::uint64_t>)
));

static_assert(equal(
	compress_combine(
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi),
		integer<0, 255>(255_bi)
	),
	integer(numeric_traits::max_value<uint128_t>)
));

enum class Foo {
	a = 0,
	b = 999
};

} // namespace
} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::Foo> = technicalmachine::Foo::a;
template<>
constexpr auto numeric_traits::max_value<technicalmachine::Foo> = technicalmachine::Foo::b;

namespace technicalmachine {
namespace {

static_assert(equal(
	compress(Foo::a),
	integer<0, 999>(0_bi)
));

static_assert(equal(
	compress_combine(
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a,
		Foo::a
	),
	integer<0, int128_t(1'000'000'000) * int128_t(1'000'000'000) * int128_t(1'000'000'000) - int128_t(1)>(0_bi)
));

static_assert(equal(
	compress(containers::array{integer<0, 9>(9_bi), integer<0, 9>(9_bi), integer<0, 9>(9_bi)}),
	integer<0, 999>(999_bi)
));

} // namespace
} // namespace technicalmachine