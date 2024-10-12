# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(create_selection_weights
	create_selection_weights.cpp
)
target_link_libraries(create_selection_weights
	tm_pokemon_showdown
)

add_executable(ps_usage_stats_create_teams_file
	create_teams_file.cpp
)
target_link_libraries(ps_usage_stats_create_teams_file
	tm_pokemon_showdown
)

add_executable(ps_usage_stats
	main.cpp
)
target_link_libraries(ps_usage_stats
	tm_pokemon_showdown
)

add_executable(ps_usage_stats_create_derivative_stats
	create_derivative_stats.cpp
)
target_link_libraries(ps_usage_stats_create_derivative_stats
	tm_pokemon_showdown
)

foreach(app
	create_selection_weights
	ps_usage_stats_create_teams_file
	ps_usage_stats
	ps_usage_stats_create_derivative_stats
)
	set_target_properties(${app} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)
endforeach()
