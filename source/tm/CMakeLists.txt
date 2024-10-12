# Copyright David Stone 2024.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_common STATIC)
target_sources(tm_common PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		bide/bide.cpp
		bide/damage.cpp
		bide/duration.cpp

		evaluate/all_evaluate.cpp
		evaluate/compressed_battle.cpp
		evaluate/depth.cpp
		evaluate/evaluate.cpp
		evaluate/evaluate_settings.cpp
		evaluate/evaluator.cpp
		evaluate/extreme_element_value.cpp
		evaluate/load_evaluate.cpp
		evaluate/possible_executed_moves.cpp
		evaluate/scored_selection.cpp
		evaluate/selector.cpp
		evaluate/state.cpp
		evaluate/transposition.cpp
		evaluate/victory.cpp
		evaluate/win.cpp
	
		move/accuracy.cpp
		move/activate_when_hit_item.cpp
		move/actual_damage.cpp
		move/affects_target.cpp
		move/base_power.cpp
		move/calculate_damage.cpp
		move/call_move.cpp
		move/category.cpp
		move/causes_recoil.cpp
		move/damage_type.cpp
		move/end_of_attack.cpp
		move/executed_move.cpp
		move/future_selection.cpp
		move/healing_move_fails_in_generation_1.cpp
		move/hit_self.cpp
		move/initial_move.cpp
		move/irrelevant_action.cpp
		move/is_blocked_by_gravity.cpp
		move/is_delayed_switch.cpp
		move/is_healing.cpp
		move/is_self_ko.cpp
		move/known_move.cpp
		move/legal_selections.cpp
		move/max_moves_per_pokemon.cpp
		move/move.cpp
		move/move_cures_target_status.cpp
		move/move_is_unsuccessful.cpp
		move/move_name.cpp
		move/move_names.cpp
		move/move_result.cpp
		move/no_effect_function.cpp
		move/other_action.cpp
		move/pass.cpp
		move/power.cpp
		move/pp.cpp
		move/priority.cpp
		move/regular_moves.cpp
		move/selection.cpp
		move/side_effect_function.cpp
		move/side_effects.cpp
		move/switch.cpp
		move/target.cpp
		move/usable_while_sleeping.cpp
		move/used_move.cpp

		pokemon/active_pokemon.cpp
		pokemon/active_status.cpp
		pokemon/any_pokemon.cpp
		pokemon/applied_damage.cpp
		pokemon/can_be_koed.cpp
		pokemon/change_hp.cpp
		pokemon/confusion.cpp
		pokemon/disable.cpp
		pokemon/embargo.cpp
		pokemon/encore.cpp
		pokemon/end_of_turn_counter.cpp
		pokemon/faint.cpp
		pokemon/find_required_pokemon_index.cpp
		pokemon/get_hidden_power_type.cpp
		pokemon/grounded.cpp
		pokemon/happiness.cpp
		pokemon/has_hidden_power.cpp
		pokemon/has_physical_or_special_move.cpp
		pokemon/heal_block.cpp
		pokemon/hidden_power.cpp
		pokemon/hp_ratio.cpp
		pokemon/indirect_status_can_apply.cpp
		pokemon/initial_pokemon.cpp
		pokemon/is_genderless.cpp
		pokemon/is_type.cpp
		pokemon/known_pokemon.cpp
		pokemon/last_used_move.cpp
		pokemon/level.cpp
		pokemon/magnet_rise.cpp
		pokemon/max_pokemon_per_team.cpp
		pokemon/nickname.cpp
		pokemon/other_pokemon.cpp
		pokemon/partial_trap.cpp
		pokemon/perish_song.cpp
		pokemon/pokemon.cpp
		pokemon/pokemon_collection.cpp
		pokemon/pokemon_not_found.cpp
		pokemon/potentially_selectable_moves.cpp
		pokemon/rampage.cpp
		pokemon/seen_pokemon.cpp
		pokemon/slow_start.cpp
		pokemon/species.cpp
		pokemon/stockpile.cpp
		pokemon/substitute.cpp
		pokemon/taunt.cpp
		pokemon/uproar.cpp
		pokemon/yawn.cpp

		# This file is used for creation and reading. I'd prefer to put this in
		# some other directory so ps_usage_stats can be just for the creation of
		# usage stats.
		ps_usage_stats/header.cpp
	
		stat/allowed_hp.cpp
		stat/base_stats.cpp
		stat/calculate.cpp
		stat/calculate_ivs_and_evs.cpp
		stat/calculate_speed.cpp
		stat/chance_to_hit.cpp
		stat/combined_stats.cpp
		stat/current_hp.cpp
		stat/default_evs.cpp
		stat/ev.cpp
		stat/evs.cpp
		stat/faster.cpp
		stat/generic_stats.cpp
		stat/hp.cpp
		stat/initial_stat.cpp
		stat/ingame_id_to_nature.cpp
		stat/iv.cpp
		stat/iv_and_ev.cpp
		stat/max_hp.cpp
		stat/nature_effect.cpp
		stat/nature.cpp
		stat/possible_dvs_or_ivs.cpp
		stat/stage.cpp
		stat/stat_names.cpp
		stat/stat_style.cpp
		stat/stat_to_ev.cpp
		stat/stats.cpp
		stat/to_real_hp.cpp

		status/blocks_status.cpp
		status/check_ability_can_status.cpp
		status/clears_status.cpp
		status/status.cpp
		status/status_can_apply.cpp
		status/status_name.cpp
		status/team_has_status.cpp
		status/toxic_resets_on_switch.cpp
	
		strategy/expectimax.cpp
		strategy/max_damage.cpp
		strategy/net_hp.cpp
		strategy/random_selection.cpp
		strategy/statistical.cpp
		strategy/strategy.cpp
		strategy/weighted_selection.cpp

		string_conversions/ability.cpp
		string_conversions/from_string.cpp
		string_conversions/gender.cpp
		string_conversions/generation.cpp
		string_conversions/initial_team.cpp
		string_conversions/invalid_string_conversion.cpp
		string_conversions/item.cpp
		string_conversions/lowercase_alphanumeric.cpp
		string_conversions/move_name.cpp
		string_conversions/nature.cpp
		string_conversions/pokemon.cpp
		string_conversions/species.cpp
		string_conversions/stat_names.cpp
		string_conversions/status_name.cpp
		string_conversions/team.cpp
		string_conversions/type.cpp
		string_conversions/weather.cpp
	
		team_predictor/ev_optimizer/compute_minimal_spread.cpp
		team_predictor/ev_optimizer/defensive.cpp
		team_predictor/ev_optimizer/individual.cpp
		team_predictor/ev_optimizer/nature_effects.cpp
		team_predictor/ev_optimizer/offensive.cpp
		team_predictor/ev_optimizer/optimize_evs.cpp
		team_predictor/ev_optimizer/pad_evs.cpp
		team_predictor/ev_optimizer/possible_optimized_ivs.cpp
		team_predictor/ev_optimizer/speed.cpp
	
		team_predictor/all_usage_stats.cpp
		team_predictor/estimate.cpp
		team_predictor/stats_user.cpp
		team_predictor/team_predictor.cpp
		team_predictor/usage_stats.cpp
		team_predictor/usage_stats_probabilities.cpp

		type/effectiveness.cpp
		type/move_type.cpp
		type/pokemon_types.cpp
		type/type.cpp
	
		ability.cpp
		ability_blocks_move.cpp
		ability_blocks_switching.cpp
		activate_ability_on_switch.cpp
		all_selections_valid.cpp
		any_team.cpp
		apply_entry_hazards.cpp
		associated_team.cpp
		battle.cpp
		bit_view.cpp
		blocks_selection_and_execution.cpp
		boost_networking.cpp
		buffer_view.cpp
		bytes_in_file.cpp
		can_execute_move.cpp
		compress.cpp
		constant_generation.cpp
		contact_ability_effect.cpp
		critical_hit_probability.cpp
		end_of_turn.cpp
		end_of_turn_flags.cpp
		entry_hazards.cpp
		environment.cpp
		exists_if.cpp
		files_in_path.cpp
		find_required_index.cpp
		gender.cpp
		generation.cpp
		generation_generic.cpp
		get_directory.cpp
		handle_curse.cpp
		heal.cpp
		held_item.cpp
		initial_team.cpp
		item.cpp
		get_legal_selections.cpp
		nlohmann_json.cpp
		load_json_from_file.cpp
		load_settings_file.cpp
		open_file.cpp
		operators.cpp
		other_team.cpp
		property_tree.cpp
		rational.cpp
		read_bytes.cpp
		saturating_add.cpp
		screen.cpp
		screens.cpp
		settings_file.cpp
		split_offsets.cpp
		split_view.cpp
		switch_decision_required.cpp
		team.cpp
		team_is_empty.cpp
		to_index.cpp
		to_used_move.cpp
		usage_for.cpp
		visible_damage_to_actual_damage.cpp
		visible_hp.cpp
		visible_state.cpp
		weather.cpp
		wish.cpp
		write_bytes.cpp
)
target_sources(tm_common PRIVATE
	move/call_move_impl.cpp
	move/side_effects_impl.cpp
	strategy/expectimax_impl.cpp
	strategy/net_hp_impl.cpp
	strategy/statistical_impl.cpp
	string_conversions/team_impl.cpp
	team_predictor/team_predictor_impl.cpp
)
target_link_libraries(tm_common
	boost
	containers
	OpenSSL::SSL
	nlohmann_json
	strict_defaults
)
target_include_directories(tm_common PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
	$<INSTALL_INTERFACE:source>
)

