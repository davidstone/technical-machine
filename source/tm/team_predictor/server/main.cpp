// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.web_server.http_server;
import tm.web_server.websocket_server;

import tm.team_predictor.server.make_dictionary;
import tm.team_predictor.server.parse_predictor_inputs;
import tm.team_predictor.server.predicted_team_string;

import tm.team_predictor.all_usage_stats;

import tm.boost_networking;
import tm.nlohmann_json;

import containers;
import std_module;
import tv;

using namespace technicalmachine;
using namespace std::string_view_literals;

auto main() -> int {
	constexpr auto http_port = static_cast<unsigned short>(46923);
	constexpr auto websocket_port = static_cast<unsigned short>(46924);
	auto const all_usage_stats = AllUsageStats();
	auto random_engine = std::mt19937(std::random_device()());
	auto context = boost::asio::io_context();

	auto http_server = HTTPServer(context, http_port);
	auto websocket_server = WebSocketServer(
		context,
		websocket_port,
		[&] -> containers::string {
			return containers::string(make_dictionary(all_usage_stats).dump());
		},
		[&](std::string_view const str) -> containers::string {
			return containers::string(nlohmann::json({{
				"predicted"sv,
				predicted_team_string(
					parse_predictor_inputs(str),
					all_usage_stats,
					random_engine
				)
			}}).dump());
		}
	);
	while (true) {
		try {
			context.run();
		} catch (std::exception const & ex) {
			std::cerr << ex.what() << '\n';
		}
	}
}
