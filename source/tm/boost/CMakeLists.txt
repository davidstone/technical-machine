# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(boost INTERFACE IMPORTED)
if(DEFINED Boost_INCLUDE_DIR)
	target_include_directories(boost SYSTEM INTERFACE "${Boost_INCLUDE_DIR}")
endif()
target_compile_definitions(boost INTERFACE
	BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
	BOOST_CHRONO_HEADER_ONLY
)

find_package(OpenSSL REQUIRED)

add_library(tm_boost_networking STATIC)
target_sources(tm_boost_networking PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		networking.cpp
)
target_link_libraries(tm_boost_networking
	PUBLIC
		containers
	PRIVATE
		boost
		OpenSSL::SSL
		strict_defaults
)

add_library(tm_boost_property_tree STATIC)
target_sources(tm_boost_property_tree PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		property_tree.cpp
)
target_link_libraries(tm_boost_property_tree
	PUBLIC
		strict_defaults_interface
		std_module
	PRIVATE
		boost
		strict_defaults
)
