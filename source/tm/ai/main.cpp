// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <chrono>
#include <iostream>

import tm.ai.parse_args;
import tm.ai.print_sizes;

import tm.clients.ps.client;

import tm.evaluate.depth;

import tm.move.move;

import tm.pokemon.pokemon;

import tm.string_conversions.generation;

import tm.environment;
import tm.generation;
import tm.get_directory;
import tm.load_settings_file;
import tm.team;

import bounded;
import std_module;

using namespace technicalmachine;
using namespace bounded::literal;
using namespace std::string_view_literals;

int main(int argc, char * * argv) {
	print_sizes();

	auto const depth = parse_args(argc, argv);
	auto const settings = load_settings_file(get_settings_directory() / "settings.json");

	while (true) {
		try {
			auto client = ps::Client(settings, depth);
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
