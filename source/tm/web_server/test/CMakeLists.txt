add_executable(tm_web_server_test)
target_sources(tm_web_server_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		normalize_requested_file.cpp
)
target_link_libraries(tm_web_server_test
	doctest::doctest_with_main
	tm_web_server
)

add_test(tm_web_server_test tm_web_server_test)
