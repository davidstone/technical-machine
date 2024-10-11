# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_web_server_test)
target_sources(tm_web_server_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		normalize_requested_file.cpp
)
target_link_libraries(tm_web_server_test
	tm_web_server
	Catch2::Catch2WithMain
)

add_test(tm_web_server_test tm_web_server_test)
