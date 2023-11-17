// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.clients.ps.client_impl;

import tm.clients.pl.write_team_file;

import tm.clients.ps.battle_message_result;
import tm.clients.ps.battles;
import tm.clients.ps.handle_chat_message;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.room_message;
import tm.clients.ps.room_messages;
import tm.clients.ps.send_message_function;
import tm.clients.ps.to_packed_format;

import tm.clients.get_team;
import tm.clients.should_accept_challenge;
import tm.clients.write_team;

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;

import tm.team_predictor.team_predictor;
import tm.team_predictor.all_usage_stats;

import tm.boost_beast_http;
import tm.constant_generation;
import tm.generation;
import tm.get_directory;
import tm.nlohmann_json;
import tm.settings_file;
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

// https://github.com/smogon/pokemon-showdown/blob/master/PROTOCOL.md

namespace technicalmachine::ps {
using namespace std::string_view_literals;

struct no_spaces_string_view {
	constexpr explicit no_spaces_string_view(std::string_view const str):
		m_str(str)
	{
	}

	friend constexpr auto operator==(no_spaces_string_view const lhs, std::string_view const rhs) -> bool {
		return containers::equal(containers::filter(lhs.m_str, [](char const c) { return c != ' '; }), rhs);
	}

private:
	std::string_view m_str;
};

constexpr auto write_team_function = [](GenerationGeneric<Team> const & team, std::filesystem::path const & path) {
	tv::visit(team, [&]<Generation generation>(Team<generation> const & t) {
		pl::write_team(t, path);
	});
};

using AuthenticationFunction = containers::trivial_inplace_function<
	AuthenticationSignature,
	sizeof(void *)
>;

export struct ClientImpl {
	ClientImpl(SettingsFile settings, Depth const depth, SendMessageFunction send_message, AuthenticationFunction authenticate):
		m_random_engine(std::random_device()()),
		m_all_usage_stats(stats_for_generation),
		m_settings(std::move(settings)),
		m_depth(depth),
		m_battles("battles", WriteTeam{".sbt", write_team_function}),
		m_send_message(std::move(send_message)),
		m_authenticate(std::move(authenticate))
	{
	}

	auto handle_messages(RoomMessages const room_messages) -> void {
		for (auto const room_message : room_messages.messages()) {
			auto print_on_exception = bounded::scope_guard([=] {
				std::cerr << room_message.message.remainder() << '\n';
			});
			handle_message(room_message);
			print_on_exception.dismiss();
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

	auto handle_battle_message(RoomMessage const room_message) -> bool {
		auto const result = m_battles.handle_message(room_message);
		if (!result) {
			return false;
		}
		tv::visit(*result, tv::overload(
			[](BattleContinues) {
			},
			[&](BattleResponseMove const move_index) {
				m_send_message(containers::concatenate<containers::string>(
					room_message.room,
					"|/choose move "sv,
					containers::to_string(move_index)
				));
			},
			[&](BattleResponseSwitch const switch_index) {
				m_send_message(containers::concatenate<containers::string>(
					room_message.room,
					"|/choose switch "sv,
					containers::to_string(switch_index)
				));
			},
			[&](BattleResponseError) {
				m_send_message(containers::concatenate<containers::string>(
					room_message.room,
					"|/choose default"sv
				));
			},
			[&](BattleStarted) {
				m_send_message(containers::concatenate<containers::string>(room_message.room, "|/timer on"sv));
			},
			[&](BattleFinished) {
				m_send_message(containers::concatenate<containers::string>(std::string_view("|/leave "), room_message.room));
				send_challenge();
			}
		));
		return true;
	}

	auto handle_message(RoomMessage const room_message) -> void {
		auto message = room_message.message;
		if (handle_battle_message(room_message)) {
			return;
		}
		if (handle_chat_message(message)) {
			return;
		}
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
			if (message.pop() == "battle") {
				m_battles.add_pending(
					containers::string(room_message.room),
					m_settings.username,
					m_evaluate,
					m_all_usage_stats,
					m_depth
				);
			}
		} else if (type == "nametaken") {
			auto const username = message.pop();
			std::cerr << "Could not change username to " << username << " because: " << message.remainder() << '\n';
		} else if (type == "popup") {
			std::cout << "popup message: " << message.remainder() << '\n';
		} else if (type == "pm") {
			auto const from = message.pop();
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
			std::cerr << "Received unknown message in room: " << room_message.room << " type: " << type << "\n\t" << message.remainder() << '\n';
		}
	}

	auto send_channel_message(std::string_view const channel, std::string_view const message) -> void {
		m_send_message(containers::concatenate<containers::string>(channel, "|/msg "sv, message));
	}

	auto join_channel(std::string_view const channel) -> void {
		m_send_message(containers::concatenate<containers::string>("|/join "sv, channel));
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
	AllEvaluate m_evaluate;

	SettingsFile m_settings;

	Depth m_depth;

	Battles m_battles;

	SendMessageFunction m_send_message;
	AuthenticationFunction m_authenticate;
};

} // namespace technicalmachine::ps
