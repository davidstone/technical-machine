// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/test/clients/pokemon_showdown/battles.hpp>
#include <tm/test/clients/pokemon_showdown/slot_memory.hpp>
#include <tm/test/collections/collection.hpp>
#include <tm/test/block.hpp>
#include <tm/test/calculate_damage.hpp>
#include <tm/test/call_move.hpp>
#include <tm/test/ev_optimizer.hpp>
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

	ps::test_battles();
	
	block_tests();
	string_conversion_tests();
	status_tests();
	stat_tests();
	damage_tests();
	collection_tests();
	call_move_tests();

	ev_optimizer_tests();
	
	evaluate_tests();
	expectiminimax_tests();

	pl::test ();
	po::test ();
	ps::test_slot_memory();

	std::cout << "All tests passed.\n";
	return 0;
}
