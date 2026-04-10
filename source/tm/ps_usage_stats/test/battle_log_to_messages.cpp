module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.ps_usage_stats.battle_log_to_messages;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.event_block;
import tm.clients.ps.parsed_message;
import tm.clients.ps.switch_message;

import tm.clients.party;

import tm.move.move_name;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.ps_usage_stats.battle_log_to_messages;

import tm.status.status_name;

import tm.gender;
import tm.generation;
import tm.nlohmann_json;
import tm.visible_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;
using namespace bounded::literal;
using namespace ps_usage_stats;

constexpr auto visible_hp(auto const current, auto const max) -> VisibleHP {
	return VisibleHP(CurrentVisibleHP(current), MaxVisibleHP(max));
}

constexpr auto p1 = Party(0_bi);
constexpr auto p2 = Party(1_bi);
constexpr auto missed = true;
constexpr auto did_not_miss = false;

TEST_CASE("Battle log to messages", "[battle_log_to_messages]") {
	auto const parsed = battle_log_to_messages(
		nlohmann::json({
			"|j|☆355",
			"|j|☆160",
			"|t:|1641491317",
			"|gametype|singles",
			"|player|p1|355|60|1100",
			"|player|p2|160|113|1200",
			"|teamsize|p1|2",
			"|teamsize|p2|3",
			"|gen|1",
			"|tier|[Gen 1] OU",
			"|rated|",
			"|rule|Desync Clause Mod: Desyncs changed to move failure.",
			"|rule|Sleep Clause Mod: Limit one foe put to sleep",
			"|rule|Freeze Clause Mod: Limit one foe frozen",
			"|rule|Species Clause: Limit one of each Pokémon",
			"|rule|OHKO Clause: OHKO moves are banned",
			"|rule|Evasion Moves Clause: Evasion moves are banned",
			"|rule|Endless Battle Clause: Forcing endless battles is banned",
			"|rule|HP Percentage Mod: HP is shown in percentages",
			"|",
			"|t:|1641491317",
			"|start",
			"|switch|p1a: Tauros|Tauros|353/353",
			"|switch|p2a: Starmie|Starmie|323/323",
			"|turn|1",
			"|",
			"|t:|1641491328",
			"|switch|p2a: Cloyster|Cloyster|303/303",
			"|move|p1a: Tauros|Body Slam|p2a: Cloyster",
			"|-damage|p2a: Cloyster|237/303",
			"|",
			"|upkeep",
			"|turn|2",
			"|",
			"|t:|1641491330",
			"|move|p1a: Tauros|Body Slam|p2a: Cloyster",
			"|-damage|p2a: Cloyster|167/303",
			"|move|p2a: Cloyster|Blizzard|p1a: Tauros",
			"|-damage|p1a: Tauros|186/353",
			"|",
			"|upkeep",
			"|turn|3",
			"|",
			"|t:|1641491334",
			"|move|p1a: Tauros|Body Slam|p2a: Cloyster",
			"|-damage|p2a: Cloyster|98/303",
			"|-status|p2a: Cloyster|par",
			"|move|p2a: Cloyster|Blizzard|p1a: Tauros",
			"|-crit|p1a: Tauros",
			"|-damage|p1a: Tauros|0 fnt",
			"|faint|p1a: Tauros",
			"|",
			"|t:|1641491340",
			"|switch|p1a: Gengar|Gengar|323/323",
			"|turn|4",
		})
	);
	auto const expected = BattleLogMessages(
		ps::BattleInitMessage(
			Generation::one,
			{
				ps::BattleInitMessage::Team(
					ps::SwitchMessage(
						p1,
						Species::Tauros,
						"Tauros"sv,
						Level(100_bi),
						Gender::genderless,
						visible_hp(353_bi, 353_bi),
						StatusName::clear
					),
					2_bi
				),
				ps::BattleInitMessage::Team(
					ps::SwitchMessage(
						p2,
						Species::Starmie,
						"Starmie"sv,
						Level(100_bi),
						Gender::genderless,
						visible_hp(323_bi, 323_bi),
						StatusName::clear
					),
					3_bi
				)
			}
		),
		containers::dynamic_array<ps::EventBlock>({
			ps::EventBlock({
				ps::SeparatorMessage(),
				ps::SwitchMessage(
					p2,
					Species::Cloyster,
					"Cloyster"sv,
					Level(100_bi),
					Gender::genderless,
					visible_hp(303_bi, 303_bi),
					StatusName::clear
				),
				ps::MoveMessage(
					p1,
					MoveName::Body_Slam,
					did_not_miss,
					true
				),
				ps::DamageMessage(
					p2,
					StatusName::clear,
					visible_hp(237_bi, 303_bi)
				),
				ps::SeparatorMessage(),
				ps::TurnMessage(2_bi),
			}),
			ps::EventBlock({
				ps::SeparatorMessage(),
				ps::MoveMessage(
					p1,
					MoveName::Body_Slam,
					did_not_miss,
					true
				),
				ps::DamageMessage(
					p2,
					StatusName::clear,
					visible_hp(167_bi, 303_bi)
				),
				ps::MoveMessage(
					p2,
					MoveName::Blizzard,
					did_not_miss,
					true
				),
				ps::DamageMessage(
					p1,
					StatusName::clear,
					visible_hp(186_bi, 353_bi)
				),
				ps::SeparatorMessage(),
				ps::TurnMessage(3_bi),
			}),
			ps::EventBlock({
				ps::SeparatorMessage(),
				ps::MoveMessage(
					p1,
					MoveName::Body_Slam,
					did_not_miss,
					true
				),
				ps::DamageMessage(
					p2,
					StatusName::clear,
					visible_hp(98_bi, 303_bi)
				),
				ps::MoveStatus(p2, StatusName::paralysis),
				ps::MoveMessage(
					p2,
					MoveName::Blizzard,
					did_not_miss,
					true
				),
				ps::CriticalHitMessage(p2),
				ps::DamageMessage(
					p1,
					StatusName::clear,
					visible_hp(0_bi, 100_bi)
				),
			}),
			ps::EventBlock({
				ps::SeparatorMessage(),
				ps::SwitchMessage(
					p1,
					Species::Gengar,
					"Gengar"sv,
					Level(100_bi),
					Gender::genderless,
					visible_hp(323_bi, 323_bi),
					StatusName::clear
				),
				ps::TurnMessage(4_bi),
			}),
		})
	);
	CHECK(parsed == expected);
}

} // namespace technicalmachine