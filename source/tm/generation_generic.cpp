export module tm.generation_generic;

import tm.generation;

import bounded;
import numeric_traits;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

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

static_assert(numeric_traits::max_value<Generation> == Generation::eight);

export template<template<Generation> typename T>
constexpr auto get_generation(GenerationGeneric<T> const & variant) {
	return Generation(variant.index().integer() + 1_bi);
}

} // namespace technicalmachine