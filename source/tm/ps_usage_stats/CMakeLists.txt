# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(ps_usage_stats STATIC)
target_sources(ps_usage_stats PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		battle_log_to_messages.cpp
		battle_result.cpp
		battle_result_reader.cpp
		battle_result_writer.cpp
		files_in_directory.cpp
		for_each_log.cpp
		glicko1.cpp
		mode.cpp
		parallel_for_each.cpp
		parse_input_log.cpp
		parse_log.cpp
		rating.cpp
		rated_side.cpp
		serialize.cpp
		thread_count.cpp
		usage_stats.cpp
)

target_link_libraries(ps_usage_stats
	tm_pokemon_showdown
)


add_executable(create_selection_weights
	create_selection_weights.cpp
)
target_link_libraries(create_selection_weights
	ps_usage_stats
)

add_executable(ps_usage_stats_create_teams_file
	create_teams_file.cpp
)
target_link_libraries(ps_usage_stats_create_teams_file
	ps_usage_stats
)

add_executable(ps_usage_stats_main
	main.cpp
)
target_link_libraries(ps_usage_stats_main
	ps_usage_stats
)

add_executable(ps_usage_stats_create_derivative_stats
	create_derivative_stats.cpp
)
target_link_libraries(ps_usage_stats_create_derivative_stats
	ps_usage_stats
)

foreach(app
	create_selection_weights
	ps_usage_stats_create_teams_file
	ps_usage_stats_main
	ps_usage_stats_create_derivative_stats
)
	set_target_properties(${app} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endforeach()

add_subdirectory(test)
