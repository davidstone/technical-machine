# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_pokemon_showdown_test)
target_sources(tm_pokemon_showdown_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		battle_message_handler.cpp
		message_block.cpp
		packed_team.cpp
		parse_switch.cpp
		parse_request.cpp
		slot_memory.cpp
)
target_link_libraries(tm_pokemon_showdown_test
	tm_pokemon_showdown
	tm_test_lib
	Catch2::Catch2WithMain
)
add_test(tm_pokemon_showdown_test tm_pokemon_showdown_test)

add_executable(ps_regression_test)
target_sources(ps_regression_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		client_message_handler.cpp
)
target_link_libraries(ps_regression_test
	tm_pokemon_showdown
	Catch2::Catch2WithMain
	TBB::tbb
)
add_test(ps_regression_test ps_regression_test)
