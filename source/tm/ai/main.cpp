// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.ai.print_sizes;

import tm.clients.ps.client;

import tm.strategy.parse_strategy;

import tm.get_directory;
import tm.load_settings_file;

import bounded;
import std_module;

using namespace technicalmachine;

int main(int argc, char * * argv) {
	print_sizes();
	while (true) {
		try {
			auto client = ps::Client(
				load_settings_file(get_settings_directory() / "settings.json"),
				parse_strategy(argc - 1, argv + 1)
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
