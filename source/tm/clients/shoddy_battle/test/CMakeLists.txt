# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_shoddy_battle_test)
target_sources(tm_shoddy_battle_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		read_team_file.cpp
)
target_link_libraries(tm_shoddy_battle_test
	strict_defaults
	tm_shoddy_battle
	Catch2::Catch2WithMain
)
add_test(tm_shoddy_battle_test tm_shoddy_battle_test)
