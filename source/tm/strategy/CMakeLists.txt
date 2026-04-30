# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_strategy_common STATIC)
target_sources(tm_strategy_common PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		selection_probability.cpp
		strategy.cpp
		weighted_selection.cpp
)
target_link_libraries(tm_strategy_common
	tm_common
	strict_defaults
)

add_library(tm_strategy STATIC)
target_sources(tm_strategy PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		max_damage.cpp
		net_hp.cpp
		random_selection.cpp
		parse_strategy.cpp
		statistical.cpp
)
target_sources(tm_strategy PRIVATE
	net_hp_impl.cpp
	statistical_impl.cpp
)
target_link_libraries(tm_strategy
	tm_evaluate
	tm_strategy_common
	tm_strategy_expectimax
	strict_defaults
)

add_subdirectory(expectimax)
add_subdirectory(test)