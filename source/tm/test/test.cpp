// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/clients/pokemon_showdown/battles.hpp>
#include <tm/test/clients/pokemon_showdown/packed_team.hpp>
#include <tm/test/clients/pokemon_showdown/parse_team.hpp>
#include <tm/test/clients/pokemon_showdown/slot_memory.hpp>
#include <tm/test/collections/collection.hpp>
#include <tm/test/block.hpp>
#include <tm/test/calculate_damage.hpp>
#include <tm/test/calculate_ivs_and_evs.hpp>
#include <tm/test/call_move.hpp>
#include <tm/test/critical_hit.hpp>
#include <tm/test/ev_optimizer.hpp>
#include <tm/test/files_in_path.hpp>
#include <tm/test/sleep.hpp>
#include <tm/test/stat.hpp>
#include <tm/test/status.hpp>
#include <tm/test/string_conversion.hpp>
#include <tm/test/evaluate/evaluate.hpp>
#include <tm/test/evaluate/expectiminimax.hpp>
#include <tm/test/pokemon_lab/test.hpp>
#include <tm/test/pokemon_online/test.hpp>

#include <iostream>

int main() {
	using namespace technicalmachine;

	test_files_in_path();

	block_tests();
	string_conversion_tests();
	status_tests();
	stat_tests();
	critical_hit_tests();
	damage_tests();
	collection_tests();
	call_move_tests();
	sleep_tests();

	calculate_ivs_and_evs_tests();
	ev_optimizer_tests();
	
	evaluate_tests();
	expectiminimax_tests();

	pl::test ();
	po::test ();
	ps::test_parse_team();
	ps::test_packed_team();
	ps::test_slot_memory();

	ps::test_battles();
	
	std::cout << "All tests passed.\n";
	return 0;
}