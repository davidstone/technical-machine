# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_web_server STATIC)
target_sources(tm_web_server PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		extension_to_content_type.cpp
		http_server.cpp
		normalize_requested_file.cpp
		web_server.cpp
		websocket_server.cpp
)
target_link_libraries(tm_web_server
	PUBLIC
		boost
		containers
		strict_defaults_interface
		tm_common
	PRIVATE
		strict_defaults
)

add_subdirectory(test)