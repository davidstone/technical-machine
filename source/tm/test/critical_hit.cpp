// Copyright (C) 2020 David Stone
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

#include <tm/test/critical_hit.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/critical_hit.hpp>
#include <tm/team.hpp>

#include <iostream>

namespace technicalmachine {
namespace {

template<Generation generation>
auto individual_test(Species const species, Moves const move_name, Item const item, bool const focus_energy, double const rate) {
	auto weather = Weather();
	auto attacker = Team<generation>(1_bi);
	constexpr auto ability = Ability::Honey_Gather;
	attacker.add_pokemon(Pokemon<generation>(
		species,
		Level(100_bi),
		Gender::genderless,
		item,
		ability,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	));
	attacker.pokemon().switch_in(weather);
	if (focus_energy) {
		attacker.pokemon().focus_energy();
	}
	attacker.reset_start_of_turn();

	auto const ch_rate = critical_hit_probability(std::as_const(attacker).pokemon(), move_name, ability, weather);
	BOUNDED_ASSERT(ch_rate == rate);
}

} // namespace

void critical_hit_tests() {
	std::cout << "Running critical hit tests.\n";
	individual_test<Generation::one>(Species::Slowpoke, Moves::Water_Gun, Item::None, true, 1.0 / 256.0);
	individual_test<Generation::one>(Species::Tauros, Moves::Body_Slam, Item::None, false, 55.0 / 256.0);
	individual_test<Generation::one>(Species::Electrode, Moves::Thunderbolt, Item::None, false, 70.0 / 256.0);
	individual_test<Generation::one>(Species::Electrode, Moves::Slash, Item::None, false, 255.0 / 256.0);
	individual_test<Generation::one>(Species::Sandslash, Moves::Slash, Item::None, false, 255.0 / 256.0);
	individual_test<Generation::one>(Species::Goldeen, Moves::Slash, Item::None, false, 248.0 / 256.0);
	individual_test<Generation::two>(Species::Pikachu, Moves::Thunder, Item::None, false, 1.0 / 16.0);
	individual_test<Generation::two>(Species::Smeargle, Moves::Slash, Item::None, false, 1.0 / 4.0);
	individual_test<Generation::two>(Species::Sandshrew, Moves::Slash, Item::None, true, 1.0 / 3.0);
	individual_test<Generation::two>(Species::Farfetchd, Moves::Slash, Item::Stick, true, 1.0 / 2.0);
	individual_test<Generation::three>(Species::Smeargle, Moves::Slash, Item::None, false, 1.0 / 8.0);
	individual_test<Generation::three>(Species::Sandshrew, Moves::Slash, Item::None, true, 1.0 / 3.0);
	individual_test<Generation::three>(Species::Farfetchd, Moves::Slash, Item::Stick, true, 1.0 / 2.0);
	std::cout << "Critical hit tests passed.\n\n";
}

} // namespace technicalmachine
