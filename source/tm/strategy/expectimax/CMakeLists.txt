# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_strategy_expectimax STATIC)
target_sources(tm_strategy_expectimax PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		expectimax.cpp
		score_selections.cpp
		to_selection_probabilities.cpp
)
target_sources(tm_strategy_expectimax PRIVATE
	expectimax_impl.cpp
)
target_link_libraries(tm_strategy_expectimax
	tm_strategy_common
	tm_evaluate
	strict_defaults
)

add_subdirectory(test)