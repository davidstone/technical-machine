// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.clients.ps.client_message_handler;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.battle_started;
import tm.clients.ps.battles;
import tm.clients.ps.log_battle_messages;
import tm.clients.ps.is_chat_message_block;
import tm.clients.ps.in_message;
import tm.clients.ps.make_battle_message;
import tm.clients.ps.message_block;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.room;
import tm.clients.ps.room_message_block;
import tm.clients.ps.send_message_function;
import tm.clients.ps.send_selection;
import tm.clients.ps.start_of_turn;
import tm.clients.ps.to_packed_format;

import tm.clients.battle_already_finished;
import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.battle_response_error;
import tm.clients.determine_selection;
import tm.clients.get_team;
import tm.clients.should_accept_challenge;
import tm.clients.turn_count;

import tm.strategy.strategy;

import tm.team_predictor.team_predictor;
import tm.team_predictor.all_usage_stats;

import tm.boost_networking;
import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.get_directory;
import tm.nlohmann_json;
import tm.open_file;
import tm.settings_file;
import tm.visible_state;

import bounded;
import containers;
import tv;
import std_module;

// https://github.com/smogon/pokemon-showdown/blob/master/PROTOCOL.md

namespace technicalmachine::ps {
using namespace std::string_view_literals;
using namespace bounded::literal;

constexpr auto remove_spaces(std::string_view str) {
	return containers::filter(
		std::move(str),
		[](char const c) { return c != ' '; }
	);
}

struct no_spaces_string_view {
	constexpr explicit no_spaces_string_view(std::string_view const str):
		m_str(str)
	{
	}

	friend constexpr auto operator==(no_spaces_string_view const lhs, no_spaces_string_view const rhs) -> bool {
		return containers::equal(
			remove_spaces(lhs.m_str),
			remove_spaces(rhs.m_str)
		);
	}
	friend constexpr auto operator==(no_spaces_string_view const lhs, std::string_view const rhs) -> bool {
		return containers::equal(
			remove_spaces(lhs.m_str),
			rhs
		);
	}

private:
	std::string_view m_str;
};

using AuthenticationFunction = containers::trivial_inplace_function<
	AuthenticationSignature,
	sizeof(void *)
>;

constexpr auto is_battle_message(Room const room) -> bool {
	// TODO: is this correct?
	return room.starts_with("battle-");
}

auto print_begin_turn(std::ostream & stream, TurnCount const turn_count) -> void {
	stream << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
}

export struct ClientMessageHandler {
	ClientMessageHandler(SettingsFile settings, Strategy strategy, SendMessageFunction send_message, AuthenticationFunction authenticate):
		m_random_engine(std::random_device()()),
		m_strategy(std::move(strategy)),
		m_settings(std::move(settings)),
		m_battles_directory(get_battles_directory()),
		m_send_message(std::move(send_message)),
		m_authenticate(std::move(authenticate))
	{
	}
	ClientMessageHandler(ClientMessageHandler &&) = default;
	ClientMessageHandler(ClientMessageHandler const &) = delete;

	auto handle_messages(RoomMessageBlock const block) -> void {
		auto const _ = bounded::scope_fail([=] {
			std::cerr << block.str() << '\n';
		});
		auto const messages = message_block(block.str());
		if (is_chat_message_block(messages)) {
		} else if (is_battle_message(block.room())) {
			log_battle_messages(m_battles_directory, block);
			handle_battle_messages(block.room(), messages);
		} else {
			for (auto const message : messages) {
				handle_message(block.room(), message);
			}
		}
	}

private:
	auto send_team(Generation const generation) -> void {
		auto const team = get_team(generation, m_settings.team, m_all_usage_stats, m_random_engine);
		auto const team_str = team ? to_packed_format(*team) : "null";
		m_send_message(containers::concatenate<containers::string>("|/utm "sv, team_str));
	}

	auto send_request_battle_start(std::string_view const format, auto const & ... strings) -> void {
		send_team(parse_generation_from_format(format, "gen"));
		m_send_message(containers::concatenate<containers::string>(strings...));
	}

	auto send_challenge() -> void {
		tv::visit(m_settings.style, tv::overload(
			[&](SettingsFile::Ladder const & ladder) {
				send_request_battle_start(ladder.format, "|/search "sv, ladder.format);
			},
			[&](SettingsFile::Challenge const & challenge) {
				send_request_battle_start(challenge.format, "|/challenge "sv, challenge.user, ","sv, challenge.format);
			},
			[](SettingsFile::Accept const &) {}
		));
	}

