// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <numeric_traits/incomplete.hpp>
#include <numeric_traits/min_max_value.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Generation : std::uint8_t {
	one = 1,
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
	seven = 7,
	eight = 8,
};

template<typename T>
extern numeric_traits::incomplete generation_from;

template<typename T>
inline constexpr auto generation_from<T const> = generation_from<T>;

template<template<Generation> typename T, Generation generation>
inline constexpr auto generation_from<T<generation>> = generation;

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::Generation> = technicalmachine::Generation::one;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::Generation> = technicalmachine::Generation::eight;
