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

import tm.clients.ps.battles;
import tm.clients.ps.handle_chat_message;
import tm.clients.ps.inmessage;
import tm.clients.ps.parse_generation_from_format;
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
import tm.buffer_view;
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

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

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

	auto handle_messages(DelimitedBufferView<std::string_view> messages) -> void {
		auto const has_room = !messages.remainder().empty() and messages.remainder().front() == '>';
		auto const room = has_room ? messages.pop().substr(1) : std::string_view{};
		while (!messages.remainder().empty()) {
			auto const next = messages.pop();
			auto print_on_exception = bounded::scope_guard([=] { std::cerr << next << '\n'; });
			handle_message(InMessage(room, next));
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

	auto handle_message(InMessage message) -> void {
		auto send_challenge = [&]{
			tv::visit(m_settings.style, tv::overload(
				[&](SettingsFile::Ladder const & ladder) {
					send_request_battle_start(ladder.format, "|/search "sv, ladder.format);
				},
				[&](SettingsFile::Challenge const & challenge) {
					send_request_battle_start(challenge.format, "|/challenge "sv, challenge.user, ","sv, challenge.format);
				},
				[](SettingsFile::Accept const &) {}
			));
		};
		if (m_battles.handle_message(m_all_usage_stats, message, m_send_message, send_challenge)) {
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
					containers::string(message.room()),
					m_settings.username,
					m_evaluate,
					m_depth,
					std::mt19937(std::random_device()())
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
			std::cerr << "Received unknown message in room: " << message.room() << " type: " << type << "\n\t" << message.remainder() << '\n';
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
		// http://play.pokemonshowdown.com/action.php?act=upkeep&challstr=CHALLSTR
		// Otherwise, make HTTP POST: http://play.pokemonshowdown.com/action.php
		// with data act=login&name=USERNAME&pass=PASSWORD&challstr=CHALLSTR
		constexpr auto host = "play.pokemonshowdown.com";

		auto const request = create_http_post(
			host,
			"/action.php",
			containers::concatenate<containers::string>("act=login&name="sv, m_settings.username, "&pass="sv, m_settings.password, "&challstr="sv, challstr)
		);
		auto const response = m_authenticate(host, "80", request);

		// Response begins with ']' followed by JSON object.
		auto const body = std::string_view(response.body()).substr(1);
		auto const json = nlohmann::json::parse(body);
		m_send_message(containers::concatenate<containers::string>("|/trn "sv, m_settings.username, ",0,"sv, json.at("assertion").get<std::string_view>()));
	}

	DelimitedBufferView<std::string_view> read_message();

	// https://github.com/llvm/llvm-project/issues/61065
	//std::random_device m_rd;
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