	auto handle_battle_messages(Room const room, MessageBlock const block) -> void {
		auto const battle_message = make_battle_message(block);
		if (!battle_message) {
			return;
		}
		auto const result = m_battles.handle_message(
			room,
			*battle_message
		);
		auto analysis_logger = open_text_file(m_battles_directory / room / "analysis.txt");
		auto determine_and_send_selection = [&](ActionRequired const & value) {
			auto const selection = determine_selection(
				value.state,
				analysis_logger,
				m_all_usage_stats,
				m_strategy,
				m_random_engine
			);
			send_selection(selection, m_send_message, room, value.slot_memory);
		};
		tv::visit(result, tv::overload(
			[&](ActionRequired const & value) {
				determine_and_send_selection(value);
			},
			[&](StartOfTurn const & value) {
				print_begin_turn(analysis_logger, value.turn_count);
				determine_and_send_selection(value);
			},
			[](BattleContinues) {
			},
			[&](BattleResponseError) {
				m_send_message(containers::concatenate<containers::string>(
					room,
					"|/choose default"sv
				));
			},
			[&](BattleStarted const & value) {
				determine_and_send_selection(value);
				if (m_settings.style.index() == bounded::type<SettingsFile::Ladder>) {
					m_send_message(containers::concatenate<containers::string>(room, "|/timer on"sv));
				}
			},
			[&](BattleFinished) {
				m_send_message(containers::concatenate<containers::string>(std::string_view("|/leave "), room));
				send_challenge();
			},
			[](BattleAlreadyFinished) {
			}
		));
	}

	auto handle_message(Room const room, InMessage message) -> void {
		auto const type = message.type();
		if (type == "b" or type == "B" or type == "battle") {
			// message.remainder() == ROOMID|username|username
		} else if (type == "challstr") {
			authenticate(message.remainder());
			// After logging in, send "|/search FORMAT_NAME" to begin laddering
			send_challenge();
		} else if (type == "formats") {
			// message.remainder() == | separated list of formats with special rules
		} else if (type == "html") {
			// message.remainder() == HTML
		} else if (type == "init") {
		} else if (type == "nametaken") {
			auto const username = message.pop();
			std::cerr << "Could not change username to " << username << " because: " << message.remainder() << '\n';
		} else if (type == "popup") {
			std::cout << "popup message: " << message.remainder() << '\n';
		} else if (type == "pm") {
			auto const from = message.pop();
			if (no_spaces_string_view(from) == no_spaces_string_view(m_settings.username)) {
				return;
			}
			auto const to = message.pop();
			auto const initial_message = message.pop();
			if (initial_message.starts_with("/challenge")) {
				auto const format = message.pop();
				if (containers::is_empty(format)) {
					return;
				}
				if (should_accept_challenge(m_settings.style, no_spaces_string_view(from))) {
					send_request_battle_start(format, "|/accept "sv, from);
				} else {
					m_send_message(containers::concatenate<containers::string>("|/reject "sv, from));
				}
			} else {
				std::cout << "PM from " << from << " to " << to << ": " << initial_message;
				if (!containers::is_empty(message.remainder())) {
					std::cout << '|' << message.remainder();
				}
				std::cout << '\n';
			}
		} else if (type == "queryresponse") {
			// message.remainder() == QUERYTYPE|JSON
		} else if (type == "uhtml" or type == "uhtmlchange") {
			// message.remainder() == NAME|HTML
		} else if (type == "updateuser") {
			// message.remainder() == username|guest ? 0 : 1|AVATAR
		} else if (type == "updatesearch") {
			// message.remainder() == JSON: battles you are searching for
		} else if (type == "usercount") {
			// message.remainder() == number of users on server
		} else if (type == "users") {
			// message.remainder() == comma separated list of users
		} else {
			std::cerr << "Received unknown message in room: " << room << " |" << type << '|' << message.remainder() << '\n';
		}
	}

	auto authenticate(std::string_view const challstr) -> void {
		// In theory, if we ever support session cookies, make HTTP GET:
		// http://play.pokemonshowdown.com/api/upkeep?challstr=CHALLSTR
		constexpr auto host = "play.pokemonshowdown.com";

		auto const request = create_http_post(
			host,
			"/api/login",
			containers::concatenate<containers::string>("name="sv, m_settings.username, "&pass="sv, m_settings.password, "&challstr="sv, challstr)
		);
		auto const response = m_authenticate(host, "80", request);

		// Response begins with ']' followed by JSON object.
		auto const body = std::string_view(response.body()).substr(1);
		auto const json = nlohmann::json::parse(body);
		m_send_message(containers::concatenate<containers::string>("|/trn "sv, m_settings.username, ",0,"sv, json.at("assertion").get<std::string_view>()));
	}

	std::mt19937 m_random_engine;

	AllUsageStats m_all_usage_stats;
	Strategy m_strategy;

	SettingsFile m_settings;

	std::filesystem::path m_battles_directory;
	Battles m_battles;

	SendMessageFunction m_send_message;
	AuthenticationFunction m_authenticate;
};

} // namespace technicalmachine::ps
