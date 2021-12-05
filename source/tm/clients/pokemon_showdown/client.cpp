// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

#include <tm/clients/pokemon_showdown/client.hpp>

#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/parse_generation_from_format.hpp>
#include <tm/clients/pokemon_showdown/to_packed_format.hpp>

#include <tm/team_predictor/lead_stats.hpp>
#include <tm/team_predictor/team_predictor.hpp>

#include <tm/constant_generation.hpp>
#include <tm/get_directory.hpp>
#include <tm/load_team_from_file.hpp>
#include <tm/team.hpp>

#include <bounded/scope_guard.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/single_element_range.hpp>
#include <containers/string.hpp>

#include <boost/beast/http.hpp>

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace technicalmachine {
namespace ps {

ClientImpl::ClientImpl(SettingsFile settings, DepthValues const depth, SendMessageFunction send_message, AuthenticationFunction authenticate):
	m_random_engine(m_rd()),
	m_settings(std::move(settings)),
	m_depth(depth),
	m_battles("battles", true),
	m_send_message(std::move(send_message)),
	m_authenticate(std::move(authenticate))
{
}

void ClientImpl::run(DelimitedBufferView<std::string_view> messages) {
	auto const has_room = !messages.remainder().empty() and messages.remainder().front() == '>';
	auto const room = has_room ? messages.pop().substr(1) : std::string_view{};
	while (!messages.remainder().empty()) {
		auto const next = messages.pop();
		auto print_on_exception = bounded::scope_guard([=]{ std::cerr << next << '\n'; });
		handle_message(InMessage(room, next));
		print_on_exception.dismiss();
	}
}

void ClientImpl::send_team(Generation const runtime_generation) {
	auto send_team = [&](std::string_view const team_str) {
		m_send_message(containers::concatenate<containers::string>("|/utm "sv, team_str));
	};
	auto send_real_team = [&](auto make_team_str) {
		send_team(constant_generation(runtime_generation, make_team_str));
	};
	return bounded::visit(m_settings.team, bounded::overload(
		[&](SettingsFile::NoTeam) { send_team("null"); },
		[&](SettingsFile::GenerateTeam) {
			send_real_team([&]<Generation generation>(constant_gen_t<generation>) {
				return to_packed_format(generate_team<generation>(
					m_all_usage_stats[generation],
					use_lead_stats,
					m_random_engine
				));
			});
		},
		[&](std::filesystem::path const & path) {
			send_real_team([&]<Generation generation>(constant_gen_t<generation>) {
				auto const team = load_random_team_from_directory(m_random_engine, path);
				constexpr auto expected_type = bounded::detail::types<KnownTeam<generation>>();
				if (!holds_alternative(team, expected_type)) {
					throw std::runtime_error("Generation mismatch in team file vs. battle.");
				}
				return to_packed_format(team[expected_type]);
			});
		}
	));
}

namespace {

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

} // namespace

void ClientImpl::handle_message(InMessage message) {
	auto send_challenge = [&]{
		bounded::visit(m_settings.style, bounded::overload(
			[&](SettingsFile::Ladder const & ladder) {
				send_team(parse_generation_from_format(ladder.format));
				m_send_message(containers::concatenate<containers::string>("|/search "sv, ladder.format));
			},
			[&](SettingsFile::Challenge const & challenge) {
				send_team(parse_generation_from_format(challenge.format));
				m_send_message(containers::concatenate<containers::string>("|/challenge "sv, challenge.user, ","sv, challenge.format));
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
				std::mt19937(m_rd())
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
			bounded::visit(m_settings.style, bounded::overload(
				[](SettingsFile::Ladder const &) {},
				[](SettingsFile::Challenge const &) {},
				[&](SettingsFile::Accept const & accept) {
					auto const should_accept = containers::any_equal(accept.users, no_spaces_string_view(from));
					if (should_accept) {
						send_team(parse_generation_from_format(format));
						m_send_message(containers::concatenate<containers::string>("|/accept "sv, from));
					} else {
						m_send_message(containers::concatenate<containers::string>("|/reject "sv, from));
					}
				}
			));
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

void ClientImpl::authenticate(std::string_view const challstr) {
	// In theory, if we ever support session cookies, make HTTP GET:
	// http://play.pokemonshowdown.com/action.php?act=upkeep&challstr=CHALLSTR
	// Otherwise, make HTTP POST: http://play.pokemonshowdown.com/action.php
	// with data act=login&name=USERNAME&pass=PASSWORD&challstr=CHALLSTR
	namespace http = boost::beast::http;
	constexpr auto host = "play.pokemonshowdown.com";
	
	constexpr auto version = 11U;
	auto request = http::request<http::string_body>{
		http::verb::post,
		"/action.php",
		version,
		containers::concatenate<containers::string>("act=login&name="sv, m_settings.username, "&pass="sv, m_settings.password, "&challstr="sv, challstr)
	};
	request.set(http::field::host, host);
	request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request.set(http::field::content_type, "application/x-www-form-urlencoded");
	request.prepare_payload();

	auto const response = m_authenticate(host, "80", request);
	
	// Response begins with ']' followed by JSON object.
	auto const body = std::string_view(response.body()).substr(1);
	auto const json = nlohmann::json::parse(body);
	m_send_message(containers::concatenate<containers::string>("|/trn "sv, m_settings.username, ",0,"sv, json.at("assertion").get<std::string_view>()));
}

void ClientImpl::join_channel(std::string_view const channel) {
	m_send_message(containers::concatenate<containers::string>("|/join "sv, channel));
}

void ClientImpl::send_channel_message(std::string_view const channel, std::string_view const message) {
	m_send_message(containers::concatenate<containers::string>(channel, "|/msg "sv, message));
}

Client::Client(SettingsFile settings, DepthValues const depth):
	m_sockets(settings.host, settings.port, settings.resource),
	m_impl(
		std::move(settings),
		depth,
		[&](std::string_view const output) { m_sockets.write_message(output); },
		[&](auto const & ... args) { return m_sockets.authenticate(args...); }
	)
{
}

void Client::run() {
	while (true) {
		m_impl.run(DelimitedBufferView<std::string_view>(m_sockets.read_message(), '\n'));
	}
}

} // namespace ps
} // namespace technicalmachine
