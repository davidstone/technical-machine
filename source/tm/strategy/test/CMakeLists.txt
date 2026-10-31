add_executable(score_predict_action
	score_predict_action.cpp
)
target_link_libraries(score_predict_action
	ps_usage_stats
	TBB::tbb
	strict_defaults
)

set_target_properties(score_predict_action PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
