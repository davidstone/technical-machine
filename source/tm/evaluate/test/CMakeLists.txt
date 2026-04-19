# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_evaluate_test STATIC)
target_sources(tm_evaluate_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		evaluate.cpp
)
target_link_libraries(tm_evaluate_test
	tm_evaluate
	strict_defaults
	Catch2::Catch2WithMain
)
