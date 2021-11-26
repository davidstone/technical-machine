# Copyright David Stone 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()


project(technical_machine LANGUAGES CXX)

find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
target_compile_definitions(boost INTERFACE
	BOOST_ASIO_HAS_STD_INVOKE_RESULT
)
target_include_directories(boost SYSTEM INTERFACE ${Boost_INCLUDE_DIR})

enable_testing()

add_library(nlohmann_json INTERFACE IMPORTED)
target_include_directories(nlohmann_json SYSTEM INTERFACE "json/single_include")

get_directory_property(has_parent PARENT_DIRECTORY)
if (NOT has_parent)
	add_subdirectory(Catch2)
	add_subdirectory(cmake_strict_defaults)
	add_subdirectory(numeric-traits)
	add_subdirectory(operators)
	add_subdirectory(bounded-integer)
endif()

# TODO: This should be an object library, but they do not really work yet.
add_library(tm_common STATIC
	source/tm/bide/bide.cpp
	source/tm/bide/damage.cpp
	source/tm/bide/duration.cpp

	source/tm/clients/netbattle/read_team_file.cpp

	source/tm/clients/pokemon_lab/read_team_file.cpp
	source/tm/clients/pokemon_lab/write_team_file.cpp

	source/tm/clients/pokemon_online/conversion.cpp
	source/tm/clients/pokemon_online/invalid_team_file.cpp
	source/tm/clients/pokemon_online/read_team_file.cpp
	source/tm/clients/pokemon_online/write_team_file.cpp

	source/tm/clients/pokemon_showdown/battle_factory.cpp
	source/tm/clients/pokemon_showdown/battle_logger.cpp
	source/tm/clients/pokemon_showdown/battle_parser.cpp
	source/tm/clients/pokemon_showdown/battles.cpp
	source/tm/clients/pokemon_showdown/chat.cpp
	source/tm/clients/pokemon_showdown/client.cpp
	source/tm/clients/pokemon_showdown/inmessage.cpp
	source/tm/clients/pokemon_showdown/move_state.cpp
	source/tm/clients/pokemon_showdown/packed_format_to_team.cpp
	source/tm/clients/pokemon_showdown/parse_team.cpp
	source/tm/clients/pokemon_showdown/parsed_hp.cpp
	source/tm/clients/pokemon_showdown/slot_memory.cpp
	source/tm/clients/pokemon_showdown/sockets.cpp
	source/tm/clients/pokemon_showdown/to_packed_format.cpp

	source/tm/clients/shoddy_battle/read_team_file.cpp

	source/tm/clients/battle_result.cpp
	source/tm/clients/invalid_team_file_format.cpp
	source/tm/clients/log_foe_team.cpp
	source/tm/clients/party.cpp

	source/tm/evaluate/best_move.cpp
	source/tm/evaluate/compressed_battle.cpp
	source/tm/evaluate/depth.cpp
	source/tm/evaluate/evaluate.cpp
	source/tm/evaluate/expectiminimax.cpp
	source/tm/evaluate/move_scores.cpp
	source/tm/evaluate/possible_executed_moves.cpp
	source/tm/evaluate/transposition.cpp

	source/tm/move/accuracy.cpp
	source/tm/move/actual_damage.cpp
	source/tm/move/base_power.cpp
	source/tm/move/calculate_damage.cpp
	source/tm/move/call_move.cpp
	source/tm/move/category.cpp
	source/tm/move/container.cpp
	source/tm/move/executed_move.cpp
	source/tm/move/is_switch.cpp
	source/tm/move/known_move.cpp
	source/tm/move/max_moves_per_pokemon.cpp
	source/tm/move/move.cpp
	source/tm/move/other_move.cpp
	source/tm/move/power.cpp
	source/tm/move/pp.cpp
	source/tm/move/priority.cpp
	source/tm/move/regular_moves.cpp
	source/tm/move/shared.cpp
	source/tm/move/side_effects.cpp
	source/tm/move/target.cpp
	source/tm/move/used_move.cpp
	source/tm/move/will_be_recharge_turn.cpp

	source/tm/pokemon/active_pokemon.cpp
	source/tm/pokemon/active_status.cpp
	source/tm/pokemon/any_pokemon.cpp
	source/tm/pokemon/collection.cpp
	source/tm/pokemon/confusion.cpp
	source/tm/pokemon/delayed_attack.cpp
	source/tm/pokemon/disable.cpp
	source/tm/pokemon/embargo.cpp
	source/tm/pokemon/encore.cpp
	source/tm/pokemon/end_of_turn_counter.cpp
	source/tm/pokemon/happiness.cpp
	source/tm/pokemon/has_physical_or_special_move.cpp
	source/tm/pokemon/heal_block.cpp
	source/tm/pokemon/hidden_power.cpp
	source/tm/pokemon/is_genderless.cpp
	source/tm/pokemon/last_used_move.cpp
	source/tm/pokemon/level.cpp
	source/tm/pokemon/magnet_rise.cpp
	source/tm/pokemon/max_pokemon_per_team.cpp
	source/tm/pokemon/partial_trap.cpp
	source/tm/pokemon/perish_song.cpp
	source/tm/pokemon/pokemon.cpp
	source/tm/pokemon/pokemon_not_found.cpp
	source/tm/pokemon/rampage.cpp
	source/tm/pokemon/slow_start.cpp
	source/tm/pokemon/stockpile.cpp
	source/tm/pokemon/substitute.cpp
	source/tm/pokemon/taunt.cpp
	source/tm/pokemon/uproar.cpp
	source/tm/pokemon/yawn.cpp

	source/tm/stat/base_stats.cpp
	source/tm/stat/calculate.cpp
	source/tm/stat/calculate_ivs_and_evs.cpp
	source/tm/stat/chance_to_hit.cpp
	source/tm/stat/combined_stats.cpp
	source/tm/stat/ev.cpp
	source/tm/stat/generic_stats.cpp
	source/tm/stat/possible_dvs_or_ivs.cpp
	source/tm/stat/hp.cpp
	source/tm/stat/initial_stat.cpp
	source/tm/stat/ingame_id_to_nature.cpp
	source/tm/stat/iv.cpp
	source/tm/stat/iv_and_ev.cpp
	source/tm/stat/nature.cpp
	source/tm/stat/stage.cpp
	source/tm/stat/stat_names.cpp
	source/tm/stat/stat_to_ev.cpp
	source/tm/stat/stats.cpp

	source/tm/string_conversions/ability.cpp
	source/tm/string_conversions/conversion.cpp
	source/tm/string_conversions/gender.cpp
	source/tm/string_conversions/generation.cpp
	source/tm/string_conversions/invalid_string_conversion.cpp
	source/tm/string_conversions/item.cpp
	source/tm/string_conversions/lowercase_alphanumeric.cpp
	source/tm/string_conversions/move.cpp
	source/tm/string_conversions/nature.cpp
	source/tm/string_conversions/pokemon.cpp
	source/tm/string_conversions/species.cpp
	source/tm/string_conversions/status.cpp
	source/tm/string_conversions/team.cpp
	source/tm/string_conversions/type.cpp

	source/tm/team_predictor/ev_optimizer/defensive.cpp
	source/tm/team_predictor/ev_optimizer/defensive_data_point.cpp
	source/tm/team_predictor/ev_optimizer/ev_optimizer.cpp
	source/tm/team_predictor/ev_optimizer/offensive.cpp
	source/tm/team_predictor/ev_optimizer/possible_optimized_ivs.cpp
	source/tm/team_predictor/ev_optimizer/speed.cpp

	source/tm/team_predictor/estimate.cpp
	source/tm/team_predictor/lead_stats.cpp
	source/tm/team_predictor/team_predictor.cpp
	source/tm/team_predictor/usage_stats.cpp

	source/tm/type/effectiveness.cpp
	source/tm/type/pokemon_types.cpp
	source/tm/type/type.cpp

	source/tm/ability.cpp
	source/tm/ability_blocks_move.cpp
	source/tm/any_team.cpp
	source/tm/apply_entry_hazards.cpp
	source/tm/battle.cpp
	source/tm/bit_view.cpp
	source/tm/block.cpp
	source/tm/buffer_view.cpp
	source/tm/bytes_in_file.cpp
	source/tm/compress.cpp
	source/tm/constant_generation.cpp
	source/tm/critical_hit.cpp
	source/tm/end_of_turn.cpp
	source/tm/entry_hazards.cpp
	source/tm/exists_if.cpp
	source/tm/files_in_path.cpp
	source/tm/gender.cpp
	source/tm/generation.cpp
	source/tm/generation_generic.cpp
	source/tm/heal.cpp
	source/tm/held_item.cpp
	source/tm/invalid_settings_file.cpp
	source/tm/item.cpp
	source/tm/load_team_from_file.cpp
	source/tm/operators.cpp
	source/tm/other_team.cpp
	source/tm/phazing_in_same_pokemon.cpp
	source/tm/random_damage.cpp
	source/tm/saturating_add.cpp
	source/tm/screen.cpp
	source/tm/screens.cpp
	source/tm/settings_file.cpp
	source/tm/status.cpp
	source/tm/team.cpp
	source/tm/visible_hp.cpp
	source/tm/weather.cpp
	source/tm/wish.cpp
)
target_link_libraries(tm_common
	boost
	bounded
	containers
	nlohmann_json
	strict_defaults
)

