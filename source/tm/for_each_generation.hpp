// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TM_FOR_EACH_GENERATION_HPP
#define TM_FOR_EACH_GENERATION_HPP

import tm.generation;

import numeric_traits;

namespace technicalmachine {

static_assert(numeric_traits::max_value<Generation> == Generation::eight);

#define TECHNICALMACHINE_FOR_EACH_GENERATION(function) \
	function(Generation::one); \
	function(Generation::two); \
	function(Generation::three); \
	function(Generation::four); \
	function(Generation::five); \
	function(Generation::six); \
	function(Generation::seven); \
	function(Generation::eight)

#define TM_INSTANTIATE_STRUCT_FOR_EACH_GENERATION(T) \
	template struct T<Generation::one>; \
	template struct T<Generation::two>; \
	template struct T<Generation::three>; \
	template struct T<Generation::four>; \
	template struct T<Generation::five>; \
	template struct T<Generation::six>; \
	template struct T<Generation::seven>; \
	template struct T<Generation::eight>

} // namespace technicalmachine

#endif // TM_FOR_EACH_GENERATION_HPP