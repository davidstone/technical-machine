# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_strategy STATIC)
target_sources(tm_strategy PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		expectimax.cpp
		max_damage.cpp
		net_hp.cpp
		parse_strategy.cpp
		random_selection.cpp
		selection_probability.cpp
		statistical.cpp
		strategy.cpp
		weighted_selection.cpp
)
target_sources(tm_strategy PRIVATE
	expectimax_impl.cpp
	net_hp_impl.cpp
	statistical_impl.cpp
)
target_link_libraries(tm_strategy
	tm_common
	tm_evaluate
	strict_defaults
)

add_subdirectory(test)