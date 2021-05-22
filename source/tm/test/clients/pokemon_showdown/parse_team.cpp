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
		default_combined_stats<generation>,
		moves(Moves::Fire_Blast, Moves::Sludge, Moves::Explosion, Moves::Thunderbolt)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Sandslash,
		Level(74_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Earthquake, Moves::Body_Slam, Moves::Rock_Slide, Moves::Swords_Dance)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Ditto,
		Level(88_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Transform)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Pikachu,
		Level(88_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Agility, Moves::Thunderbolt, Moves::Surf, Moves::Thunder_Wave)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Jynx,
		Level(68_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Seismic_Toss, Moves::Lovely_Kiss, Moves::Psychic, Moves::Blizzard)
	));
	
	team.add_pokemon(Pokemon<generation>(
		Species::Slowbro,
		Level(68_bi),
		Gender::genderless,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
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

auto expected_generation_two_team() {
	constexpr auto generation = Generation::two;
	auto team = Team<generation>(6_bi, true);

	auto moves = [](auto... move_names) {
		return RegularMoves({Move(generation, move_names)...});
	};

	team.add_pokemon(Pokemon<generation>(
		Species::Noctowl,
		Level(80_bi),
		Gender::male,
		Item::None,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Thief, Moves::Hypnosis, Moves::Night_Shade, Moves::Return)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Qwilfish,
		Level(74_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Spikes, Moves::Sludge_Bomb, Moves::Surf, Moves::Curse)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Hypno,
		Level(74_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Curse, Moves::Body_Slam, Moves::Psychic, Moves::Rest)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Misdreavus,
		Level(68_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Thunderbolt, Moves::Shadow_Ball, Moves::Hypnosis, Moves::Pain_Split)
	));

	team.add_pokemon(Pokemon<generation>(
		Species::Machamp,
		Level(68_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Honey_Gather,
		default_combined_stats<generation>,
		moves(Moves::Rest, Moves::Sleep_Talk, Moves::Cross_Chop, Moves::Curse)
	));
	
	team.add_pokemon(Pokemon<generation>(
		Species::Sunflora,
		Level(80_bi),
		Gender::male,
		Item::Leftovers,
		Ability::Honey_Gather,
		CombinedStats<generation>{
			Nature::Hardy,
			IVs(DVs(DV(15_bi), DV(13_bi), DV(15_bi), DV(15_bi))),
			default_evs<generation>
		},
		moves(Moves::Growth, Moves::Giga_Drain, Moves::Synthesis, Moves::Hidden_Power)
	));

	return team;
}

void test_generation_two() {
	using namespace std::string_view_literals;
	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Thief","id":"thief","pp":16,"maxpp":16,"target":"normal","disabled":false},{"move":"Hypnosis","id":"hypnosis","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Night Shade","id":"nightshade","pp":24,"maxpp":24,"target":"normal","disabled":false},{"move":"Return 102","id":"return","pp":32,"maxpp":32,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Noctowl","details":"Noctowl, L80, M","condition":"324/324","active":true,"stats":{"atk":159,"def":159,"spa":201,"spd":233,"spe":191},"moves":["thief","hypnosis","nightshade","return102"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Qwilfish","details":"Qwilfish, L74, M","condition":"249/249","active":false,"stats":{"atk":214,"def":184,"spa":155,"spd":155,"spe":199},"moves":["spikes","sludgebomb","surf","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Hypno","details":"Hypno, L74, M","condition":"278/278","active":false,"stats":{"atk":181,"def":177,"spa":181,"spd":244,"spe":172},"moves":["curse","bodyslam","psychic","rest"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Misdreavus","details":"Misdreavus, L68, M","condition":"222/222","active":false,"stats":{"atk":149,"def":149,"spa":183,"spd":183,"spe":183},"moves":["thunderbolt","shadowball","hypnosis","painsplit"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Machamp","details":"Machamp, L68, M","condition":"263/263","active":false,"stats":{"atk":245,"def":177,"spa":156,"spd":183,"spe":143},"moves":["rest","sleeptalk","crosschop","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Sunflora","details":"Sunflora, L80, M","condition":"284/284","active":false,"stats":{"atk":199,"def":164,"spa":247,"spd":215,"spe":127},"moves":["growth","gigadrain","synthesis","hiddenpowerice"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"}]},"rqid":2})#"sv;
	constexpr auto generation = Generation::two;

	auto const parsed = parse_team<generation>(initial);
	auto const expected = expected_generation_two_team();
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
	test_generation_two();
}

} // namespace ps
} // namespace technicalmachine