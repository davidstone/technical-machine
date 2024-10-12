# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_netbattle STATIC)
target_sources(tm_netbattle PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		file_version.cpp
		game_version_to_generation.cpp
		id_to_ability.cpp
		id_to_gender.cpp
		id_to_item.cpp
		id_to_move.cpp
		id_to_species.cpp
		read_team_file.cpp
)
target_link_libraries(tm_netbattle
	PUBLIC
		tm_common
		strict_defaults_interface
	PRIVATE
		strict_defaults
)
