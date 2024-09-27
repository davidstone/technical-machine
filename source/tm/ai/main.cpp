// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.ai.parse_args;
import tm.ai.print_sizes;

import tm.clients.ps.client;

import tm.evaluate.all_evaluate;

import tm.strategy.expectimax;
import tm.strategy.statistical;

import tm.environment;
import tm.get_directory;
import tm.load_settings_file;

import bounded;
import std_module;

using namespace technicalmachine;
using namespace bounded::literal;
using namespace std::string_view_literals;

int main(int argc, char * * argv) {
	print_sizes();

	auto const depth = parse_args(argc, argv);
	while (true) {
		try {
			auto client = ps::Client(
				load_settings_file(get_settings_directory() / "settings.json"),
				make_expectimax(
					AllEvaluate(),
					depth,
					make_statistical()
				)
			);
			std::cout << "Connected\n" << std::flush;
			client.run();
		} catch (std::exception const & ex) {
			constexpr auto timeout = std::chrono::seconds(10);
			std::cerr << ex.what() << " Disconnected. Waiting " << timeout.count() << " seconds and trying again.\n";
			std::this_thread::sleep_for(timeout);
			std::cerr << "Reconnecting.\n";
		}
	}
}
