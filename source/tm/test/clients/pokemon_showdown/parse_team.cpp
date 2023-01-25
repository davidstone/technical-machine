// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.clients.ps.parse_team;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.iv;
import tm.stat.nature;

import tm.string_conversions.team;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto expected_generation_one_team() {
	constexpr auto generation = Generation::one;

	auto moves = [](auto... move_names) {
		return RegularMoves({Move(generation, move_names)...});
	};

	return Team<generation>({
		Pokemon<generation>(
			Species::Koffing,
			Level(88_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Fire_Blast, MoveName::Sludge, MoveName::Explosion, MoveName::Thunderbolt)
		),
		Pokemon<generation>(
			Species::Sandslash,
			Level(74_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Earthquake, MoveName::Body_Slam, MoveName::Rock_Slide, MoveName::Swords_Dance)
		),
		Pokemon<generation>(
			Species::Ditto,
			Level(88_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Transform)
		),
		Pokemon<generation>(
			Species::Pikachu,
			Level(88_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Agility, MoveName::Thunderbolt, MoveName::Surf, MoveName::Thunder_Wave)
		),
		Pokemon<generation>(
			Species::Jynx,
			Level(68_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Seismic_Toss, MoveName::Lovely_Kiss, MoveName::Psychic, MoveName::Blizzard)
		),
		Pokemon<generation>(
			Species::Slowbro,
			Level(68_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Psychic, MoveName::Surf, MoveName::Thunder_Wave, MoveName::Amnesia)
		)
	}, true);
}

TEST_CASE("Parse Pokemon Showdown generation 1 team", "[pokemon showdown]") {
	using namespace std::string_view_literals;
	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Fire Blast","id":"fireblast","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Sludge","id":"sludge","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Explosion","id":"explosion","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Thunderbolt","id":"thunderbolt","pp":24,"maxpp":24,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Koffing","details":"Koffing, L88","condition":"250/250","active":true,"stats":{"atk":201,"def":254,"spa":192,"spd":192,"spe":148},"moves":["fireblast","sludge","explosion","thunderbolt"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Sandslash","details":"Sandslash, L74","condition":"263/263","active":false,"stats":{"atk":221,"def":236,"spa":155,"spd":155,"spe":170},"moves":["earthquake","bodyslam","rockslide","swordsdance"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Ditto","details":"Ditto, L88","condition":"264/264","active":false,"stats":{"atk":171,"def":171,"spa":171,"spd":171,"spe":171},"moves":["transform"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Pikachu","details":"Pikachu, L88","condition":"241/241","active":false,"stats":{"atk":183,"def":139,"spa":174,"spd":174,"spe":245},"moves":["agility","thunderbolt","surf","thunderwave"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Jynx","details":"Jynx, L68","condition":"229/229","active":false,"stats":{"atk":136,"def":115,"spa":197,"spd":197,"spe":197},"moves":["seismictoss","lovelykiss","psychic","blizzard"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Slowbro","details":"Slowbro, L68","condition":"270/270","active":false,"stats":{"atk":170,"def":217,"spa":177,"spd":177,"spe":109},"moves":["psychic","surf","thunderwave","amnesia"],"baseAbility":"none","item":"","pokeball":"pokeball"}]},"rqid":2})#"sv;
	constexpr auto generation = Generation::one;
	auto const parsed = ps::parse_team<generation>(initial);
	auto const expected = expected_generation_one_team();
	auto const parsed_str = to_string(parsed);
	auto const expected_str = to_string(expected);
	CHECK(parsed_str == expected_str);
}

auto expected_generation_two_team() {
	constexpr auto generation = Generation::two;

	auto moves = [](auto... move_names) {
		return RegularMoves({Move(generation, move_names)...});
	};

	return Team<generation>({
		Pokemon<generation>(
			Species::Noctowl,
			Level(80_bi),
			Gender::male,
			Item::None,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Thief, MoveName::Hypnosis, MoveName::Night_Shade, MoveName::Return)
		),
		Pokemon<generation>(
			Species::Qwilfish,
			Level(74_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Spikes, MoveName::Sludge_Bomb, MoveName::Surf, MoveName::Curse)
		),
		Pokemon<generation>(
			Species::Hypno,
			Level(74_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Curse, MoveName::Body_Slam, MoveName::Psychic, MoveName::Rest)
		),
		Pokemon<generation>(
			Species::Misdreavus,
			Level(68_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Thunderbolt, MoveName::Shadow_Ball, MoveName::Hypnosis, MoveName::Pain_Split)
		),
		Pokemon<generation>(
			Species::Machamp,
			Level(68_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			default_combined_stats<generation>,
			moves(MoveName::Rest, MoveName::Sleep_Talk, MoveName::Cross_Chop, MoveName::Curse)
		),
		Pokemon<generation>(
			Species::Sunflora,
			Level(80_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(13_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			moves(MoveName::Growth, MoveName::Giga_Drain, MoveName::Synthesis, MoveName::Hidden_Power)
		)
	}, true);
}

TEST_CASE("Parse Pokemon Showdown generation 2 team", "[pokemon showdown]") {
	using namespace std::string_view_literals;
	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Thief","id":"thief","pp":16,"maxpp":16,"target":"normal","disabled":false},{"move":"Hypnosis","id":"hypnosis","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Night Shade","id":"nightshade","pp":24,"maxpp":24,"target":"normal","disabled":false},{"move":"Return 102","id":"return","pp":32,"maxpp":32,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Noctowl","details":"Noctowl, L80, M","condition":"324/324","active":true,"stats":{"atk":159,"def":159,"spa":201,"spd":233,"spe":191},"moves":["thief","hypnosis","nightshade","return102"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Qwilfish","details":"Qwilfish, L74, M","condition":"249/249","active":false,"stats":{"atk":214,"def":184,"spa":155,"spd":155,"spe":199},"moves":["spikes","sludgebomb","surf","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Hypno","details":"Hypno, L74, M","condition":"278/278","active":false,"stats":{"atk":181,"def":177,"spa":181,"spd":244,"spe":172},"moves":["curse","bodyslam","psychic","rest"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Misdreavus","details":"Misdreavus, L68, M","condition":"222/222","active":false,"stats":{"atk":149,"def":149,"spa":183,"spd":183,"spe":183},"moves":["thunderbolt","shadowball","hypnosis","painsplit"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Machamp","details":"Machamp, L68, M","condition":"263/263","active":false,"stats":{"atk":245,"def":177,"spa":156,"spd":183,"spe":143},"moves":["rest","sleeptalk","crosschop","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Sunflora","details":"Sunflora, L80, M","condition":"284/284","active":false,"stats":{"atk":199,"def":164,"spa":247,"spd":215,"spe":127},"moves":["growth","gigadrain","synthesis","hiddenpowerice"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"}]},"rqid":2})#"sv;
	constexpr auto generation = Generation::two;

	auto const parsed = ps::parse_team<generation>(initial);
	auto const expected = expected_generation_two_team();
	auto const parsed_str = to_string(parsed);
	auto const expected_str = to_string(expected);
	CHECK(parsed_str == expected_str);
}

} // namespace
} // namespace technicalmachine