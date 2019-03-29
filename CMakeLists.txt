# Copyright David Stone 2019.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(technical_machine LANGUAGES CXX)

find_package(Boost REQUIRED)
add_library(boost INTERFACE IMPORTED)
target_compile_definitions(boost INTERFACE
	BOOST_ALL_NO_LIB
	BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
	BOOST_CHRONO_DONT_PROVIDES_DEPRECATED_IO_SINCE_V2_0_0
	BOOST_CHRONO_HEADER_ONLY
	BOOST_ERROR_CODE_HEADER_ONLY
	BOOST_SYSTEM_NO_DEPRECATED
)
target_include_directories(boost SYSTEM INTERFACE ${Boost_INCLUDE_DIR})

set(FLTK_SKIP_OPENGL On)
set(FLTK_SKIP_FORMS On)
set(FLTK_SKIP_IMAGES On)
set(FLTK_SKIP_FLUID On)
find_package(FLTK REQUIRED)
add_library(fltk INTERFACE IMPORTED)
target_include_directories(fltk SYSTEM INTERFACE ${FLTK_INCLUDE_DIR})
target_link_libraries(fltk INTERFACE ${FLTK_LIBRARIES})

enable_testing()

add_library(bounded INTERFACE)
add_library(containers INTERFACE)
set_property(TARGET bounded PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/bounded_integer/include)
set_property(TARGET containers PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/bounded_integer/include)