add_subdirectory(clients/netbattle)
add_subdirectory(clients/pokemon_lab)
add_subdirectory(clients/pokemon_online)
add_subdirectory(clients/shoddy_battle)
add_subdirectory(clients)

add_library(tm_pokemon_showdown STATIC)
target_sources(tm_pokemon_showdown PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		clients/pokemon_showdown/action_required.cpp
		clients/pokemon_showdown/battle_init_message.cpp
		clients/pokemon_showdown/battle_manager.cpp
		clients/pokemon_showdown/battle_message.cpp
		clients/pokemon_showdown/battle_message_handler.cpp
		clients/pokemon_showdown/battle_response_switch.cpp
		clients/pokemon_showdown/battle_started.cpp
		clients/pokemon_showdown/battles.cpp
		clients/pokemon_showdown/client.cpp
		clients/pokemon_showdown/client_message_handler.cpp
		clients/pokemon_showdown/end_of_turn_state.cpp
		clients/pokemon_showdown/end_of_turn_state_builder.cpp
		clients/pokemon_showdown/event_block.cpp
		clients/pokemon_showdown/in_message.cpp
		clients/pokemon_showdown/is_chat_message.cpp
		clients/pokemon_showdown/is_chat_message_block.cpp
		clients/pokemon_showdown/log_battle_messages.cpp
		clients/pokemon_showdown/make_battle_message.cpp
		clients/pokemon_showdown/make_battle_init_message.cpp
		clients/pokemon_showdown/make_battle_message_handler.cpp
		clients/pokemon_showdown/make_party.cpp
		clients/pokemon_showdown/message_block.cpp
		clients/pokemon_showdown/move_state.cpp
		clients/pokemon_showdown/move_state_builder.cpp
		clients/pokemon_showdown/optional_hp_and_status.cpp
		clients/pokemon_showdown/packed_format_to_team.cpp
		clients/pokemon_showdown/parse_details.cpp
		clients/pokemon_showdown/parse_effect_source.cpp
		clients/pokemon_showdown/parse_gender.cpp
		clients/pokemon_showdown/parse_generation_from_format.cpp
		clients/pokemon_showdown/parse_hp.cpp
		clients/pokemon_showdown/parse_hp_and_status.cpp
		clients/pokemon_showdown/parse_moves.cpp
		clients/pokemon_showdown/parse_pokemon_identity.cpp
		clients/pokemon_showdown/parse_set_hp_message.cpp
		clients/pokemon_showdown/parse_status.cpp
		clients/pokemon_showdown/parse_switch.cpp
		clients/pokemon_showdown/parse_team_from_request.cpp
		clients/pokemon_showdown/parsed_message.cpp
		clients/pokemon_showdown/parsed_side.cpp
		clients/pokemon_showdown/parsed_team.cpp
		clients/pokemon_showdown/parsed_team_to_known_team.cpp
		clients/pokemon_showdown/party_from_player_id.cpp
		clients/pokemon_showdown/room.cpp
		clients/pokemon_showdown/room_message_block.cpp
		clients/pokemon_showdown/send_message_function.cpp
		clients/pokemon_showdown/slot_memory.cpp
		clients/pokemon_showdown/start_of_turn.cpp
		clients/pokemon_showdown/switch_message.cpp
		clients/pokemon_showdown/sockets.cpp
		clients/pokemon_showdown/to_packed_format.cpp

		ps_usage_stats/add_to_workers.cpp
		ps_usage_stats/battle_log_to_messages.cpp
		ps_usage_stats/battle_result.cpp
		ps_usage_stats/battle_result_reader.cpp
		ps_usage_stats/battle_result_writer.cpp
		ps_usage_stats/files_in_directory.cpp
		ps_usage_stats/glicko1.cpp
		ps_usage_stats/mode.cpp
		ps_usage_stats/parse_input_log.cpp
		ps_usage_stats/parse_log.cpp
		ps_usage_stats/rating.cpp
		ps_usage_stats/serialize.cpp
		ps_usage_stats/thread_count.cpp
		ps_usage_stats/usage_stats.cpp
		ps_usage_stats/worker.cpp
)
target_sources(tm_pokemon_showdown PRIVATE
	clients/pokemon_showdown/battle_message_handler_impl.cpp
	clients/pokemon_showdown/make_battle_message_handler_impl.cpp
	clients/pokemon_showdown/to_packed_format_impl.cpp
)
target_link_libraries(tm_pokemon_showdown
	concurrent
	tm_clients
	strict_defaults
)

