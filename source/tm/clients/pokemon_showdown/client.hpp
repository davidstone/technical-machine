// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battles.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/sockets.hpp>

#include <tm/evaluate/evaluate.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/buffer_view.hpp>
#include <tm/settings_file.hpp>

#include <containers/trivial_inplace_function.hpp>

#include <random>
#include <string_view>

namespace technicalmachine {
namespace ps {

struct ClientImpl {
	using AuthenticationSignature = http::response<http::string_body>(
		std::string_view,
		std::string_view,
		http::request<http::string_body> const &
	) const;
	using AuthenticationFunction = containers::trivial_inplace_function<
		AuthenticationSignature,
		sizeof(void *)
	>;
	ClientImpl(SettingsFile, DepthValues, SendMessageFunction, AuthenticationFunction);
	void run(DelimitedBufferView<std::string_view> messages);

private:
	void send_team(Generation);

	void handle_message(InMessage message);
	void send_channel_message(std::string_view channel, std::string_view message);

	void join_channel(std::string_view channel);

	void authenticate(std::string_view challstr);

	DelimitedBufferView<std::string_view> read_message();

	std::random_device m_rd;
	std::mt19937 m_random_engine;

	AllUsageStats m_all_usage_stats;
	AllEvaluate m_evaluate;

	SettingsFile m_settings;

	DepthValues m_depth;

	Battles m_battles;

	SendMessageFunction m_send_message;
	AuthenticationFunction m_authenticate;
};

struct Client {
	Client(SettingsFile settings, DepthValues);
	[[noreturn]] void run();

private:
	Sockets m_sockets;
	ClientImpl m_impl;
};

} // namespace ps
} // namespace technicalmachine
