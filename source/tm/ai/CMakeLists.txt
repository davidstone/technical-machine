add_executable(ai
	main.cpp
)
target_sources(ai PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		print_sizes.cpp
)
target_link_libraries(ai
	tm_pokemon_showdown
	TBB::tbb
)
set_target_properties(ai PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
