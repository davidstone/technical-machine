# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_executable(tm_string_conversions_test)
target_sources(tm_string_conversions_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		ability.cpp
		gender.cpp
		item.cpp
		move_name.cpp
		nature.cpp
		pokemon.cpp
		species.cpp
		stat_names.cpp
		status_name.cpp
		test_enum_string.cpp
		type.cpp
		weather.cpp
)
target_link_libraries(tm_string_conversions_test
	tm_common
	Catch2::Catch2WithMain
)

add_test(tm_string_conversions_test tm_string_conversions_test)