# TODO: This should be an object library, but they do not really work yet.
add_library(tm_common STATIC
	source/tm/bide/bide.cpp
	source/tm/bide/damage.cpp
	source/tm/bide/duration.cpp

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
	source/tm/clients/pokemon_showdown/json_parser.cpp
	source/tm/clients/pokemon_showdown/move_state.cpp
	source/tm/clients/pokemon_showdown/packed_team.cpp
	source/tm/clients/pokemon_showdown/parsed_hp.cpp
	source/tm/clients/pokemon_showdown/slot_memory.cpp

	source/tm/clients/battle.cpp
	source/tm/clients/battle_result.cpp
	source/tm/clients/invalid_team_file_format.cpp
	source/tm/clients/log_foe_team.cpp
	source/tm/clients/party.cpp
	source/tm/clients/timestamp.cpp

	source/tm/evaluate/depth.cpp
	source/tm/evaluate/evaluate.cpp
	source/tm/evaluate/expectiminimax.cpp
	source/tm/evaluate/move_scores.cpp
	source/tm/evaluate/reorder.cpp
	source/tm/evaluate/transposition.cpp

	source/tm/move/accuracy.cpp
	source/tm/move/actual_damage.cpp
	source/tm/move/base_power.cpp
	source/tm/move/calculate_damage.cpp
	source/tm/move/call_move.cpp
	source/tm/move/category.cpp
	source/tm/move/container.cpp
	source/tm/move/damage_type.cpp
	source/tm/move/executed_move.cpp
	source/tm/move/is_switch.cpp
	source/tm/move/max_moves_per_pokemon.cpp
	source/tm/move/move.cpp
	source/tm/move/other_move.cpp
	source/tm/move/power.cpp
	source/tm/move/pp.cpp
	source/tm/move/priority.cpp
	source/tm/move/shared.cpp
	source/tm/move/target.cpp
	source/tm/move/used_move.cpp
	source/tm/move/will_be_recharge_turn.cpp

	source/tm/pokemon/active_pokemon.cpp
	source/tm/pokemon/collection.cpp
	source/tm/pokemon/confusion.cpp
	source/tm/pokemon/damage_blocker.cpp
	source/tm/pokemon/delayed_attack.cpp
	source/tm/pokemon/disable.cpp
	source/tm/pokemon/embargo.cpp
	source/tm/pokemon/encore.cpp
	source/tm/pokemon/end_of_turn_counter.cpp
	source/tm/pokemon/happiness.cpp
	source/tm/pokemon/heal_block.cpp
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

	source/tm/stat/calculate.cpp
	source/tm/stat/chance_to_hit.cpp
	source/tm/stat/combined_stats.cpp
	source/tm/stat/ev.cpp
	source/tm/stat/hp.cpp
	source/tm/stat/iv.cpp
	source/tm/stat/nature.cpp
	source/tm/stat/stage.cpp
	source/tm/stat/stat.cpp
	source/tm/stat/stat_names.cpp
	source/tm/stat/stat_to_ev.cpp
	source/tm/stat/stats.cpp

	source/tm/string_conversions/ability.cpp
	source/tm/string_conversions/conversion.cpp
	source/tm/string_conversions/gender.cpp
	source/tm/string_conversions/invalid_string_conversion.cpp
	source/tm/string_conversions/item.cpp
	source/tm/string_conversions/move.cpp
	source/tm/string_conversions/nature.cpp
	source/tm/string_conversions/pokemon.cpp
	source/tm/string_conversions/status.cpp

	source/tm/team_predictor/ev_optimizer/defensive.cpp
	source/tm/team_predictor/ev_optimizer/defensive_data_point.cpp
	source/tm/team_predictor/ev_optimizer/ev_optimizer.cpp
	source/tm/team_predictor/ev_optimizer/offensive.cpp
	source/tm/team_predictor/ev_optimizer/speed.cpp

	source/tm/team_predictor/detailed_stats.cpp
	source/tm/team_predictor/estimate.cpp
	source/tm/team_predictor/lead_stats.cpp
	source/tm/team_predictor/multiplier.cpp
	source/tm/team_predictor/team_predictor.cpp
	source/tm/team_predictor/usage_stats.cpp

	source/tm/type/collection.cpp
	source/tm/type/effectiveness.cpp
	source/tm/type/type.cpp

	source/tm/ability.cpp
	source/tm/block.cpp
	source/tm/endofturn.cpp
	source/tm/entry_hazards.cpp
	source/tm/enum.cpp
	source/tm/files_in_directory.cpp
	source/tm/gender.cpp
	source/tm/generation.cpp
	source/tm/heal.cpp
	source/tm/invalid_settings_file.cpp
	source/tm/item.cpp
	source/tm/operators.cpp
	source/tm/phazing_in_same_pokemon.cpp
	source/tm/random_damage.cpp
	source/tm/screen.cpp
	source/tm/screens.cpp
	source/tm/settings_file.cpp
	source/tm/status.cpp
	source/tm/switch.cpp
	source/tm/team.cpp
	source/tm/variable.cpp
	source/tm/weather.cpp
	source/tm/wish.cpp
)
target_link_libraries(tm_common
	boost
	bounded
	containers
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


add_executable(predict
	source/tm/team_predictor/ui/ev_inputs.cpp
	source/tm/team_predictor/ui/move_inputs.cpp
	source/tm/team_predictor/ui/nature_input.cpp
	source/tm/team_predictor/ui/pokemon_inputs.cpp
	source/tm/team_predictor/ui/species_input.cpp

	source/tm/team_predictor/predictor.cpp
	source/tm/team_predictor/random_team.cpp
)
target_link_libraries(predict
	tm_common
	fltk
)

add_executable(tm_test
	source/tm/test/clients/pokemon_showdown/battles.cpp
	source/tm/test/clients/pokemon_showdown/slot_memory.cpp

	source/tm/test/collections/collection.cpp
	source/tm/test/collections/invalid_collection.cpp
	source/tm/test/collections/move_container.cpp
	source/tm/test/collections/variable_collection.cpp

	source/tm/test/evaluate/evaluate.cpp
	source/tm/test/evaluate/expectiminimax.cpp

	source/tm/test/pokemon_lab/team_file.cpp
	source/tm/test/pokemon_lab/test.cpp

	source/tm/test/pokemon_online/conversion.cpp
	source/tm/test/pokemon_online/team_file.cpp
	source/tm/test/pokemon_online/test.cpp

	source/tm/test/block.cpp
	source/tm/test/calculate_damage.cpp
	source/tm/test/call_move.cpp
	source/tm/test/ev_optimizer.cpp
	source/tm/test/incorrect_calculation.cpp
	source/tm/test/stat.cpp
	source/tm/test/status.cpp
	source/tm/test/string_conversion.cpp
	source/tm/test/test.cpp
)
target_link_libraries(tm_test
	tm_common
)


set(COMMON_CLANG_COMPILE_OPTIONS
	"-Weverything"
	"-Werror"
	"-Wno-c++98-compat"
	"-Wno-c++98-compat-pedantic"
	# Complains about comments like "//<N3757>" referring to an ISO paper number
	"-Wno-documentation"
	# Floating-point comparisons are safe in these tests
	"-Wno-float-equal"
	# -Wmissing-braces wants two sets of braces when using constructs such
	# as std::array
	"-Wno-missing-braces"
	"-Wno-newline-eof"
	"-Wno-padded"
	"-Wno-range-loop-analysis"
	# Still don't need compatibility with older standards
	"-Wno-return-std-move-in-c++11"
	# -Wswitch-enum warns even if you have default
	"-Wno-switch-enum"
	# -Wunneeded-member-function has a bug that blocks constexpr libraries
	# from using it: https://llvm.org/bugs/show_bug.cgi?id=25084
	"-Wno-unneeded-member-function"
	# Has a bug that warns even for templates that are used
	"-Wno-unused-template"
	# -Wweak-vtables is incompatible with header-only libraries
	"-Wno-weak-vtables"
	# -Wzero-as-null-pointer-constant does not work with the `operator<=>`
	# emulation
	"-Wno-zero-as-null-pointer-constant"
	"-Wno-exit-time-destructors"
)

if(${CMAKE_CXX_COMPILER_VERSION} VERSION_GREATER 8)
	set(COMMON_CLANG_COMPILE_OPTIONS
		${COMMON_CLANG_COMPILE_OPTIONS}
		"-Wno-ctad-maybe-unsupported"
	)
endif()

if(MSVC)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
	target_compile_options(tm_common PUBLIC
		"/std:c++latest"
		"/MP"
	)
	target_link_options(tm_common PUBLIC
		"/STACK:8388608"
	)
	target_link_options(tm_common PUBLIC
		"/WX"
	)
	set_target_properties(tm_test PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	)

	if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(tm_common PUBLIC
			${COMMON_CLANG_COMPILE_OPTIONS}
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
			# FLTK has this warning
			"-Wno-shift-sign-overflow"
		)
		# Sanitizers currently do not work very well on Windows
		set(USE_SANITIZERS OFF)
	else()
		set(COMPILE_OPTIONS
			"/FI ciso646"
		)
		set(USE_SANITIZERS OFF)
	endif()

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_link_libraries(tm_common
		c++fs
	)
	target_link_libraries(ai
		pthread
	)
	target_compile_options(tm_common PUBLIC
		${COMMON_CLANG_COMPILE_OPTIONS}
		"-g"
		"-std=c++2a"
		"-march=native"
	)
	target_link_options(tm_common PUBLIC
		"-Wl,--fatal-warnings"
	)

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_link_libraries(tm_common
		stdc++fs
	)
	target_link_libraries(ai
		pthread
	)
	target_compile_options(tm_common PUBLIC
		"-g"
		"-std=c++2a"
		"-march=native"
		"-Wall"
		"-Wextra"
		"-Wpedantic"
		"-Wabi"
		"-Wcast-align"
		"-Wcast-qual"
		# It seems impossible to initialize bit fields with this warning on
		#"-Wconversion"
		"-Wctor-dtor-privacy"
		"-Wdisabled-optimization"
		"-Wdouble-promotion"
		# -Weffc++ includes a warning if all data members are not explicitly
		# initialized in the initializer list. I intentionally do not do this in
		# many cases. This would be more useful as a collection of warnings
		# like -Wall instead of a single warning on its own.
		# "-Weffc++"
		# -Wfloat-equal warns for safe comparisons
		# "-Wfloat-equal"
		"-Wformat=2"
		"-Winit-self"
		"-Winvalid-pch"
		# -Wlogical-op warns for expressions that happen to be equal in a
		# template instantiation
		# "-Wlogical-op"
		"-Wmissing-declarations"
		# -Wmissing-format-attribute is not used because I do not use GNU
		# extensions. Same for -Wsuggest-attribute and several others.
		"-Wmissing-include-dirs"
		#"-Wnoexcept"
		"-Wno-non-template-friend"
		"-Wodr"
		"-Wold-style-cast"
		"-Woverloaded-virtual"
		"-Wno-padded"
		"-Wredundant-decls"
		# -Wreturn-type warns even for a switch over an enum for which all cases
		# are handled explicitly and each case is a return statement.
		"-Wno-return-type"
		"-Wshadow"
		"-Wsign-conversion"
		# -Wsign-promo triggers on code that is guaranteed safe due to the use
		# of the bounded::integer library. Working around the warning would lead
		# to either less efficient code or more obfuscated code.
		"-Wsign-promo"
		# -Wsuggest-final-methods and -Wsuggest-final-types is a linker warning,
		# so it is not possible to disable it for boost and other third-party
		# libraries by saying they are system headers.
		# "-Wsuggest-final-methods"
		# "-Wsuggest-final-types"
		"-Wstrict-null-sentinel"
		# -Wstrict-overflow=2 warns about comparing two pointers
		"-Wstrict-overflow=1"
		# -Wswitch-default warns even for a switch over an enum for which all
		# cases are handled explicitly
		# "-Wswitch-default"
		# -Wswitch-enum warns even if there is a default case
		# "-Wswitch-enum"
		"-Wtrampolines"
		"-Wundef"
		# -Wunsafe-loop-optimizations causes too many spurious warnings. It may
		# be useful to apply this one periodically and manually verify the
		# results. It generated this warning in my code when I looped over all
		# elements in a vector to apply a set of functions to them (using the
		# range-based for loop).  It is also warning for the constructor of a
		# const array of const std::string where there is no loop in user code.
		# "-Wunsafe-loop-optimizations"
		# -Wuseless-cast is incompatible with BOUNDED_INTEGER_CONDITIONAL
		# "-Wuseless-cast"
		"-Wvector-operation-performance"
		# -Wzero-as-null-pointer-constant does not work with the `operator<=>`
		# emulation
		# "-Wzero-as-null-pointer-constant"
		"-Werror"
	)
	target_link_options(tm_common PUBLIC
		"-Wl,--fatal-warnings"
	)
endif()

set(sanitizers "-fsanitize=undefined" "-fsanitize=address")

if(DEFINED USE_SANITIZERS)
	if(${USE_SANITIZERS})
		target_compile_options(tm_common PUBLIC ${sanitizers})
		target_link_options(tm_common PUBLIC ${sanitizers})
	endif()
else()
	target_compile_options(tm_common PUBLIC
		$<$<CONFIG:Debug>:${sanitizers}>
	)
	target_link_options(tm_common PUBLIC
		$<$<CONFIG:Debug>:${sanitizers}>
	)
endif()

add_test(tm_test tm_test)
