# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_evaluate STATIC)
target_sources(tm_evaluate PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		all_evaluate.cpp
		compressed_battle.cpp
		depth.cpp
		evaluate.cpp
		evaluate_settings.cpp
		extreme_element_value.cpp
		load_evaluate.cpp
		possible_executed_moves.cpp
		score.cpp
		scored_selection.cpp
		selector.cpp
		state.cpp
		transposition.cpp
		victory.cpp
		win.cpp
)
target_link_libraries(tm_evaluate
	tm_common
	strict_defaults
)

add_subdirectory(test)