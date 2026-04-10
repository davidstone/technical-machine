# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_strategy_test)
target_sources(tm_strategy_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		expectimax.cpp
)
target_link_libraries(tm_strategy_test
	tm_strategy
	Catch2::Catch2WithMain
	TBB::tbb
	strict_defaults
)
add_test(tm_strategy_test tm_strategy_test)

add_executable(score_predict_action
	score_predict_action.cpp
)
target_link_libraries(score_predict_action
	ps_usage_stats
	TBB::tbb
	strict_defaults
)

set_target_properties(score_predict_action PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
