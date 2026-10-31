add_executable(file_converter
	main.cpp
)
set_target_properties(file_converter PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
target_link_libraries(file_converter
	tm_clients
)