target_include_directories(tm_common PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>
	$<INSTALL_INTERFACE:source>
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/teams DESTINATION ${CMAKE_BINARY_DIR}/)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/teams/foe)

add_executable(ai
	source/tm/ai.cpp
)
target_link_libraries(ai
	tm_common
)

add_executable(file_converter
	source/tm/file_converter/main.cpp
)
target_link_libraries(file_converter
	tm_common
)


add_executable(predict
	source/tm/team_predictor/generate_team_builder_ui.cpp
	source/tm/team_predictor/predictor.cpp
	source/tm/team_predictor/random_team.cpp
)
target_link_libraries(predict
	tm_common
)

add_executable(tm_test
	source/tm/test/clients/netbattle/read_team_file.cpp

	source/tm/test/clients/pokemon_showdown/battles.cpp
	source/tm/test/clients/pokemon_showdown/packed_team.cpp
	source/tm/test/clients/pokemon_showdown/parse_team.cpp
	source/tm/test/clients/pokemon_showdown/slot_memory.cpp

	source/tm/test/collections/move_container.cpp
	source/tm/test/collections/variable_collection.cpp

	source/tm/test/evaluate/evaluate.cpp
	source/tm/test/evaluate/expectiminimax.cpp

	source/tm/test/pokemon_lab/team_file.cpp

	source/tm/test/pokemon_online/conversion.cpp
	source/tm/test/pokemon_online/team_file.cpp

	source/tm/test/block.cpp
	source/tm/test/calculate_damage.cpp
	source/tm/test/calculate_ivs_and_evs.cpp
	source/tm/test/call_move.cpp
	source/tm/test/critical_hit.cpp
	source/tm/test/ev_optimizer.cpp
	source/tm/test/files_in_path.cpp
	source/tm/test/get_directory.cpp
	source/tm/test/sleep.cpp
	source/tm/test/stat.cpp
	source/tm/test/status.cpp
	source/tm/test/string_conversion.cpp
)
target_link_libraries(tm_test
	tm_common
	Catch2::Catch2WithMain
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
	target_compile_options(tm_common PUBLIC
		"-O3"
	)
	target_link_libraries(ai pthread)
	target_link_libraries(predict pthread)

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(tm_common PUBLIC
		"-O3"
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
