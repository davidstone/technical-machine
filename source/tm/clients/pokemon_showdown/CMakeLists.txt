# Copyright David Stone 2026.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

add_library(tm_pokemon_showdown STATIC)
target_sources(tm_pokemon_showdown PUBLIC
	FILE_SET CXX_MODULES
	BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
	FILES
		action_required.cpp
		battle_init_message.cpp
		battle_manager.cpp
		battle_message_handler.cpp
		battle_message_kind.cpp
		battle_response_switch.cpp
		battle_started.cpp
		battles.cpp
		client.cpp
		client_message_handler.cpp
		end_of_turn_state.cpp
		end_of_turn_state_builder.cpp
		event_block.cpp
		in_message.cpp
		is_chat_message.cpp
		is_chat_message_block.cpp
		log_battle_messages.cpp
		make_battle_init_message.cpp
		make_party.cpp
		message_block.cpp
		move_state.cpp
		move_state_builder.cpp
		optional_hp_and_status.cpp
		packed_format_to_team.cpp
		parse_details.cpp
		parse_effect_source.cpp
		parse_gender.cpp
		parse_generation_from_format.cpp
		parse_hp.cpp
		parse_hp_and_status.cpp
		parse_moves.cpp
		parse_pokemon_identity.cpp
		parse_set_hp_message.cpp
		parse_status.cpp
		parse_switch.cpp
		parse_request.cpp
		parsed_message.cpp
		parsed_request.cpp
		parsed_side.cpp
		parsed_stats.cpp
		parsed_team.cpp
		parsed_team_to_known_team.cpp
		party_from_player_id.cpp
		room.cpp
		room_message_block.cpp
		send_message_function.cpp
		send_selection.cpp
		slot_memory.cpp
		start_of_turn.cpp
		switch_message.cpp
		sockets.cpp
		to_packed_format.cpp
)
target_sources(tm_pokemon_showdown PRIVATE
	battle_message_handler_impl.cpp
	to_packed_format_impl.cpp
)
target_link_libraries(tm_pokemon_showdown
	concurrent
	tm_boost_networking
	tm_clients
	strict_defaults
)

add_subdirectory(test)