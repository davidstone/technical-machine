# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_shoddy_battle STATIC)
target_sources(tm_shoddy_battle PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		read_team_file.cpp
)
target_sources(tm_shoddy_battle PRIVATE
	read_team_file_impl.cpp
)
target_link_libraries(tm_shoddy_battle
	PUBLIC
		tm_common
		strict_defaults_interface
	PRIVATE
		strict_defaults
)
