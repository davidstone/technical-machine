// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/compress.hpp>

#include <containers/array/array.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {
namespace {

using bounded::integer;
using bounded::detail::int128_t;
using bounded::detail::uint128_t;

template<typename T>
constexpr bool equal(T const lhs, T const rhs) {
	return lhs == rhs;
}

static_assert(equal(compress(false), integer<0, 1>(0)));
static_assert(equal(compress(true), integer<0, 1>(1)));

static_assert(equal(compress_combine(false, false), integer<0, 3>(0)));
static_assert(equal(compress_combine(false, true), integer<0, 3>(1)));
static_assert(equal(compress_combine(true, false), integer<0, 3>(2)));
static_assert(equal(compress_combine(true, true), integer<0, 3>(3)));

static_assert(equal(compress_combine(integer<0, 9>(4), integer<0, 9>(6)), integer<0, 99>(46)));

static_assert(equal(
	compress_combine(
		integer<0, 9>(1),
		integer<0, 99>(29)
	),
	integer<0, 999>(129)
));

static_assert(equal(
	compress_combine(
		integer<0, 4>(1),
		integer<0, 1>(1)
	),
	integer<0, 9>(3)
));

static_assert(equal(
	compress_combine(
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255)
	),
	integer(numeric_traits::max_value<std::uint64_t>)
));

static_assert(equal(
	compress_combine(
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255),
		integer<0, 255>(255)
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
	integer<0, 999>(0)
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
	integer<0, int128_t(1'000'000'000) * int128_t(1'000'000'000) * int128_t(1'000'000'000) - int128_t(1)>(0)
));

static_assert(equal(
	compress(containers::array{integer<0, 9>(9), integer<0, 9>(9), integer<0, 9>(9)}),
	integer<0, 999>(999)
));

} // namespace
} // namespace technicalmachine