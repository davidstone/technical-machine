#pragma once

import tm.generation;

import numeric_traits;

namespace technicalmachine {

static_assert(numeric_traits::max_value<Generation> == Generation::eight);

#define TM_FOR_EACH_GENERATION(function) \
	function(Generation::one); \
	function(Generation::two); \
	function(Generation::three); \
	function(Generation::four); \
	function(Generation::five); \
	function(Generation::six); \
	function(Generation::seven); \
	function(Generation::eight)

} // namespace technicalmachine