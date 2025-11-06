# Copyright David Stone 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

project(technical_machine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

enable_testing()

add_library(boost INTERFACE IMPORTED)
if(DEFINED Boost_INCLUDE_DIR)
	target_include_directories(boost SYSTEM INTERFACE "${Boost_INCLUDE_DIR}")
endif()
target_compile_definitions(boost INTERFACE
	BOOST_ASIO_NO_DEPRECATED
	BOOST_BEAST_USE_STD_STRING_VIEW
	BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
	BOOST_CHRONO_HEADER_ONLY
)

find_package(OpenSSL REQUIRED)

find_package(TBB REQUIRED)

add_library(nlohmann_json INTERFACE IMPORTED)
target_include_directories(nlohmann_json SYSTEM INTERFACE "dependencies/json/include")

get_directory_property(has_parent PARENT_DIRECTORY)
if (NOT has_parent)
	add_subdirectory(dependencies/cmake_strict_defaults)
	add_subdirectory(dependencies/concurrent)
	add_subdirectory(dependencies/numeric-traits)
	add_subdirectory(dependencies/operators)
	add_subdirectory(dependencies/bounded-integer)
	add_subdirectory(dependencies/std_module)

	add_subdirectory(dependencies/Catch2 EXCLUDE_FROM_ALL)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(Catch2 PUBLIC
			"-Wno-c2y-extensions"
		)
	endif()

	set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable testing benchmark.")
	add_subdirectory(dependencies/benchmark EXCLUDE_FROM_ALL)
	get_property(benchmark_include_dirs TARGET benchmark PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
	set_property(TARGET benchmark PROPERTY INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${benchmark_include_dirs}")
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(benchmark PUBLIC
			"-Wno-global-constructors"
			"-Wno-c2y-extensions"
		)
	endif()
endif()

add_subdirectory(source/tm)
