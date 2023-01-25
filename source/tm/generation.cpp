// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.generation;

import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {

export enum class Generation : std::uint8_t {
	one = 1,
	two = 2,
	three = 3,
	four = 4,
	five = 5,
	six = 6,
	seven = 7,
	eight = 8,
};

export template<typename T>
extern numeric_traits::incomplete generation_from;

template<typename T>
constexpr auto generation_from<T const> = generation_from<T>;

template<template<Generation> typename T, Generation generation>
constexpr auto generation_from<T<generation>> = generation;

export template<template<Generation> typename T>
using GenerationGeneric = tv::variant<
	T<Generation::one>,
	T<Generation::two>,
	T<Generation::three>,
	T<Generation::four>,
	T<Generation::five>,
	T<Generation::six>,
	T<Generation::seven>,
	T<Generation::eight>
>;

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::Generation> = technicalmachine::Generation::one;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::Generation> = technicalmachine::Generation::eight;
