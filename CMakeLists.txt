# Copyright David Stone 2018.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(technical_machine LANGUAGES CXX)

find_package(Boost COMPONENTS filesystem system thread REQUIRED)
add_library(boost INTERFACE IMPORTED)
set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})
enable_testing()

add_library(bounded INTERFACE)
add_library(containers INTERFACE)
add_library(endian INTERFACE)
set_property(TARGET bounded PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/bounded_integer/include)
set_property(TARGET containers PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/bounded_integer/include)
set_property(TARGET endian PROPERTY INTERFACE_INCLUDE_DIRECTORIES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/endian)

# TODO: This should be an object library, but they do not really work yet.
add_library(tm_common STATIC
	source/bide/bide.cpp
	source/bide/damage.cpp
	source/bide/duration.cpp

	source/clients/pokemon_lab/read_team_file.cpp
	source/clients/pokemon_lab/write_team_file.cpp

	source/clients/pokemon_online/conversion.cpp
	source/clients/pokemon_online/invalid_team_file.cpp
	source/clients/pokemon_online/read_team_file.cpp
	source/clients/pokemon_online/write_team_file.cpp

	source/clients/invalid_team_file_format.cpp

	source/evaluate/evaluate.cpp
	source/evaluate/expectiminimax.cpp
	source/evaluate/move_scores.cpp
	source/evaluate/reorder.cpp
	source/evaluate/transposition.cpp

	source/move/accuracy.cpp
	source/move/base_power.cpp
	source/move/container.cpp
	source/move/is_switch.cpp
	source/move/max_moves_per_pokemon.cpp
	source/move/move.cpp
	source/move/pp.cpp
	source/move/power.cpp
	source/move/priority.cpp
	source/move/shared.cpp
	source/move/use_move.cpp

	source/pokemon/active_pokemon.cpp
	source/pokemon/active_pokemon_flags.cpp
	source/pokemon/collection.cpp
	source/pokemon/confusion.cpp
	source/pokemon/container.cpp
	source/pokemon/delayed_attack.cpp
	source/pokemon/disable.cpp
	source/pokemon/embargo.cpp
	source/pokemon/encore.cpp
	source/pokemon/end_of_turn_counter.cpp
	source/pokemon/happiness.cpp
	source/pokemon/heal_block.cpp
	source/pokemon/last_used_move.cpp
	source/pokemon/level.cpp
	source/pokemon/magnet_rise.cpp
	source/pokemon/max_pokemon_per_team.cpp
	source/pokemon/partial_trap.cpp
	source/pokemon/perish_song.cpp
	source/pokemon/pokemon.cpp
	source/pokemon/pokemon_not_found.cpp
	source/pokemon/rampage.cpp
	source/pokemon/slow_start.cpp
	source/pokemon/stockpile.cpp
	source/pokemon/substitute.cpp
	source/pokemon/taunt.cpp
	source/pokemon/toxic.cpp
	source/pokemon/uproar.cpp
	source/pokemon/vanish.cpp
	source/pokemon/yawn.cpp

	source/stat/calculate.cpp
	source/stat/chance_to_hit.cpp
	source/stat/ev.cpp
	source/stat/hp.cpp
	source/stat/iv.cpp
	source/stat/nature.cpp
	source/stat/stage.cpp
	source/stat/stat.cpp
	source/stat/stat_names.cpp
	source/stat/stats.cpp

	source/string_conversions/ability.cpp
	source/string_conversions/gender.cpp
	source/string_conversions/invalid_string_conversion.cpp
	source/string_conversions/item.cpp
	source/string_conversions/move.cpp
	source/string_conversions/nature.cpp
	source/string_conversions/pokemon.cpp
	source/string_conversions/status.cpp

	source/team_predictor/ev_optimizer/combine.cpp
	source/team_predictor/ev_optimizer/defensive.cpp
	source/team_predictor/ev_optimizer/defensive_data_point.cpp
	source/team_predictor/ev_optimizer/ev_optimizer.cpp
	source/team_predictor/ev_optimizer/offensive.cpp
	source/team_predictor/ev_optimizer/single_classification_evs.cpp
	source/team_predictor/ev_optimizer/speed.cpp

	source/team_predictor/detailed_stats.cpp
	source/team_predictor/estimate.cpp
	source/team_predictor/load_stats.cpp
	source/team_predictor/multiplier.cpp
	source/team_predictor/team_predictor.cpp

	source/type/collection.cpp
	source/type/effectiveness.cpp
	source/type/type.cpp

	source/ability.cpp
	source/block.cpp
	source/collection.cpp
	source/damage.cpp
	source/endofturn.cpp
	source/entry_hazards.cpp
	source/enum.cpp
	source/enum_range.cpp
	source/gender.cpp
	source/heal.cpp
	source/invalid_settings_file.cpp
	source/item.cpp
	source/operators.cpp
	source/phazing_in_same_pokemon.cpp
	source/random_damage.cpp
	source/screen.cpp
	source/screens.cpp
	source/settings_file.cpp
	source/status.cpp
	source/switch.cpp
	source/team.cpp
	source/variable.cpp
	source/weather.cpp
	source/wish.cpp
)
target_link_libraries(tm_common
	boost
	boost_filesystem
	boost_system
	bounded
	containers
	endian
)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/teams DESTINATION ${CMAKE_BINARY_DIR}/)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/teams/foe)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/source/test/test1.sbt DESTINATION ${CMAKE_BINARY_DIR}/test)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/source/test/test1.tp DESTINATION ${CMAKE_BINARY_DIR}/test)

