// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

#include <tm/clients/pokemon_showdown/client.hpp>

#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/team_predictor/lead_stats.hpp>

#include <tm/constant_generation.hpp>
#include <tm/settings_file.hpp>
#include <tm/team.hpp>

#include <bounded/scope_guard.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate.hpp>
#include <containers/single_element_range.hpp>

#include <boost/beast/http.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace technicalmachine {
namespace ps {
namespace {

using namespace std::string_view_literals;

// Ideally this would return a lazy range
auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto result = containers::vector<containers::string>();
	auto file = std::ifstream(file_name);
	auto line = std::string();
	while (std::getline(file, line)) {
		containers::push_back(result, containers::string(line));
	}
	return result;
}

constexpr auto parse_generation(std::string_view const id) -> Generation {
	// TODO: This won't work for generation 10
	// Expected format: genXou
	constexpr auto generation_index = std::string_view("gen").size();
	if (id.size() < generation_index) {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid format string. Expected something in the format of: \"gen[generation_number]ou\", but got "sv, id));
	}
	auto const generation_char = id[generation_index];
	auto const generation = generation_char - '0';
	if (generation < 1 or 8 < generation) {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid generation. Expected a value between 1 and 8, but got "sv, containers::single_element_range(generation_char)));
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
		auto print_on_exception = bounded::scope_guard([=]{ std::cerr << next << '\n'; });
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
		std::cout << "PM from " << from << " to " << to << ": " << message.remainder() << '\n';
	} else if (type == "queryresponse") {
		// message.remainder() == QUERYTYPE|JSON
	} else if (type == "uhtml" or type == "uhtmlchange") {
		// message.remainder() == NAME|HTML
	} else if (type == "updatechallenges") {
		auto const json = m_parse_json(message.remainder());
		for (auto const & challenge : json.get_child("challengesFrom")) {
			auto const is_trusted = containers::any_equal(m_trusted_users, challenge.first);
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
		containers::concatenate<containers::string>("act=login&name="sv, m_settings.username, "&pass="sv, m_settings.password, "&challstr="sv, challstr)
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
	m_send_message(containers::concatenate<containers::string>("|/trn "sv, m_settings.username, ",0,"sv, json.get<std::string>("assertion")));
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