add_subdirectory(ai)
add_subdirectory(file_converter)
add_subdirectory(ps_usage_stats)
add_subdirectory(string_conversions/test)
add_subdirectory(team_predictor/server)
add_subdirectory(web_server)

add_library(tm_test_lib STATIC)
target_sources(tm_test_lib PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		test/usage_bytes.cpp
)
target_link_libraries(tm_test_lib
	PUBLIC
		tm_common
		strict_defaults_interface
	PRIVATE
		strict_defaults
)

add_executable(tm_test)
target_sources(tm_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		test/pokemon_online/conversion.cpp
	
		test/clients/pokemon_showdown/battle_message_handler.cpp
		test/clients/pokemon_showdown/message_block.cpp
		test/clients/pokemon_showdown/packed_team.cpp
		test/clients/pokemon_showdown/parse_switch.cpp
		test/clients/pokemon_showdown/parse_team_from_request.cpp
		
		test/clients/shoddy_battle/read_team_file.cpp

		test/clients/client_battle.cpp

		test/collections/variable_collection.cpp

		test/move/actual_damage.cpp
		test/move/calculate_damage.cpp
		test/move/move_power.cpp
		test/move/possible_side_effects.cpp

		test/pokemon/active_pokemon.cpp

		test/ps_usage_stats/battle_log_to_messages.cpp
		test/ps_usage_stats/glicko1.cpp
		test/ps_usage_stats/parse_input_log.cpp

		test/stat/calculate.cpp
		test/stat/calculate_ivs_and_evs.cpp
		test/stat/calculate_speed.cpp

		test/status/status.cpp
		
		test/team_predictor/ev_optimizer/defensive.cpp
		test/team_predictor/ev_optimizer/pad_evs.cpp
		test/team_predictor/ev_optimizer/speed.cpp

		test/can_execute_move.cpp
		test/critical_hit_probability.cpp
		test/get_legal_selections.cpp
		test/make_seen_team.cpp
		test/visible_damage_to_actual_damage.cpp
)
target_sources(tm_test PRIVATE
	test/clients/netbattle/read_team_file.cpp

	test/clients/pokemon_showdown/slot_memory.cpp

	test/evaluate/evaluate.cpp
	
	test/move/call_move.cpp
	
	test/pokemon_lab/team_file.cpp
	
	test/pokemon_online/team_file.cpp
	
	test/ps_usage_stats/read_write_battle_result.cpp
	test/ps_usage_stats/serialize.cpp

	test/strategy/expectimax.cpp

	test/team_predictor/estimate.cpp
	test/team_predictor/usage_stats.cpp

	test/battle.cpp
	test/optimize_evs.cpp
	test/files_in_path.cpp
	test/get_directory.cpp
	test/sleep.cpp
)
target_link_libraries(tm_test
	tm_pokemon_showdown
	tm_test_lib
	Catch2::Catch2WithMain
	TBB::tbb
)
add_test(tm_test tm_test)

add_executable(ps_regression_test)
target_sources(ps_regression_test PRIVATE
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		test/clients/pokemon_showdown/battles.cpp
)
target_link_libraries(ps_regression_test
	tm_pokemon_showdown
	Catch2::Catch2WithMain
	TBB::tbb
)
add_test(ps_regression_test ps_regression_test)

add_executable(score_predict_action
	test/ps_usage_stats/score_predict_action.cpp
)
target_link_libraries(score_predict_action
	tm_pokemon_showdown
	TBB::tbb
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