add_executable(ai
	source/clients/pokemon_online/battle.cpp
	source/clients/pokemon_online/battle_settings.cpp
	source/clients/pokemon_online/client.cpp
	source/clients/pokemon_online/inmessage.cpp
	source/clients/pokemon_online/invalid_user.cpp
	source/clients/pokemon_online/outmessage.cpp
	source/clients/pokemon_online/read_user_info.cpp

	source/clients/pokemon_showdown/battle.cpp
	source/clients/pokemon_showdown/battle_settings.cpp
	source/clients/pokemon_showdown/client.cpp
	source/clients/pokemon_showdown/inmessage.cpp
	source/clients/pokemon_showdown/outmessage.cpp

	source/clients/battle.cpp
	source/clients/battle_result.cpp
	source/clients/battles.cpp
	source/clients/client.cpp
	source/clients/network/buffer_overrun.cpp
	source/clients/network/client.cpp
	source/clients/network/inmessage.cpp
	source/clients/network/invalid_channel.cpp
	source/clients/network/invalid_packet.cpp
	source/clients/network/outmessage.cpp
	source/clients/no_pending_challenges.cpp
	source/clients/party.cpp
	source/clients/timestamp.cpp
	source/clients/updated_hp.cpp

	source/cryptography/md5.cpp

	source/ai.cpp
)
target_link_libraries(ai
	boost_thread
	pthread
	tm_common
)



add_executable(predict
	source/team_predictor/ui/ev_inputs.cpp
	source/team_predictor/ui/move_inputs.cpp
	source/team_predictor/ui/nature_input.cpp
	source/team_predictor/ui/pokemon_inputs.cpp
	source/team_predictor/ui/species_input.cpp

	source/team_predictor/predictor.cpp
	source/team_predictor/random_team.cpp
)
target_link_libraries(predict
	fltk
	tm_common
)


add_executable(tm_test
	source/test/collections/collection.cpp
	source/test/collections/invalid_collection.cpp
	source/test/collections/move_container.cpp
	source/test/collections/variable_collection.cpp

	source/test/evaluate/evaluate.cpp
	source/test/evaluate/expectiminimax.cpp

	source/test/pokemon_lab/team_file.cpp
	source/test/pokemon_lab/test.cpp

	source/test/pokemon_online/conversion.cpp
	source/test/pokemon_online/team_file.cpp
	source/test/pokemon_online/test.cpp

	source/test/damage.cpp
	source/test/ev_optimizer.cpp
	source/test/incorrect_calculation.cpp
	source/test/stat.cpp
	source/test/status.cpp
	source/test/string_conversion.cpp
	source/test/test.cpp
)
target_link_libraries(tm_test
	tm_common
)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(tm_common
		PUBLIC "/std:c++17"
	)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_options(tm_common PUBLIC
		"-std=c++17"
		$<$<CONFIG:Debug>:-O1;-march=native>
		$<$<CONFIG:Release>:-march=native>
		"-Weverything"
		"-Werror"
		"-Wno-c++98-compat"
		"-Wno-c++98-compat-pedantic"
		# Floating-point comparisons are safe in these tests
		"-Wno-float-equal"
		# -Wmissing-braces wants two sets of braces when using constructs such
		# as std::array
		"-Wno-missing-braces"
		"-Wno-newline-eof"
		"-Wno-padded"
		# -Wswitch-enum warns even if you have default
		"-Wno-switch-enum"
		# -Wundefined-func-template has a bug that triggers on deduction guides
		"-Wno-undefined-func-template"
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

	foreach(SANITIZER -fsanitize=undefined -fsanitize=address)
		target_compile_options(tm_common PUBLIC
			$<$<CONFIG:Debug>:${SANITIZER}>
		)
		target_link_libraries(tm_common
			$<$<CONFIG:Debug>:${SANITIZER}>
		)
	endforeach()

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(tm_common PUBLIC
		"-std=c++17"
		$<$<CONFIG:Debug>:-Og;-march=native>
		$<$<CONFIG:Release>:-O3;-march=native>
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
		"-Wswitch-default"
		"-Wswitch-enum"
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

	foreach(SANITIZER -fsanitize=undefined -fsanitize=address)
		target_compile_options(tm_common PUBLIC
			$<$<CONFIG:Debug>:${SANITIZER}>
		)
		target_link_libraries(tm_common
			$<$<CONFIG:Debug>:${SANITIZER}>
		)
	endforeach()
endif()

add_test(tm_test tm_test)
