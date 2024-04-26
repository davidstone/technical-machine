# Copyright David Stone 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

project(technical_machine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

enable_testing()

find_package(Boost REQUIRED)
target_compile_definitions(Boost::headers INTERFACE
	BOOST_ASIO_NO_DEPRECATED
	BOOST_BEAST_USE_STD_STRING_VIEW
	BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
	BOOST_CHRONO_HEADER_ONLY
)

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

	set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable testing benchmark.")
	add_subdirectory(dependencies/benchmark EXCLUDE_FROM_ALL)
	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(benchmark PUBLIC
			"-Wno-global-constructors"
			"-Wno-shift-sign-overflow"
		)
	endif()
endif()

add_library(tm_common STATIC)
target_sources(tm_common PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/bide/bide.cpp
		source/tm/bide/damage.cpp
		source/tm/bide/duration.cpp

		source/tm/evaluate/all_evaluate.cpp
		source/tm/evaluate/compressed_battle.cpp
		source/tm/evaluate/depth.cpp
		source/tm/evaluate/evaluate.cpp
		source/tm/evaluate/evaluate_settings.cpp
		source/tm/evaluate/evaluator.cpp
		source/tm/evaluate/expectiminimax.cpp
		source/tm/evaluate/extreme_element_value.cpp
		source/tm/evaluate/load_evaluate.cpp
		source/tm/evaluate/possible_executed_moves.cpp
		source/tm/evaluate/predict_selection.cpp
		source/tm/evaluate/predicted.cpp
		source/tm/evaluate/random_selection.cpp
		source/tm/evaluate/score_selections.cpp
		source/tm/evaluate/scored_selection.cpp
		source/tm/evaluate/selector.cpp
		source/tm/evaluate/state.cpp
		source/tm/evaluate/transposition.cpp
		source/tm/evaluate/victory.cpp
		source/tm/evaluate/win.cpp
	
		source/tm/move/accuracy.cpp
		source/tm/move/activate_when_hit_item.cpp
		source/tm/move/actual_damage.cpp
		source/tm/move/affects_target.cpp
		source/tm/move/base_power.cpp
		source/tm/move/calculate_damage.cpp
		source/tm/move/call_move.cpp
		source/tm/move/category.cpp
		source/tm/move/causes_recoil.cpp
		source/tm/move/damage_type.cpp
		source/tm/move/end_of_attack.cpp
		source/tm/move/executed_move.cpp
		source/tm/move/future_selection.cpp
		source/tm/move/healing_move_fails_in_generation_1.cpp
		source/tm/move/hit_self.cpp
		source/tm/move/initial_move.cpp
		source/tm/move/irrelevant_action.cpp
		source/tm/move/is_blocked_by_gravity.cpp
		source/tm/move/is_delayed_switch.cpp
		source/tm/move/is_healing.cpp
		source/tm/move/is_self_ko.cpp
		source/tm/move/known_move.cpp
		source/tm/move/legal_selections.cpp
		source/tm/move/max_moves_per_pokemon.cpp
		source/tm/move/move.cpp
		source/tm/move/move_cures_target_status.cpp
		source/tm/move/move_is_unsuccessful.cpp
		source/tm/move/move_name.cpp
		source/tm/move/move_names.cpp
		source/tm/move/move_result.cpp
		source/tm/move/no_effect_function.cpp
		source/tm/move/other_action.cpp
		source/tm/move/pass.cpp
		source/tm/move/power.cpp
		source/tm/move/pp.cpp
		source/tm/move/priority.cpp
		source/tm/move/regular_moves.cpp
		source/tm/move/selection.cpp
		source/tm/move/side_effect_function.cpp
		source/tm/move/side_effects.cpp
		source/tm/move/switch.cpp
		source/tm/move/target.cpp
		source/tm/move/usable_while_sleeping.cpp
		source/tm/move/used_move.cpp
		source/tm/move/will_be_recharge_turn.cpp

		source/tm/pokemon/active_pokemon.cpp
		source/tm/pokemon/active_status.cpp
		source/tm/pokemon/any_pokemon.cpp
		source/tm/pokemon/applied_damage.cpp
		source/tm/pokemon/can_be_koed.cpp
		source/tm/pokemon/change_hp.cpp
		source/tm/pokemon/confusion.cpp
		source/tm/pokemon/disable.cpp
		source/tm/pokemon/embargo.cpp
		source/tm/pokemon/encore.cpp
		source/tm/pokemon/end_of_turn_counter.cpp
		source/tm/pokemon/faint.cpp
		source/tm/pokemon/find_required_pokemon_index.cpp
		source/tm/pokemon/get_hidden_power_type.cpp
		source/tm/pokemon/grounded.cpp
		source/tm/pokemon/happiness.cpp
		source/tm/pokemon/has_hidden_power.cpp
		source/tm/pokemon/has_physical_or_special_move.cpp
		source/tm/pokemon/heal_block.cpp
		source/tm/pokemon/hidden_power.cpp
		source/tm/pokemon/hp_ratio.cpp
		source/tm/pokemon/indirect_status_can_apply.cpp
		source/tm/pokemon/initial_pokemon.cpp
		source/tm/pokemon/is_genderless.cpp
		source/tm/pokemon/is_type.cpp
		source/tm/pokemon/known_pokemon.cpp
		source/tm/pokemon/last_used_move.cpp
		source/tm/pokemon/level.cpp
		source/tm/pokemon/magnet_rise.cpp
		source/tm/pokemon/max_pokemon_per_team.cpp
		source/tm/pokemon/nickname.cpp
		source/tm/pokemon/other_pokemon.cpp
		source/tm/pokemon/partial_trap.cpp
		source/tm/pokemon/perish_song.cpp
		source/tm/pokemon/pokemon.cpp
		source/tm/pokemon/pokemon_collection.cpp
		source/tm/pokemon/pokemon_not_found.cpp
		source/tm/pokemon/potentially_selectable_moves.cpp
		source/tm/pokemon/rampage.cpp
		source/tm/pokemon/seen_pokemon.cpp
		source/tm/pokemon/slow_start.cpp
		source/tm/pokemon/species.cpp
		source/tm/pokemon/stockpile.cpp
		source/tm/pokemon/substitute.cpp
		source/tm/pokemon/taunt.cpp
		source/tm/pokemon/uproar.cpp
		source/tm/pokemon/yawn.cpp

		# This file is used for creation and reading. I'd prefer to put this in
		# some other directory so ps_usage_stats can be just for the creation of
		# usage stats.
		source/tm/ps_usage_stats/header.cpp
	
		source/tm/stat/allowed_hp.cpp
		source/tm/stat/base_stats.cpp
		source/tm/stat/calculate.cpp
		source/tm/stat/calculate_ivs_and_evs.cpp
		source/tm/stat/calculate_speed.cpp
		source/tm/stat/chance_to_hit.cpp
		source/tm/stat/combined_stats.cpp
		source/tm/stat/current_hp.cpp
		source/tm/stat/default_evs.cpp
		source/tm/stat/ev.cpp
		source/tm/stat/evs.cpp
		source/tm/stat/faster.cpp
		source/tm/stat/generic_stats.cpp
		source/tm/stat/hp.cpp
		source/tm/stat/initial_stat.cpp
		source/tm/stat/ingame_id_to_nature.cpp
		source/tm/stat/iv.cpp
		source/tm/stat/iv_and_ev.cpp
		source/tm/stat/max_hp.cpp
		source/tm/stat/nature_effect.cpp
		source/tm/stat/nature.cpp
		source/tm/stat/possible_dvs_or_ivs.cpp
		source/tm/stat/stage.cpp
		source/tm/stat/stat_names.cpp
		source/tm/stat/stat_style.cpp
		source/tm/stat/stat_to_ev.cpp
		source/tm/stat/stats.cpp
		source/tm/stat/to_real_hp.cpp

		source/tm/status/blocks_status.cpp
		source/tm/status/check_ability_can_status.cpp
		source/tm/status/clears_status.cpp
		source/tm/status/status.cpp
		source/tm/status/status_can_apply.cpp
		source/tm/status/status_name.cpp
		source/tm/status/team_has_status.cpp
		source/tm/status/toxic_resets_on_switch.cpp
	
		source/tm/string_conversions/ability.cpp
		source/tm/string_conversions/from_string.cpp
		source/tm/string_conversions/gender.cpp
		source/tm/string_conversions/generation.cpp
		source/tm/string_conversions/initial_team.cpp
		source/tm/string_conversions/invalid_string_conversion.cpp
		source/tm/string_conversions/item.cpp
		source/tm/string_conversions/lowercase_alphanumeric.cpp
		source/tm/string_conversions/move_name.cpp
		source/tm/string_conversions/nature.cpp
		source/tm/string_conversions/pokemon_from_string.cpp
		source/tm/string_conversions/pokemon_helpers.cpp
		source/tm/string_conversions/pokemon_to_string.cpp
		source/tm/string_conversions/species.cpp
		source/tm/string_conversions/status_name.cpp
		source/tm/string_conversions/team.cpp
		source/tm/string_conversions/type.cpp
		source/tm/string_conversions/weather.cpp
	
		source/tm/team_predictor/ev_optimizer/compute_minimal_spread.cpp
		source/tm/team_predictor/ev_optimizer/defensive.cpp
		source/tm/team_predictor/ev_optimizer/individual.cpp
		source/tm/team_predictor/ev_optimizer/nature_effects.cpp
		source/tm/team_predictor/ev_optimizer/offensive.cpp
		source/tm/team_predictor/ev_optimizer/optimize_evs.cpp
		source/tm/team_predictor/ev_optimizer/pad_evs.cpp
		source/tm/team_predictor/ev_optimizer/possible_optimized_ivs.cpp
		source/tm/team_predictor/ev_optimizer/speed.cpp
	
		source/tm/team_predictor/all_usage_stats.cpp
		source/tm/team_predictor/estimate.cpp
		source/tm/team_predictor/stats_user.cpp
		source/tm/team_predictor/style.cpp
		source/tm/team_predictor/team_predictor.cpp
		source/tm/team_predictor/usage_stats.cpp
		source/tm/team_predictor/usage_stats_probabilities.cpp

		source/tm/type/effectiveness.cpp
		source/tm/type/move_type.cpp
		source/tm/type/pokemon_types.cpp
		source/tm/type/type.cpp
	
		source/tm/ability.cpp
		source/tm/ability_blocks_move.cpp
		source/tm/ability_blocks_switching.cpp
		source/tm/ability_blocks_weather.cpp
		source/tm/activate_ability_on_switch.cpp
		source/tm/any_team.cpp
		source/tm/apply_entry_hazards.cpp
		source/tm/associated_team.cpp
		source/tm/battle.cpp
		source/tm/bit_view.cpp
		source/tm/blocks_selection_and_execution.cpp
		source/tm/boost_beast_http.cpp
		source/tm/buffer_view.cpp
		source/tm/bytes_in_file.cpp
		source/tm/can_execute_move.cpp
		source/tm/compress.cpp
		source/tm/constant_generation.cpp
		source/tm/contact_ability_effect.cpp
		source/tm/critical_hit_probability.cpp
		source/tm/end_of_turn.cpp
		source/tm/end_of_turn_flags.cpp
		source/tm/entry_hazards.cpp
		source/tm/environment.cpp
		source/tm/exists_if.cpp
		source/tm/files_in_path.cpp
		source/tm/find_required_index.cpp
		source/tm/gender.cpp
		source/tm/generation.cpp
		source/tm/generation_generic.cpp
		source/tm/get_directory.cpp
		source/tm/handle_curse.cpp
		source/tm/heal.cpp
		source/tm/held_item.cpp
		source/tm/initial_team.cpp
		source/tm/item.cpp
		source/tm/get_both_selections.cpp
		source/tm/get_legal_selections.cpp
		source/tm/nlohmann_json.cpp
		source/tm/load_json_from_file.cpp
		source/tm/load_settings_file.cpp
		source/tm/open_file.cpp
		source/tm/operators.cpp
		source/tm/other_team.cpp
		source/tm/property_tree.cpp
		source/tm/rational.cpp
		source/tm/saturating_add.cpp
		source/tm/screen.cpp
		source/tm/screens.cpp
		source/tm/settings_file.cpp
		source/tm/split_offsets.cpp
		source/tm/split_view.cpp
		source/tm/switch_decision_required.cpp
		source/tm/team.cpp
		source/tm/team_is_empty.cpp
		source/tm/to_used_move.cpp
		source/tm/visible_damage_to_actual_damage.cpp
		source/tm/visible_hp.cpp
		source/tm/visible_state.cpp
		source/tm/weather.cpp
		source/tm/wish.cpp
)
target_sources(tm_common PRIVATE
	source/tm/evaluate/predict_selection_impl.cpp
	source/tm/evaluate/score_selections_impl.cpp
	source/tm/move/call_move_impl.cpp
	source/tm/move/side_effects_impl.cpp
	source/tm/string_conversions/team_impl.cpp
	source/tm/team_predictor/team_predictor_impl.cpp
)
target_link_libraries(tm_common
	Boost::headers
	bounded
	containers
	nlohmann_json
	strict_defaults
)
target_include_directories(tm_common PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
	$<INSTALL_INTERFACE:source>
)

add_library(tm_netbattle STATIC)
target_sources(tm_netbattle PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/netbattle/file_version.cpp
		source/tm/clients/netbattle/game_version_to_generation.cpp
		source/tm/clients/netbattle/id_to_ability.cpp
		source/tm/clients/netbattle/id_to_gender.cpp
		source/tm/clients/netbattle/id_to_item.cpp
		source/tm/clients/netbattle/id_to_move.cpp
		source/tm/clients/netbattle/id_to_species.cpp
		source/tm/clients/netbattle/read_team_file.cpp
)
target_link_libraries(tm_netbattle
	bounded
	containers
	tm_common
	strict_defaults
)

add_library(tm_pokemon_lab STATIC)
target_sources(tm_pokemon_lab PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/pokemon_lab/read_team_file.cpp
)
target_sources(tm_pokemon_lab PRIVATE
	source/tm/clients/pokemon_lab/read_team_file_impl.cpp
)
target_link_libraries(tm_pokemon_lab
	bounded
	containers
	tm_common
	strict_defaults
)

add_library(tm_pokemon_online STATIC)
target_sources(tm_pokemon_online PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/pokemon_online/conversion.cpp
		source/tm/clients/pokemon_online/invalid_team_file.cpp
		source/tm/clients/pokemon_online/read_team_file.cpp
)
target_link_libraries(tm_pokemon_online
	bounded
	containers
	tm_common
	strict_defaults
)

add_library(tm_shoddy_battle STATIC)
target_sources(tm_shoddy_battle PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/shoddy_battle/read_team_file.cpp
)
target_sources(tm_shoddy_battle PRIVATE
	source/tm/clients/shoddy_battle/read_team_file_impl.cpp
)
target_link_libraries(tm_shoddy_battle
	bounded
	containers
	tm_common
	strict_defaults
)

add_library(tm_clients STATIC)
target_sources(tm_clients PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/battle_already_finished.cpp
		source/tm/clients/battle_continues.cpp
		source/tm/clients/battle_finished.cpp
		source/tm/clients/battle_response_error.cpp
		source/tm/clients/check_weathers_match.cpp
		source/tm/clients/client_battle.cpp
		source/tm/clients/determine_selection.cpp
		source/tm/clients/get_team.cpp
		source/tm/clients/invalid_team_file_format.cpp
		source/tm/clients/load_team_from_file.cpp
		source/tm/clients/make_client_battle.cpp
		source/tm/clients/party.cpp
		source/tm/clients/should_accept_challenge.cpp
		source/tm/clients/teams.cpp
		source/tm/clients/turn_count.cpp
)
target_sources(tm_clients PRIVATE
	source/tm/clients/get_team_impl.cpp
	source/tm/clients/make_client_battle_impl.cpp
)
target_link_libraries(tm_clients
	bounded
	containers
	strict_defaults
	tm_common
	tm_netbattle
	tm_pokemon_lab
	tm_pokemon_online
	tm_shoddy_battle
)

add_library(tm_pokemon_showdown STATIC)
target_sources(tm_pokemon_showdown PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/clients/pokemon_showdown/action_required.cpp
		source/tm/clients/pokemon_showdown/battle_init_message.cpp
		source/tm/clients/pokemon_showdown/battle_manager.cpp
		source/tm/clients/pokemon_showdown/battle_message.cpp
		source/tm/clients/pokemon_showdown/battle_message_handler.cpp
		source/tm/clients/pokemon_showdown/battle_response_switch.cpp
		source/tm/clients/pokemon_showdown/battle_started.cpp
		source/tm/clients/pokemon_showdown/battles.cpp
		source/tm/clients/pokemon_showdown/client.cpp
		source/tm/clients/pokemon_showdown/client_message_handler.cpp
		source/tm/clients/pokemon_showdown/end_of_turn_state.cpp
		source/tm/clients/pokemon_showdown/end_of_turn_state_builder.cpp
		source/tm/clients/pokemon_showdown/event_block.cpp
		source/tm/clients/pokemon_showdown/in_message.cpp
		source/tm/clients/pokemon_showdown/is_chat_message.cpp
		source/tm/clients/pokemon_showdown/is_chat_message_block.cpp
		source/tm/clients/pokemon_showdown/log_battle_messages.cpp
		source/tm/clients/pokemon_showdown/make_battle_message.cpp
		source/tm/clients/pokemon_showdown/make_battle_init_message.cpp
		source/tm/clients/pokemon_showdown/make_battle_message_handler.cpp
		source/tm/clients/pokemon_showdown/make_party.cpp
		source/tm/clients/pokemon_showdown/message_block.cpp
		source/tm/clients/pokemon_showdown/move_state.cpp
		source/tm/clients/pokemon_showdown/move_state_builder.cpp
		source/tm/clients/pokemon_showdown/optional_hp_and_status.cpp
		source/tm/clients/pokemon_showdown/packed_format_to_team.cpp
		source/tm/clients/pokemon_showdown/parse_details.cpp
		source/tm/clients/pokemon_showdown/parse_effect_source.cpp
		source/tm/clients/pokemon_showdown/parse_gender.cpp
		source/tm/clients/pokemon_showdown/parse_generation_from_format.cpp
		source/tm/clients/pokemon_showdown/parse_hp.cpp
		source/tm/clients/pokemon_showdown/parse_hp_and_status.cpp
		source/tm/clients/pokemon_showdown/parse_moves.cpp
		source/tm/clients/pokemon_showdown/parse_pokemon_identity.cpp
		source/tm/clients/pokemon_showdown/parse_set_hp_message.cpp
		source/tm/clients/pokemon_showdown/parse_status.cpp
		source/tm/clients/pokemon_showdown/parse_switch.cpp
		source/tm/clients/pokemon_showdown/parse_team_from_request.cpp
		source/tm/clients/pokemon_showdown/parsed_message.cpp
		source/tm/clients/pokemon_showdown/parsed_side.cpp
		source/tm/clients/pokemon_showdown/parsed_team.cpp
		source/tm/clients/pokemon_showdown/parsed_team_to_known_team.cpp
		source/tm/clients/pokemon_showdown/party_from_player_id.cpp
		source/tm/clients/pokemon_showdown/room.cpp
		source/tm/clients/pokemon_showdown/room_message_block.cpp
		source/tm/clients/pokemon_showdown/send_message_function.cpp
		source/tm/clients/pokemon_showdown/slot_memory.cpp
		source/tm/clients/pokemon_showdown/start_of_turn.cpp
		source/tm/clients/pokemon_showdown/switch_message.cpp
		source/tm/clients/pokemon_showdown/sockets.cpp
		source/tm/clients/pokemon_showdown/to_packed_format.cpp

		source/tm/ps_usage_stats/add_to_workers.cpp
		source/tm/ps_usage_stats/battle_log_to_messages.cpp
		source/tm/ps_usage_stats/battle_result.cpp
		source/tm/ps_usage_stats/battle_result_reader.cpp
		source/tm/ps_usage_stats/battle_result_writer.cpp
		source/tm/ps_usage_stats/files_in_directory.cpp
		source/tm/ps_usage_stats/glicko1.cpp
		source/tm/ps_usage_stats/mode.cpp
		source/tm/ps_usage_stats/parse_input_log.cpp
		source/tm/ps_usage_stats/parse_log.cpp
		source/tm/ps_usage_stats/rating.cpp
		source/tm/ps_usage_stats/serialize.cpp
		source/tm/ps_usage_stats/thread_count.cpp
		source/tm/ps_usage_stats/to_index.cpp
		source/tm/ps_usage_stats/usage_stats.cpp
		source/tm/ps_usage_stats/worker.cpp
)
target_sources(tm_pokemon_showdown PRIVATE
	source/tm/clients/pokemon_showdown/battle_message_handler_impl.cpp
	source/tm/clients/pokemon_showdown/make_battle_message_handler_impl.cpp
	source/tm/clients/pokemon_showdown/to_packed_format_impl.cpp
)
target_link_libraries(tm_pokemon_showdown
	Boost::headers
	bounded
	concurrent
	containers
	tm_common
	tm_clients
	strict_defaults
)

add_executable(ai
	source/tm/ai/main.cpp
)
target_sources(ai PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/ai/parse_args.cpp
		source/tm/ai/print_sizes.cpp
)
target_link_libraries(ai
	tm_common
	tm_pokemon_showdown
	TBB::tbb
)

add_executable(file_converter
	source/tm/file_converter/main.cpp
)
target_link_libraries(file_converter
	tm_common
	tm_pokemon_lab
	tm_pokemon_online
	tm_clients
)

add_executable(ps_usage_stats_create_teams_file
	source/tm/ps_usage_stats/create_teams_file.cpp
)
target_link_libraries(ps_usage_stats_create_teams_file
	tm_common
	tm_pokemon_showdown
)

add_executable(ps_usage_stats
	source/tm/ps_usage_stats/main.cpp
)
target_link_libraries(ps_usage_stats
	tm_common
	tm_pokemon_showdown
)

add_executable(ps_usage_stats_create_derivative_stats
	source/tm/ps_usage_stats/create_derivative_stats.cpp
)
target_link_libraries(ps_usage_stats_create_derivative_stats
	tm_common
	tm_pokemon_showdown
)


add_executable(predict
	source/tm/team_predictor/predictor.cpp
)
target_sources(predict PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/team_predictor/connection.cpp
		source/tm/team_predictor/generate_team_builder_ui.cpp
		source/tm/team_predictor/mutable_buffer.cpp
		source/tm/team_predictor/parser.cpp
)
target_link_libraries(predict
	tm_common
)

add_executable(tm_test)
target_sources(tm_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/test/pokemon_online/conversion.cpp
	
		source/tm/test/clients/pokemon_showdown/battle_message_handler.cpp
		source/tm/test/clients/pokemon_showdown/message_block.cpp
		source/tm/test/clients/pokemon_showdown/packed_team.cpp
		source/tm/test/clients/pokemon_showdown/parse_switch.cpp
		source/tm/test/clients/pokemon_showdown/parse_team_from_request.cpp
		
		source/tm/test/clients/shoddy_battle/read_team_file.cpp

		source/tm/test/clients/client_battle.cpp

		source/tm/test/collections/variable_collection.cpp

		source/tm/test/move/actual_damage.cpp
		source/tm/test/move/calculate_damage.cpp
		source/tm/test/move/move_power.cpp
		source/tm/test/move/possible_side_effects.cpp

		source/tm/test/pokemon/active_pokemon.cpp

		source/tm/test/ps_usage_stats/battle_log_to_messages.cpp
		source/tm/test/ps_usage_stats/glicko1.cpp
		source/tm/test/ps_usage_stats/parse_input_log.cpp

		source/tm/test/stat/calculate.cpp
		source/tm/test/stat/calculate_ivs_and_evs.cpp
		source/tm/test/stat/calculate_speed.cpp

		source/tm/test/status/status.cpp
		
		source/tm/test/team_predictor/ev_optimizer/defensive.cpp
		source/tm/test/team_predictor/ev_optimizer/pad_evs.cpp
		source/tm/test/team_predictor/ev_optimizer/speed.cpp

		source/tm/test/can_execute_move.cpp
		source/tm/test/critical_hit_probability.cpp
		source/tm/test/get_legal_selections.cpp
		source/tm/test/make_seen_team.cpp
		source/tm/test/string_conversion.cpp
		source/tm/test/usage_bytes.cpp
		source/tm/test/visible_damage_to_actual_damage.cpp
)
target_sources(tm_test PRIVATE
	source/tm/test/clients/netbattle/read_team_file.cpp

	source/tm/test/clients/pokemon_showdown/slot_memory.cpp

	source/tm/test/evaluate/evaluate.cpp
	source/tm/test/evaluate/expectiminimax.cpp
	source/tm/test/evaluate/predict_selection.cpp

	source/tm/test/move/call_move.cpp

	source/tm/test/pokemon_lab/team_file.cpp

	source/tm/test/pokemon_online/team_file.cpp

	source/tm/test/ps_usage_stats/read_write_battle_result.cpp
	source/tm/test/ps_usage_stats/serialize.cpp

	source/tm/test/team_predictor/estimate.cpp
	source/tm/test/team_predictor/usage_stats.cpp

	source/tm/test/battle.cpp
	source/tm/test/optimize_evs.cpp
	source/tm/test/files_in_path.cpp
	source/tm/test/get_directory.cpp
	source/tm/test/sleep.cpp
)
target_link_libraries(tm_test
	tm_common
	tm_clients
	tm_netbattle
	tm_pokemon_lab
	tm_pokemon_online
	tm_pokemon_showdown
	tm_shoddy_battle
	Catch2::Catch2WithMain
	TBB::tbb
)

add_executable(ps_regression_test)
target_sources(ps_regression_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		source/tm/test/clients/pokemon_showdown/battles.cpp
)
target_link_libraries(ps_regression_test
	tm_common
	tm_pokemon_showdown
	Catch2::Catch2WithMain
	TBB::tbb
)

add_executable(score_predict_action
	source/tm/test/ps_usage_stats/score_predict_action.cpp
)
target_link_libraries(score_predict_action
	tm_common
	tm_pokemon_showdown
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	if(MSVC)
		target_link_options(tm_common PUBLIC
			"/STACK:8388608"
		)
		set_target_properties(tm_test PROPERTIES
			VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
		)

		target_compile_options(tm_common PUBLIC
			# For some reason, exceptions are turned off here but not in bounded
			"/EHsc"
			# Setting boost as a system include does not turn off these warnings on Windows
			"-Wno-gnu-anonymous-struct"
			"-Wno-comma"
			"-Wno-conversion"
			"-Wno-covered-switch-default"
			"-Wno-deprecated"
			"-Wno-deprecated-dynamic-exception-spec"
			"-Wno-documentation-unknown-command"
			"-Wno-extra-semi"
			"-Wno-global-constructors"
			"-Wno-implicit-fallthrough"
			"-Wno-inconsistent-missing-destructor-override"
			"-Wno-language-extension-token"
			"-Wno-missing-noreturn"
			"-Wno-nonportable-system-include-path"
			"-Wno-non-virtual-dtor"
			"-Wno-old-style-cast"
			"-Wno-redundant-parens"
			"-Wno-reserved-id-macro"
			"-Wno-shadow"
			"-Wno-shadow-field-in-constructor"
			"-Wno-shadow-uncaptured-local"
			"-Wno-shorten-64-to-32"
			"-Wno-sign-conversion"
			"-Wno-undef"
			"-Wno-unused-private-field"
		)
	endif()
	target_link_libraries(ai pthread)
	target_link_libraries(predict pthread)
	target_link_libraries(score_predict_action atomic)

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(tm_common PUBLIC
		# gcc does not understand switching over an enum where only explicitly
		# defined enumerators are valid
		"-Wno-implicit-fallthrough"
		# False positive on variant implementation if lto is off
		"-Wno-maybe-uninitialized"
		"-Wodr"
		# -Wreturn-type warns even for a switch over an enum for which all cases
		# are handled explicitly and each case is a return statement.
		"-Wno-return-type"
	)
endif()

add_test(tm_test tm_test)
add_test(ps_regression_test ps_regression_test)
