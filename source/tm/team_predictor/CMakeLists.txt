# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_team_predictor STATIC)
target_sources(tm_team_predictor PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		ev_optimizer/compute_minimal_spread.cpp
		ev_optimizer/defensive.cpp
		ev_optimizer/individual.cpp
		ev_optimizer/nature_effects.cpp
		ev_optimizer/offensive.cpp
		ev_optimizer/optimize_evs.cpp
		ev_optimizer/pad_evs.cpp
		ev_optimizer/possible_optimized_ivs.cpp
		ev_optimizer/speed.cpp
	
		all_usage_stats.cpp
		estimate.cpp
		stats_user.cpp
		team_predictor.cpp
		usage_stats.cpp
		usage_stats_probabilities.cpp
)
target_sources(tm_team_predictor PRIVATE
	team_predictor_impl.cpp
)
target_link_libraries(tm_team_predictor
	tm_common
	strict_defaults
)

add_subdirectory(server)
add_subdirectory(test)
add_subdirectory(ev_optimizer/test)
