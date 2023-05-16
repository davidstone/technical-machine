// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <iostream>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.connection;
import tm.team_predictor.parser;

import std_module;

int main() {
	using namespace technicalmachine;
	using tcp = boost::asio::ip::tcp;
	auto const all_usage_stats = AllUsageStats(StatsForGeneration(stats_for_generation));

	constexpr auto port = static_cast<unsigned short>(46923);

	auto ioc = boost::asio::io_context();

	auto acceptor = tcp::acceptor(ioc, tcp::endpoint(tcp::v4(), port));
	auto random_engine = std::mt19937(std::random_device()());
	auto parser = Parser(all_usage_stats, random_engine);
	while (true) {
		try {
			auto socket = tcp::socket(ioc);
			acceptor.accept(socket);
			auto connection = Connection(std::move(socket));
			auto data = connection.read();
			connection.write(data, parser.team_string(data));
		} catch (std::exception const & ex) {
			std::cerr << ex.what() << '\n';
		}
	}
}
