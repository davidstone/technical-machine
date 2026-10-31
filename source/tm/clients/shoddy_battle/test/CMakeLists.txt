add_executable(tm_shoddy_battle_test)
target_sources(tm_shoddy_battle_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		read_team_file.cpp
)
target_link_libraries(tm_shoddy_battle_test
	doctest::doctest_with_main
	strict_defaults
	tm_shoddy_battle
)
add_test(tm_shoddy_battle_test tm_shoddy_battle_test)
