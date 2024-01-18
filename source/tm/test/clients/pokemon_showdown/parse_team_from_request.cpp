// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

export module tm.test.parse_team_from_request;

import tm.clients.ps.parse_team_from_request;
import tm.clients.ps.parsed_side;

import tm.clients.party;

import tm.move.move_name;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.iv;
import tm.stat.nature;

import tm.type.type;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.visible_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto visible_hp(auto const current, auto const max) {
	return VisibleHP(CurrentVisibleHP(current), MaxVisibleHP(max));
}

TEST_CASE("Parse Pokemon Showdown generation 1 team", "[pokemon showdown]") {
	auto const expected = ps::ParsedSide(
		Party(0_bi),
		{{
			{
				.species = Species::Koffing,
				.level = Level(88_bi),
				.stats = {
					.hp = visible_hp(250_bi, 250_bi),
					.atk = 201_bi,
					.def = 254_bi,
					.spa = 192_bi,
					.spd = 192_bi,
					.spe = 148_bi
				},
				.moves = {{
					MoveName::Fire_Blast,
					MoveName::Sludge,
					MoveName::Explosion,
					MoveName::Thunderbolt,
				}},
			},
			{
				.species = Species::Sandslash,
				.level = Level(74_bi),
				.stats = {
					.hp = visible_hp(263_bi, 263_bi),
					.atk = 221_bi,
					.def = 236_bi,
					.spa = 155_bi,
					.spd = 155_bi,
					.spe = 170_bi
				},
				.moves = {{
					MoveName::Earthquake,
					MoveName::Body_Slam,
					MoveName::Rock_Slide,
					MoveName::Swords_Dance,
				}}
			},
			{
				.species = Species::Ditto,
				.level = Level(88_bi),
				.stats = {
					.hp = visible_hp(264_bi, 264_bi),
					.atk = 171_bi,
					.def = 171_bi,
					.spa = 171_bi,
					.spd = 171_bi,
					.spe = 171_bi
				},
				.moves = {{
					MoveName::Transform,
				}}
			},
			{
				.species = Species::Pikachu,
				.level = Level(88_bi),
				.stats = {
					.hp = visible_hp(241_bi, 241_bi),
					.atk = 183_bi,
					.def = 139_bi,
					.spa = 174_bi,
					.spd = 174_bi,
					.spe = 245_bi
				},
				.moves = {{
					MoveName::Agility,
					MoveName::Thunderbolt,
					MoveName::Surf,
					MoveName::Thunder_Wave,
				}}
			},
			{
				.species = Species::Jynx,
				.level = Level(68_bi),
				.stats = {
					.hp = visible_hp(229_bi, 229_bi),
					.atk = 136_bi,
					.def = 115_bi,
					.spa = 197_bi,
					.spd = 197_bi,
					.spe = 197_bi
				},
				.moves = {{
					MoveName::Seismic_Toss,
					MoveName::Lovely_Kiss,
					MoveName::Psychic,
					MoveName::Blizzard,
				}}
			},
			{
				.species = Species::Slowbro,
				.level = Level(68_bi),
				.stats = {
					.hp = visible_hp(270_bi, 270_bi),
					.atk = 170_bi,
					.def = 217_bi,
					.spa = 177_bi,
					.spd = 177_bi,
					.spe = 109_bi
				},
				.moves = {{
					MoveName::Psychic,
					MoveName::Surf,
					MoveName::Thunder_Wave,
					MoveName::Amnesia,
				}}
			},
		}}
	);

	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Fire Blast","id":"fireblast","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Sludge","id":"sludge","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Explosion","id":"explosion","pp":8,"maxpp":8,"target":"normal","disabled":false},{"move":"Thunderbolt","id":"thunderbolt","pp":24,"maxpp":24,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Koffing","details":"Koffing, L88","condition":"250/250","active":true,"stats":{"atk":201,"def":254,"spa":192,"spd":192,"spe":148},"moves":["fireblast","sludge","explosion","thunderbolt"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Sandslash","details":"Sandslash, L74","condition":"263/263","active":false,"stats":{"atk":221,"def":236,"spa":155,"spd":155,"spe":170},"moves":["earthquake","bodyslam","rockslide","swordsdance"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Ditto","details":"Ditto, L88","condition":"264/264","active":false,"stats":{"atk":171,"def":171,"spa":171,"spd":171,"spe":171},"moves":["transform"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Pikachu","details":"Pikachu, L88","condition":"241/241","active":false,"stats":{"atk":183,"def":139,"spa":174,"spd":174,"spe":245},"moves":["agility","thunderbolt","surf","thunderwave"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Jynx","details":"Jynx, L68","condition":"229/229","active":false,"stats":{"atk":136,"def":115,"spa":197,"spd":197,"spe":197},"moves":["seismictoss","lovelykiss","psychic","blizzard"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Slowbro","details":"Slowbro, L68","condition":"270/270","active":false,"stats":{"atk":170,"def":217,"spa":177,"spd":177,"spe":109},"moves":["psychic","surf","thunderwave","amnesia"],"baseAbility":"none","item":"","pokeball":"pokeball"}]},"rqid":2})#"sv;
	auto const parsed = ps::parse_team_from_request(initial);
	CHECK(parsed == expected);
}

TEST_CASE("Parse Pokemon Showdown generation 2 team", "[pokemon showdown]") {
	auto const expected = ps::ParsedSide(
		Party(0_bi),
		{{
			{
				.species = Species::Noctowl,
				.level = Level(80_bi),
				.gender = Gender::male,
				.stats = {
					.hp = visible_hp(324_bi, 324_bi),
					.atk = 159_bi,
					.def = 159_bi,
					.spa = 201_bi,
					.spd = 233_bi,
					.spe = 191_bi
				},
				.moves = {{
					MoveName::Thief,
					MoveName::Hypnosis,
					MoveName::Night_Shade,
					MoveName::Return,
				}}
			},
			{
				.species = Species::Qwilfish,
				.level = Level(74_bi),
				.gender = Gender::male,
				.item = Item::Leftovers,
				.stats = {
					.hp = visible_hp(249_bi, 249_bi),
					.atk = 214_bi,
					.def = 184_bi,
					.spa = 155_bi,
					.spd = 155_bi,
					.spe = 199_bi
				},
				.moves = {{
					MoveName::Spikes,
					MoveName::Sludge_Bomb,
					MoveName::Surf,
					MoveName::Curse,
				}}
			},
			{
				.species = Species::Hypno,
				.level = Level(74_bi),
				.gender = Gender::male,
				.item = Item::Leftovers,
				.stats = {
					.hp = visible_hp(278_bi, 278_bi),
					.atk = 181_bi,
					.def = 177_bi,
					.spa = 181_bi,
					.spd = 244_bi,
					.spe = 172_bi
				},
				.moves = {{
					MoveName::Curse,
					MoveName::Body_Slam,
					MoveName::Psychic,
					MoveName::Rest,
				}}
			},
			{
				.species = Species::Misdreavus,
				.level = Level(68_bi),
				.gender = Gender::male,
				.item = Item::Leftovers,
				.stats = {
					.hp = visible_hp(222_bi, 222_bi),
					.atk = 149_bi,
					.def = 149_bi,
					.spa = 183_bi,
					.spd = 183_bi,
					.spe = 183_bi
				},
				.moves = {{
					MoveName::Thunderbolt,
					MoveName::Shadow_Ball,
					MoveName::Hypnosis,
					MoveName::Pain_Split,
				}}
			},
			{
				.species = Species::Machamp,
				.level = Level(68_bi),
				.gender = Gender::male,
				.item = Item::Leftovers,
				.stats = {
					.hp = visible_hp(263_bi, 263_bi),
					.atk = 245_bi,
					.def = 177_bi,
					.spa = 156_bi,
					.spd = 183_bi,
					.spe = 143_bi
				},
				.moves = {{
					MoveName::Rest,
					MoveName::Sleep_Talk,
					MoveName::Cross_Chop,
					MoveName::Curse,
				}}
			},
			{
				.species = Species::Sunflora,
				.level = Level(80_bi),
				.gender = Gender::male,
				.item = Item::Leftovers,
				.stats = {
					.hp = visible_hp(284_bi, 284_bi),
					.atk = 199_bi,
					.def = 164_bi,
					.spa = 247_bi,
					.spd = 215_bi,
					.spe = 127_bi
				},
				.moves = {{
					MoveName::Growth,
					MoveName::Giga_Drain,
					MoveName::Synthesis,
					MoveName::Hidden_Power,
				}},
				.hidden_power_type = Type::Ice
			},
		}}
	);

	constexpr auto initial =
		R"#({"active":[{"moves":[{"move":"Thief","id":"thief","pp":16,"maxpp":16,"target":"normal","disabled":false},{"move":"Hypnosis","id":"hypnosis","pp":32,"maxpp":32,"target":"normal","disabled":false},{"move":"Night Shade","id":"nightshade","pp":24,"maxpp":24,"target":"normal","disabled":false},{"move":"Return 102","id":"return","pp":32,"maxpp":32,"target":"normal","disabled":false}]}],"side":{"name":"Technical Machine","id":"p1","pokemon":[{"ident":"p1: Noctowl","details":"Noctowl, L80, M","condition":"324/324","active":true,"stats":{"atk":159,"def":159,"spa":201,"spd":233,"spe":191},"moves":["thief","hypnosis","nightshade","return102"],"baseAbility":"none","item":"","pokeball":"pokeball"},{"ident":"p1: Qwilfish","details":"Qwilfish, L74, M","condition":"249/249","active":false,"stats":{"atk":214,"def":184,"spa":155,"spd":155,"spe":199},"moves":["spikes","sludgebomb","surf","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Hypno","details":"Hypno, L74, M","condition":"278/278","active":false,"stats":{"atk":181,"def":177,"spa":181,"spd":244,"spe":172},"moves":["curse","bodyslam","psychic","rest"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Misdreavus","details":"Misdreavus, L68, M","condition":"222/222","active":false,"stats":{"atk":149,"def":149,"spa":183,"spd":183,"spe":183},"moves":["thunderbolt","shadowball","hypnosis","painsplit"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Machamp","details":"Machamp, L68, M","condition":"263/263","active":false,"stats":{"atk":245,"def":177,"spa":156,"spd":183,"spe":143},"moves":["rest","sleeptalk","crosschop","curse"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"},{"ident":"p1: Sunflora","details":"Sunflora, L80, M","condition":"284/284","active":false,"stats":{"atk":199,"def":164,"spa":247,"spd":215,"spe":127},"moves":["growth","gigadrain","synthesis","hiddenpowerice"],"baseAbility":"none","item":"leftovers","pokeball":"pokeball"}]},"rqid":2})#"sv;

	auto const parsed = ps::parse_team_from_request(initial);
	CHECK(parsed == expected);
}

} // namespace
} // namespace technicalmachine