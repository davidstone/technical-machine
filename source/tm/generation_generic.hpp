// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <bounded/detail/variant/variant.hpp>

namespace technicalmachine {

static_assert(numeric_traits::max_value<Generation> == Generation::eight);

template<template<Generation> typename T>
using GenerationGeneric = bounded::variant<
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
