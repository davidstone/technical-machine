// AI to win a 1v1 battle
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/client.hpp>
#include <tm/evaluate/depth.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>
#include <tm/generation.hpp>
#include <tm/string_conversions/generation.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <thread>

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

}	// namespace

int main(int argc, char * * argv) {
	print_debug_statements();

	auto const general_depth = (argc <= 1) ? 2U : boost::lexical_cast<unsigned>(argv[1]);
	auto const single_depth = (argc <= 2) ? 0U : boost::lexical_cast<unsigned>(argv[2]);
	auto const depth = technicalmachine::DepthValues{general_depth, single_depth};

	auto const settings = technicalmachine::load_settings_file("settings/settings.json");

	while (true) {
		try {
			// Too large to fit on stack
			auto client = std::make_unique<technicalmachine::ps::Client>(settings, depth);
			std::cout << "Connected\n" << std::flush;
			client->run();
		} catch (std::exception const & ex) {
			constexpr auto timeout = std::chrono::seconds(10);
			std::cerr << ex.what() << " Disconnected. Waiting " << timeout.count() << " seconds and trying again.\n";
			std::this_thread::sleep_for(timeout);
			std::cerr << "Reconnecting.\n";
		}
	}
}

