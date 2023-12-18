// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ai.print_sizes;

import tm.move.move;

import tm.pokemon.pokemon;

import tm.string_conversions.generation;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import std_module;

template<technicalmachine::Generation generation>
auto print_team_size() -> void {
	std::cout << to_string(generation) << ": " << sizeof(technicalmachine::Team<generation>);
}

template<technicalmachine::Generation generation>
auto print_pokemon_size() -> void {
	std::cout << to_string(generation) << ": " << sizeof(technicalmachine::Pokemon<generation>);
}

export auto print_sizes() -> void {
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
	std::cout << "sizeof(Environment): " << sizeof(technicalmachine::Environment) << '\n';
}
