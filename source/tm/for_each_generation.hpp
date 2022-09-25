// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

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

} // namespace technicalmachine