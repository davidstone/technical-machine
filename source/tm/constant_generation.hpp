// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {

template<Generation generation>
using constant_gen_t = std::integral_constant<Generation, generation>;

template<Generation generation>
inline constexpr auto constant_gen = constant_gen_t<generation>();

constexpr auto constant_generation(Generation const generation, auto && make) {
	switch (generation) {
		case Generation::one: return make(constant_gen<Generation::one>);
		case Generation::two: return make(constant_gen<Generation::two>);
		case Generation::three: return make(constant_gen<Generation::three>);
		case Generation::four: return make(constant_gen<Generation::four>);
		case Generation::five: return make(constant_gen<Generation::five>);
		case Generation::six: return make(constant_gen<Generation::six>);
		case Generation::seven: return make(constant_gen<Generation::seven>);
		case Generation::eight: return make(constant_gen<Generation::eight>);
	}
}

} // namespace technicalmachine
