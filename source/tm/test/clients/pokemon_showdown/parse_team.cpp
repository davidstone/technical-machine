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

#include <tm/test/clients/pokemon_showdown/parse_team.hpp>

#include <tm/clients/pokemon_showdown/parse_team.hpp>

#include <tm/pokemon/species.hpp>
#include <tm/string_conversions/team.hpp>

#include <iostream>
#include <string_view>

namespace technicalmachine {
namespace ps {
namespace {

constexpr auto move_array(Generation const generation, auto... moves) {
	return containers::array{Move(generation, moves)...};
}

auto max_all_evs(Generation const generation, Pokemon & pokemon) {
	constexpr auto ev = EV(252_bi);
	for (auto const stat_name : containers::enum_range<PermanentStat>()) {
		pokemon.set_ev(generation, stat_name, IV(30_bi), ev);
	}
}

auto expected_generation_one_team() {
	auto team = Team(6_bi, true);
	constexpr auto generation = Generation::one;
	auto add_pokemon = [&](Species const species, auto const level, auto const ... moves) -> Pokemon & {
		Pokemon & pokemon = team.add_pokemon(Pokemon(
			generation,
			species,
			Level(level),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			Nature::Hardy
		));
		containers::append(pokemon.regular_moves(), move_array(generation, moves...));
		max_all_evs(generation, pokemon);
		return pokemon;
	};

	auto reduce_stat = [=](Pokemon & pokemon, PermanentStat const stat_name) {
		constexpr auto reduced_ev = EV(248_bi);
		constexpr auto iv = IV(30_bi);
		pokemon.set_ev(generation, stat_name, iv, reduced_ev);
	};

	add_pokemon(Species::Koffing, 88_bi, Moves::Fire_Blast, Moves::Sludge, Moves::Explosion, Moves::Thunderbolt);

	auto & sandslash = add_pokemon(Species::Sandslash, 74_bi, Moves::Earthquake, Moves::Body_Slam, Moves::Rock_Slide, Moves::Swords_Dance);
	reduce_stat(sandslash, PermanentStat::hp);
	reduce_stat(sandslash, PermanentStat::atk);

	add_pokemon(Species::Ditto, 88_bi, Moves::Transform);

	add_pokemon(Species::Pikachu, 88_bi, Moves::Agility, Moves::Thunderbolt, Moves::Surf, Moves::Thunder_Wave);

	auto & jynx = add_pokemon(Species::Jynx, 68_bi, Moves::Seismic_Toss, Moves::Lovely_Kiss, Moves::Psychic, Moves::Blizzard);
	reduce_stat(jynx, PermanentStat::def);

	auto & slowbro = add_pokemon(Species::Slowbro, 68_bi, Moves::Psychic, Moves::Surf, Moves::Thunder_Wave, Moves::Amnesia);
	reduce_stat(slowbro, PermanentStat::def);

	return team;
}

void test_generation_one() {
	constexpr auto initial = std::string_view(
		R"#({"active":[{"moves":[{"move":"Fire Blast","id":"fireblast","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Sludge","id":"sludge","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Explosion","id":"explosion","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Thunderbolt","id":"thunderbolt","pp":24,"maxpp":24,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Koffing","details":"Koffing, L88","condition":"250/250","active":true,"stats":{"atk":201,"def":254,"spa":192,"spd":192,"spe":148},"moves":["fireblast","sludge","explosion","thunderbolt"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Sandslash","details":"Sandslash, L74","condition":"263/263","active":false,"stats":{"atk":221,"def":236,"spa":155,"spd":155,"spe":170},"moves":["earthquake","bodyslam","rockslide","swordsdance"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Ditto","details":"Ditto, L88","condition":"264/264","active":false,"stats":{"atk":171,"def":171,"spa":171,"spd":171,"spe":171},"moves":["transform"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Pikachu","details":"Pikachu, L88","condition":"241/241","active":false,"stats":{"atk":183,"def":139,"spa":174,"spd":174,"spe":245},"moves":["agility","thunderbolt","surf","thunderwave"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Jynx","details":"Jynx, L68","condition":"229/229","active":false,"stats":{"atk":136,"def":115,"spa":197,"spd":197,"spe":197},"moves":["seismictoss","lovelykiss","psychic","blizzard"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Slowbro","details":"Slowbro, L68","condition":"270/270","active":false,"stats":{"atk":170,"def":217,"spa":177,"spd":177,"spe":109},"moves":["psychic","surf","thunderwave","amnesia"],"baseAbility":"none","item":"","pokeball":"pokeball"}]},"rqid":2})#"
	);
	constexpr auto generation = Generation::one;
	auto const parsed = parse_team(initial, generation);
	auto const expected = expected_generation_one_team();
	auto const parsed_str = to_string(generation, parsed);
	auto const expected_str = to_string(generation, expected);
	if (parsed_str != expected_str) {
		auto const str = "Expected:\n\n" + expected_str + "\n\nbut got\n\n" + parsed_str;
		throw std::runtime_error(std::string(std::string_view(str)));
	}
}

} // namespace

void test_parse_team() {
	std::cout << "Testing ps::parse_team\n";
	test_generation_one();
}

} // namespace ps
} // namespace technicalmachine