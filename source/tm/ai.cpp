// AI to win a 1v1 battle
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <chrono>
#include <iostream>
#include <string_view>

import tm.clients.ps.client;

import tm.evaluate.depth;

import tm.move.move;

import tm.pokemon.pokemon;

import tm.string_conversions.generation;

import tm.generation;
import tm.get_directory;
import tm.load_settings_file;
import tm.team;
import tm.weather;

import bounded;
import std_module;

namespace {

template<technicalmachine::Generation generation>
void print_team_size() {
	std::cout << to_string(generation) << ": " << sizeof(technicalmachine::Team<generation>);
}

template<technicalmachine::Generation generation>
void print_pokemon_size() {
	std::cout << to_string(generation) << ": " << sizeof(technicalmachine::Pokemon<generation>);
}

void print_debug_statements() {
	std::cout << "sizeof(Team): ";
	print_team_size<technicalmachine::Generation::one>();
	std::cout << ", ";
	print_team_size<technicalmachine::Generation::four>();
	std::cout << '\n';
	std::cout << "sizeof(Pokemon): ";
	print_pokemon_size<technicalmachine::Generation::one>();
	std::cout << ", ";
	print_pokemon_size<technicalmachine::Generation::four>();
	std::cout << '\n';
	std::cout << "sizeof(Move): " << sizeof(technicalmachine::Move) << '\n';
	std::cout << "sizeof(Weather): " << sizeof(technicalmachine::Weather) << '\n';
}

} // namespace

int main(int argc, char * * argv) {
	using namespace technicalmachine;
	using namespace bounded::literal;
	print_debug_statements();

	auto const general_depth = (argc <= 1) ? 2_bi : bounded::to_integer<DepthInt>(argv[1]);
	auto const single_depth = (argc <= 2) ? 0_bi : bounded::to_integer<DepthInt>(argv[2]);
	auto const depth = Depth(general_depth, single_depth);

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
