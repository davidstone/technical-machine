add_library(tm_evaluate_test STATIC)
target_sources(tm_evaluate_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		evaluate.cpp
)
target_link_libraries(tm_evaluate_test
	doctest::doctest_with_main
	strict_defaults
	tm_evaluate
)
