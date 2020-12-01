// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

#include <tm/clients/pokemon_showdown/client.hpp>

#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/constant_generation.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/team_predictor/lead_stats.hpp>

#include <tm/settings_file.hpp>
#include <tm/team.hpp>

#include <containers/scope_guard.hpp>

#include <boost/beast/http.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace technicalmachine {
namespace ps {
namespace {

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto container = std::unordered_set<std::string>{};
	auto file = std::ifstream(file_name);
	auto line = std::string{};
	while (getline(file, line)) {
		if (!line.empty())
			container.insert(std::move(line));
	}
	return container;
}

constexpr auto parse_generation(std::string_view const id) -> Generation {
	// TODO: This won't work for generation 10
	// Expected format: genXou
	constexpr auto generation_index = std::string_view("gen").size();
	if (id.size() < generation_index) {
		throw std::runtime_error("Invalid format string. Expected something in the format of: \"gen[generation_number]ou\", but got " + std::string(id));
	}
	auto const generation_char = id[generation_index];
	auto const generation = generation_char - '0';
	if (generation < 1 or 8 < generation) {
		throw std::runtime_error("Invalid generation. Expected a value between 1 and 8, but got " + std::string(1, generation_char));
	}
	return static_cast<Generation>(generation);
}

}	// namespace

ClientImpl::ClientImpl(SettingsFile settings, DepthValues const depth, SendMessageFunction send_message, AuthenticationFunction authenticate):
	m_random_engine(m_rd()),
	m_settings(std::move(settings)),
	m_trusted_users(load_lines_from_file("settings/trusted_users.txt")),
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
		auto print_on_exception = containers::scope_guard([=]{ std::cerr << next << '\n'; });
		handle_message(InMessage(room, next));
		print_on_exception.dismiss();
	}
}

void ClientImpl::send_team(Generation const runtime_generation) {
	auto make_packed = [&]<Generation generation>(std::integral_constant<Generation, generation>) {
		return to_packed_format(generate_team<generation>());
	};
	m_send_message("|/utm " + constant_generation(runtime_generation, make_packed));
}


void ClientImpl::handle_message(InMessage message) {
	auto send_challenge = [&]{
		auto const format = containers::string("gen1ou");
		send_team(parse_generation(format));
		// m_send_message("|/search gen1ou");
		m_send_message("|/challenge davidstone," + format);
		std::cout << "Sent challenge\n" << std::flush;
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
				std::string(message.room()),
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
		std::cout << "PM from " << from << " to " << to << ": " << message.remainder() << '\n';
	} else if (type == "queryresponse") {
		// message.remainder() == QUERYTYPE|JSON
	} else if (type == "uhtml" or type == "uhtmlchange") {
		// message.remainder() == NAME|HTML
	} else if (type == "updatechallenges") {
		auto const json = m_parse_json(message.remainder());
		for (auto const & challenge : json.get_child("challengesFrom")) {
			auto const is_trusted = m_trusted_users.find(challenge.first) != m_trusted_users.end();
			if (is_trusted) {
				send_team(parse_generation(challenge.second.get<std::string>("")));
				m_send_message("|/accept " + challenge.first);
			} else {
				m_send_message("|/reject " + challenge.first);
			}
		}
		// "cancelchallenge" is the command to stop challenging someone
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
		("act=login&name=" + m_settings.username + "&pass=" + m_settings.password + "&challstr=").append(challstr)
	};
	request.set(http::field::host, host);
	request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request.set(http::field::content_type, "application/x-www-form-urlencoded");
	request.prepare_payload();

	auto response = m_authenticate(host, "80", request);
	
	// Sadly, it does not look like it is possible to use
	// boost::property_tree to directly parse JSON from some arbitrary
	// range. I could create my own custom stream class that lets me do this
	// without all of the copying and memory allocation, but it doesn't seem
	// worth it considering how rare this will be.
	//
	// Response begins with ']' followed by JSON object.
	response.body().erase(0U, 1U);
	auto const json = m_parse_json(response.body());
	m_send_message("|/trn " + m_settings.username + ",0," + json.get<std::string>("assertion"));
}

void ClientImpl::join_channel(std::string const & channel) {
	m_send_message("|/join " + channel);
}

void ClientImpl::send_channel_message(std::string const & channel, std::string const & message) {
	m_send_message(channel + "|/msg " + message);
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
		m_impl.run(m_sockets.read_message());
	}
}

} // namespace ps
} // namespace technicalmachine
