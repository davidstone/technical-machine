// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/clients/pokemon_showdown/parse_team.hpp>

#include <tm/clients/pokemon_showdown/parse_team.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/string_conversions/team.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

#include <iostream>
#include <string_view>

namespace technicalmachine {
namespace ps {
namespace {

auto expected_generation_one_team() {
	constexpr auto generation = Generation::one;
	auto team = Team<generation>(6_bi, true);

	auto moves = [](auto... move_names) {
		return RegularMoves({Move(generation, move_names)...});
	};

	team.add_pokemon(Pokemon<generation>(
		Species::Koffing,
		Level(88_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Fire_Blast, Moves::Sludge, Moves::Explosion, Moves::Thunderbolt)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Sandslash,
		Level(74_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(248_bi),
				EV(248_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Earthquake, Moves::Body_Slam, Moves::Rock_Slide, Moves::Swords_Dance)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Ditto,
		Level(88_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Transform)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(88_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Agility, Moves::Thunderbolt, Moves::Surf, Moves::Thunder_Wave)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Jynx,
		Level(68_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(248_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Seismic_Toss, Moves::Lovely_Kiss, Moves::Psychic, Moves::Blizzard)
	));
	
	team.add_pokemon(Pokemon<generation>(
		Species::Slowbro,
		Level(68_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			max_dvs_or_ivs<generation>,
			EVs(
				EV(252_bi),
				EV(252_bi),
				EV(248_bi),
				EV(252_bi),
				EV(252_bi),
				EV(252_bi)
			)
		},
		moves(Moves::Psychic, Moves::Surf, Moves::Thunder_Wave, Moves::Amnesia)
	));

	return team;
}

void test_generation_one() {
	using namespace std::string_view_literals;
	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Fire Blast","id":"fireblast","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Sludge","id":"sludge","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Explosion","id":"explosion","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Thunderbolt","id":"thunderbolt","pp":24,"maxpp":24,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Koffing","details":"Koffing, L88","condition":"250/250","active":true,"stats":{"atk":201,"def":254,"spa":192,"spd":192,"spe":148},"moves":["fireblast","sludge","explosion","thunderbolt"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Sandslash","details":"Sandslash, L74","condition":"263/263","active":false,"stats":{"atk":221,"def":236,"spa":155,"spd":155,"spe":170},"moves":["earthquake","bodyslam","rockslide","swordsdance"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Ditto","details":"Ditto, L88","condition":"264/264","active":false,"stats":{"atk":171,"def":171,"spa":171,"spd":171,"spe":171},"moves":["transform"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Pikachu","details":"Pikachu, L88","condition":"241/241","active":false,"stats":{"atk":183,"def":139,"spa":174,"spd":174,"spe":245},"moves":["agility","thunderbolt","surf","thunderwave"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Jynx","details":"Jynx, L68","condition":"229/229","active":false,"stats":{"atk":136,"def":115,"spa":197,"spd":197,"spe":197},"moves":["seismictoss","lovelykiss","psychic","blizzard"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Slowbro","details":"Slowbro, L68","condition":"270/270","active":false,"stats":{"atk":170,"def":217,"spa":177,"spd":177,"spe":109},"moves":["psychic","surf","thunderwave","amnesia"],"baseAbility":"none","item":"","pokeball":"pokeball"}]},"rqid":2})#"sv;
	constexpr auto generation = Generation::one;
	auto const parsed = parse_team<generation>(initial);
	auto const expected = expected_generation_one_team();
	auto const parsed_str = to_string(parsed);
	auto const expected_str = to_string(expected);
	if (parsed_str != expected_str) {
		throw std::runtime_error(containers::concatenate<std::string>("Expected:\n\n"sv, expected_str, "\n\nbut got\n\n"sv, parsed_str));
	}
}

} // namespace

void test_parse_team() {
	std::cout << "Testing ps::parse_team\n";
	test_generation_one();
}

} // namespace ps
} // namespace technicalmachine