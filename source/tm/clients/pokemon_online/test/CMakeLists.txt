# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_pokemon_online_test)
target_sources(tm_pokemon_online_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		conversion.cpp
		team_file.cpp
)
target_link_libraries(tm_pokemon_online_test
	strict_defaults
	tm_pokemon_online
	Catch2::Catch2WithMain
)
add_test(tm_pokemon_online_test tm_pokemon_online_test)
