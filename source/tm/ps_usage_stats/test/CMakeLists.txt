# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(score_predict_action
	score_predict_action.cpp
)
target_link_libraries(score_predict_action
	ps_usage_stats
	TBB::tbb
	strict_defaults
)

add_executable(ps_usage_stats_test)
target_sources(ps_usage_stats_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		battle_log_to_messages.cpp
		glicko1.cpp
		parse_input_log.cpp
		read_write_battle_result.cpp
		serialize.cpp
)
target_link_libraries(ps_usage_stats_test
	ps_usage_stats
	tm_test_lib
	strict_defaults
	Catch2::Catch2WithMain
	TBB::tbb
)
add_test(ps_usage_stats_test ps_usage_stats_test)

foreach(app
	score_predict_action
)
	set_target_properties(${app} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endforeach()
