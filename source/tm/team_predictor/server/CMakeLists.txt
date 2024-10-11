# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(team_predictor_server_lib STATIC)
target_sources(team_predictor_server_lib PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		make_dictionary.cpp
		parse_predictor_inputs.cpp
		predicted_team_string.cpp
		predictor_inputs.cpp
		style.cpp
)
target_link_libraries(team_predictor_server_lib
	tm_web_server
	tm_common
	strict_defaults
	pthread
)
add_executable(team_predictor_server
	main.cpp
)
set_target_properties(team_predictor_server PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
target_link_libraries(team_predictor_server
	team_predictor_server_lib
)

add_subdirectory(test)