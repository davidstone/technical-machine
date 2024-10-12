# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_clients STATIC)
target_sources(tm_clients PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		battle_already_finished.cpp
		battle_continues.cpp
		battle_finished.cpp
		battle_response_error.cpp
		check_weathers_match.cpp
		client_battle.cpp
		determine_selection.cpp
		get_team.cpp
		invalid_team_file_format.cpp
		load_team_from_file.cpp
		make_client_battle.cpp
		party.cpp
		should_accept_challenge.cpp
		teams.cpp
		turn_count.cpp
)
target_sources(tm_clients PRIVATE
	get_team_impl.cpp
	make_client_battle_impl.cpp
)
target_link_libraries(tm_clients
	PUBLIC
		tm_common
		tm_netbattle
		tm_pokemon_lab
		tm_pokemon_online
		tm_shoddy_battle
		strict_defaults_interface
	PRIVATE
		strict_defaults
)
