# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(team_predictor_server_test)
target_sources(team_predictor_server_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		make_dictionary.cpp
)
target_link_libraries(team_predictor_server_test
	team_predictor_server_lib
	tm_test_lib
	Catch2::Catch2WithMain
)

add_test(team_predictor_server_test team_predictor_server_test)
