# Compile-time-only tests
add_library(netbattle_test STATIC)
target_sources(netbattle_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		read_team_file.cpp
)
target_link_libraries(netbattle_test
	tm_netbattle
	strict_defaults
)
