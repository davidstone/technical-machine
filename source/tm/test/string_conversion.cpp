// Copyright (C) 2015 David Stone
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

#include <tm/test/string_conversion.hpp>

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>
#include <tm/move/moves.hpp>
#include <tm/status.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/nature.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <containers/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {

// I make the ToString conversion a logic error because I only convert to a
// string from an internal data structure, so I know the input is only invalid
// if there is a logic error in my program.

struct InvalidToStringConversion : std::logic_error {
	InvalidToStringConversion(auto original, auto result, std::string_view const intermediate):
		std::logic_error(to_string(bounded::integer(original)) + " is seen as " + to_string(bounded::integer(result)) + " with an intermediate string of " + std::string(intermediate) + ".\n") {
	}
};

template<typename Enum>
void test_generic(std::string_view const thing) {
	std::cout << "\tVerifying correct " << thing << ".\n";
	for (auto const original : containers::enum_range<Enum>()) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		if (original != result) {
			throw InvalidToStringConversion(original, result, str);
		}
	}
}

void test_pokemon() {
	constexpr auto generation = Generation::three;
	constexpr auto team_size = 6_bi;
	constexpr auto species = Species::Mewtwo;
	constexpr auto level = Level(100_bi);
	constexpr auto gender = Gender::genderless;
	constexpr auto item = Item::Leftovers;
	constexpr auto ability = Ability::Pressure;
	constexpr auto nature = Nature::Modest;

	constexpr auto hp = EV(4_bi);
	constexpr auto atk = EV(12_bi);
	constexpr auto def = EV(24_bi);
	constexpr auto spa = EV(0_bi);
	constexpr auto spd = EV(32_bi);
	constexpr auto spe = EV(100_bi);
	constexpr auto iv = default_iv(generation);

	auto pokemon = Pokemon(generation, 6_bi, species, level, gender, item, ability, nature);
	set_hp_ev(generation, pokemon, iv, hp);
	set_stat_ev(pokemon, StatNames::ATK, iv, atk);
	set_stat_ev(pokemon, StatNames::DEF, iv, def);
	set_stat_ev(pokemon, StatNames::SPA, iv, spa);
	set_stat_ev(pokemon, StatNames::SPD, iv, spd);
	set_stat_ev(pokemon, StatNames::SPE, iv, spe);

	auto check = [&] {
		auto const str = to_string(pokemon);
		auto const result = pokemon_from_string(str, generation, team_size);

		if (pokemon != result) {
			std::cerr << str << '\n';
			throw std::exception();
		}
	};

	check();
	for (auto const move : {Moves::Psychic, Moves::Recover, Moves::Calm_Mind, Moves::Taunt}) {
		all_moves(pokemon).add(Move(generation, move));
		check();
	}
	pokemon.set_status(Statuses::burn);
	check();
}

} // namespace

void string_conversion_tests() {
	std::cout << "Running string conversion tests.\n";
	test_generic<Ability>("ability");
	test_generic<Gender>("gender");
	test_generic<Item>("item");
	test_generic<Moves>("move");
	test_generic<Nature>("nature");
	test_generic<Species>("species");
	test_generic<Statuses>("status");
	test_pokemon();
	std::cout << "String conversion tests passed.\n\n";
}

} // namespace technicalmachine
