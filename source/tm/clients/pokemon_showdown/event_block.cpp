// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.event_block;

import tm.clients.ps.in_message;
import tm.clients.ps.make_party;
import tm.clients.ps.message_block;
import tm.clients.ps.party_from_player_id;
import tm.clients.ps.parse_effect_source;
import tm.clients.ps.parse_set_hp_message;
import tm.clients.ps.parse_status;
import tm.clients.ps.parse_switch;
import tm.clients.ps.switch_message;

import tm.clients.party;
import tm.clients.turn_count;

import tm.move.move_name;

import tm.status.check_ability_can_status;
import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.weather;

import tm.ability;
import tm.item;
import tm.split_view;
import tm.visible_hp;
import tm.weather;

import bounded;
import containers;
import std_module;
import tv;

// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
namespace technicalmachine::ps {

using namespace bounded::literal;
using namespace std::string_view_literals;

export struct SeparatorMessage {
	friend auto operator==(SeparatorMessage, SeparatorMessage) -> bool = default;
};

export struct AbilityMessage {
	Party party;
	Ability ability;
	tv::optional<Ability> other_ability = tv::none;
	friend auto operator==(AbilityMessage, AbilityMessage) -> bool = default;
};

export struct ForewarnMessage {
	Party party;
	MoveName other_move;
	friend auto operator==(ForewarnMessage, ForewarnMessage) -> bool = default;
};

export struct ShedSkinMessage {
	Party party;
	friend auto operator==(ShedSkinMessage, ShedSkinMessage) -> bool = default;
};

export struct ItemMessage {
	Party party;
	Item item;
	friend auto operator==(ItemMessage, ItemMessage) -> bool = default;
};

export struct DamageSubstituteMessage {
	Party party;
	friend auto operator==(DamageSubstituteMessage, DamageSubstituteMessage) -> bool = default;
};
export struct DestroySubstituteMessage {
	Party party;
	friend auto operator==(DestroySubstituteMessage, DestroySubstituteMessage) -> bool = default;
};

export struct FlinchMessage {
	Party party;
	friend auto operator==(FlinchMessage, FlinchMessage) -> bool = default;
};

export struct FocusPunchMessage {
	Party party;
	friend auto operator==(FocusPunchMessage, FocusPunchMessage) -> bool = default;
};

export struct FrozenSolidMessage {
	Party party;
	friend auto operator==(FrozenSolidMessage, FrozenSolidMessage) -> bool = default;
};

export struct FullyParalyzedMessage {
	Party party;
	friend auto operator==(FullyParalyzedMessage, FullyParalyzedMessage) -> bool = default;
};

export struct StillAsleepMessage {
	Party party;
	friend auto operator==(StillAsleepMessage, StillAsleepMessage) -> bool = default;
};

export struct RechargingMessage {
	Party party;
	friend auto operator==(RechargingMessage, RechargingMessage) -> bool = default;
};

export struct CriticalHitMessage {
	Party party;
	friend auto operator==(CriticalHitMessage, CriticalHitMessage) -> bool = default;
};

export struct StatusClearMessage {
	Party party;
	StatusName status;
	friend auto operator==(StatusClearMessage, StatusClearMessage) -> bool = default;
};

export struct DamageMessage {
	Party party;
	StatusName status;
	VisibleHP hp;
	friend auto operator==(DamageMessage, DamageMessage) -> bool = default;
};

export struct HitSelfMessage {
	Party party;
	StatusName status;
	VisibleHP hp;
	friend auto operator==(HitSelfMessage, HitSelfMessage) -> bool = default;
};

export struct RecoilMessage {
	Party party;
	StatusName status;
	VisibleHP hp;
	friend auto operator==(RecoilMessage, RecoilMessage) -> bool = default;
};

export struct ConfusionEndedMessage {
	Party party;
	friend auto operator==(ConfusionEndedMessage, ConfusionEndedMessage) -> bool = default;
};

export struct TauntEndedMessage {
	Party party;
	friend auto operator==(TauntEndedMessage, TauntEndedMessage) -> bool = default;
};

export struct HPMessage {
	Party party;
	StatusName status;
	VisibleHP hp;
	struct Other {
		friend auto operator==(Other, Other) -> bool = default;
	};
	tv::variant<Other, Item, Ability> item = Other();
	friend auto operator==(HPMessage, HPMessage) -> bool = default;
};

export struct MoveMessage {
	Party party;
	MoveName move;
	bool miss;
	friend auto operator==(MoveMessage, MoveMessage) -> bool = default;
};

export struct EffectivenessMessage {
	enum class Value : std::uint8_t {
		immune, not_very, super
	};
	using enum Value;
	Party party;
	Value value;
	friend auto operator==(EffectivenessMessage, EffectivenessMessage) -> bool = default;
};

export struct ScreenEndMessage {
	enum class ScreenName : std::uint8_t {
		Reflect,
		Light_Screen,
		Mist,
		Safeguard,
		Lucky_Chant,
		Tailwind
	};
	using enum ScreenName;
	Party party;
	ScreenName name;
	friend auto operator==(ScreenEndMessage, ScreenEndMessage) -> bool = default;
};

constexpr auto parse_condition(std::string_view const str) -> tv::optional<ScreenEndMessage::ScreenName> {
	using enum ScreenEndMessage::ScreenName;
	if (str == "Reflect") {
		return Reflect;
	} else if (str == "Light Screen") {
		return Light_Screen;
	} else if (str == "Mist") {
		return Mist;
	} else if (str == "Safeguard") {
		return Safeguard;
	} else if (str == "Lucky Chant") {
		return Lucky_Chant;
	} else if (str == "Tailwind") {
		return Tailwind;
	} else {
		return tv::none;
	}
}

export struct PhazeMessage : SwitchMessage {
	friend auto operator==(PhazeMessage, PhazeMessage) -> bool = default;
};

export struct RampageEndMessage {
	Party party;
	friend auto operator==(RampageEndMessage, RampageEndMessage) -> bool = default;
};

export struct StartConfusionMessage {
	Party party;
	friend auto operator==(StartConfusionMessage, StartConfusionMessage) -> bool = default;
};

export struct MoveStatus {
	Party party;
	StatusName status;
	friend auto operator==(MoveStatus, MoveStatus) -> bool = default;
};

export struct AbilityStatusMessage {
	Party party;
	Ability ability;
	StatusName status;
	friend auto operator==(AbilityStatusMessage, AbilityStatusMessage) -> bool = default;
};

export struct TurnMessage {
	TurnCount count;
	friend auto operator==(TurnMessage, TurnMessage) -> bool = default;
};

export struct EndOfTurnMessage {
	friend auto operator==(EndOfTurnMessage, EndOfTurnMessage) -> bool = default;
};

export struct WeatherMessage {
	Weather weather;
	friend auto operator==(WeatherMessage, WeatherMessage) -> bool = default;
};

export struct BattleFinishedMessage {
	friend auto operator==(BattleFinishedMessage, BattleFinishedMessage) -> bool = default;
};

export using ParsedMessage = tv::variant<
	SeparatorMessage,
	AbilityMessage,
	ForewarnMessage,
	ShedSkinMessage,
	ItemMessage,
	DamageSubstituteMessage,
	DestroySubstituteMessage,
	FlinchMessage,
	FocusPunchMessage,
	FrozenSolidMessage,
	FullyParalyzedMessage,
	StillAsleepMessage,
	RechargingMessage,
	CriticalHitMessage,
	StatusClearMessage,
	DamageMessage,
	HitSelfMessage,
	RecoilMessage,
	ConfusionEndedMessage,
	TauntEndedMessage,
	HPMessage,
	MoveMessage,
	EffectivenessMessage,
	ScreenEndMessage,
	SwitchMessage,
	PhazeMessage,
	RampageEndMessage,
	StartConfusionMessage,
	MoveStatus,
	AbilityStatusMessage,
	TurnMessage,
	EndOfTurnMessage,
	WeatherMessage,
	BattleFinishedMessage
>;

static_assert(bounded::equality_comparable<ParsedMessage>);

constexpr auto party_from_side_id(std::string_view const str) -> Party {
	return make_party(str.substr(0, 2));
}

constexpr auto parse_message(InMessage message) -> tv::optional<ParsedMessage> {
	auto const type = message.type();
	if (type == "") {
		return SeparatorMessage();
	} else if (type == "-ability") {
		auto const party = party_from_player_id(message.pop());
		auto const ability = from_string<Ability>(message.pop());
		auto const source = parse_from_source(message.pop());
		return tv::visit(source, tv::overload(
			[&](MainEffect) -> AbilityMessage {
				return AbilityMessage(party, ability);
			},
			[&](Ability const source_ability) -> AbilityMessage {
				switch (source_ability) {
					case Ability::Trace:
						return AbilityMessage(party, source_ability, ability);
					default:
						throw std::runtime_error("Invalid ability to get another ability");
				}
			},
			[](auto) -> AbilityMessage {
				throw std::runtime_error("Invalid ability source");
			}
		));
	} else if (type == "-activate") {
		auto const party = party_from_player_id(message.pop());
		auto const [category, source] = split_view(message.pop(), ": "sv);
		auto const details = message.pop();
		return tv::visit(parse_effect_source(category, source), tv::overload(
			[](MainEffect) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -activate source MainEffect");
			},
			[](FromConfusion) -> tv::optional<ParsedMessage> {
				// This means that I did not snap out of confusion
				// TODO: Should I send this as a message?
				return tv::none;
			},
			[](FromEntryHazards) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -activate source FromEntryHazards");
			},
			[](FromMiscellaneous) -> tv::optional<ParsedMessage> {
				return tv::none;
			},
			[](FromMove) -> tv::optional<ParsedMessage> {
				return tv::none;
			},
			[](FromRecoil) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -activate source FromRecoil");
			},
			[&](FromSubstitute) -> tv::optional<ParsedMessage> {
				return DamageSubstituteMessage(party);
			},
			[&](Ability const ability) -> tv::optional<ParsedMessage> {
				switch (ability) {
					case Ability::Forewarn:
						return ForewarnMessage(
							party,
							from_string<MoveName>(details)
						);
					case Ability::Shed_Skin:
						return ShedSkinMessage(party);
					default:
						return AbilityMessage(party, ability);
				}
			},
			[&](Item const item) -> tv::optional<ParsedMessage> {
				return ItemMessage(party, item);
			}
		));
	} else if (type == "-anim") {
		return tv::none;
	} else if (type == "-boost") {
		// TODO: Acupressure?
		return tv::none;
	} else if (type == "cant") {
		auto const party = party_from_player_id(message.pop());
		auto const reason = message.pop();
		if (reason == "flinch") {
			return FlinchMessage(party);
		} else if (reason == "Focus Punch") {
			return FocusPunchMessage(party);
		} else if (reason == "frz") {
			return FrozenSolidMessage(party);
		} else if (reason == "par") {
			return FullyParalyzedMessage(party);
		} else if (reason == "slp") {
			return StillAsleepMessage(party);
		} else if (reason == "recharge") {
			return RechargingMessage(party);
		} else {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Received unknown \"cant\" reason: "sv,
				reason
			));
		}
	} else if (type == "-center") {
		return tv::none;
	} else if (type == "-clearallboost") {
		return tv::none;
	} else if (type == "-crit") {
		auto const party = other(party_from_player_id(message.pop()));
		return CriticalHitMessage(party);
	} else if (type == "-curestatus") {
		auto const party = party_from_player_id(message.pop());
		auto const status = parse_status(message.pop());
		auto const source = parse_from_source(message.pop());
		return tv::visit(source, tv::overload(
			[&](MainEffect) -> tv::optional<ParsedMessage> {
				return StatusClearMessage(party, status);
			},
			[&](Ability const ability) -> tv::optional<ParsedMessage> {
				switch (ability) {
					case Ability::Natural_Cure:
						return AbilityMessage(party, ability);
					default:
						throw std::runtime_error("Unexpected ability cured status");
				}
			},
			[](auto) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -curestatus source");
			}
		));
	} else if (type == "-cureteam") {
		return tv::none;
	} else if (type == "-damage") {
		auto const parsed = parse_set_hp_message(message);
		return tv::visit(parsed.source, tv::overload(
			[&](MainEffect) -> tv::optional<ParsedMessage> {
				return DamageMessage(parsed.party, parsed.status, parsed.hp);
			},
			[&](FromConfusion) -> tv::optional<ParsedMessage> {
				return HitSelfMessage(parsed.party, parsed.status, parsed.hp);
			},
			[&](FromEntryHazards) -> tv::optional<ParsedMessage> {
				return HPMessage(parsed.party, parsed.status, parsed.hp);
			},
			[&](FromMiscellaneous) -> tv::optional<ParsedMessage> {
				return HPMessage(parsed.party, parsed.status, parsed.hp);
			},
			[&](FromRecoil) -> tv::optional<ParsedMessage> {
				return RecoilMessage(parsed.party, parsed.status, parsed.hp);
			},
			[&](Item const value) -> tv::optional<ParsedMessage> {
				return ItemMessage(parsed.party, value);
			},
			[&](Ability const value) -> tv::optional<ParsedMessage> {
				// TODO: Do we need get the actual source from the remainder?
				return AbilityMessage(other(parsed.party), value);
			},
			[](FromMove) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Invalid -damage source");
			},
			[](FromSubstitute) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Invalid -damage source");
			}
		));
	} else if (type == "detailschange" or type == "-formechange") {
		// TODO
		return tv::none;
	} else if (type == "-end") {
		auto const party = party_from_player_id(message.pop());
		// https://github.com/llvm/llvm-project/issues/72828
		auto const temp = split_view(message.pop(), ": "sv);
		auto const effect_type = temp.first;
		auto const other = temp.second;
		return tv::visit(parse_effect_source(effect_type, ""), tv::overload(
			[&](FromSubstitute) -> tv::optional<ParsedMessage> {
				return DestroySubstituteMessage(party);
			},
			[&](FromConfusion) -> tv::optional<ParsedMessage> {
				return ConfusionEndedMessage(party);
			},
			[](FromMiscellaneous) -> tv::optional<ParsedMessage> {
				return tv::none;
			},
			[&](FromMove) -> tv::optional<ParsedMessage> {
				auto const move = from_string<MoveName>(other);
				switch (move) {
					case MoveName::Taunt:
						return TauntEndedMessage(party);
					default:
						return tv::none;
				}
			},
			[](auto) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -end source");
			}
		));
	} else if (type == "-endability") {
		return tv::none;
	} else if (type == "-enditem") {
		auto const party = party_from_player_id(message.pop());
		auto const item = from_string<Item>(message.pop());
		return ItemMessage(party, item);
	} else if (type == "-fail") {
		return tv::none;
	} else if (type == "faint") {
		return tv::none;
	} else if (type == "-fieldactivate") {
		return tv::none;
	} else if (type == "-fieldend") {
		return tv::none;
	} else if (type == "-fieldstart") {
		return tv::none;
	} else if (type == "-heal") {
		auto const parsed = parse_set_hp_message(message);
		auto const party = parsed.party;
		return tv::visit(parsed.source, tv::overload(
			[&](MainEffect) -> tv::optional<ParsedMessage> {
				return HPMessage(party, parsed.status, parsed.hp);
			},
			[&](FromMiscellaneous) -> tv::optional<ParsedMessage> {
				return HPMessage(party, parsed.status, parsed.hp);
			},
			[&](FromMove) -> tv::optional<ParsedMessage> {
				return HPMessage(party, parsed.status, parsed.hp);
			},
			[&](Ability const value) -> tv::optional<ParsedMessage> {
				return HPMessage(party, parsed.status, parsed.hp, value);
			},
			[&](Item const value) -> tv::optional<ParsedMessage> {
				return HPMessage(party, parsed.status, parsed.hp, value);
			},
			[](auto) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Invalid heal source");
			}
		));
	} else if (type == "-hint") {
		return tv::none;
	} else if (type == "-hitcount") {
		return tv::none;
	} else if (type == "-immune") {
		auto const party = party_from_player_id(message.pop());
		auto const source = parse_from_source(message.pop());
		return tv::visit(source, tv::overload(
			[&](MainEffect) -> tv::optional<ParsedMessage> {
				return EffectivenessMessage(
					party,
					EffectivenessMessage::immune
				);
			},
			[&](Item const value) -> tv::optional<ParsedMessage> {
				return ItemMessage(party, value);
			},
			[&](Ability const value) -> tv::optional<ParsedMessage> {
				return AbilityMessage(party, value);
			},
			[](auto) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Invalid immunity source");
			}
		));
	} else if (type == "inactiveoff") {
		// message.remainder() == MESSAGE
		// Timer is off
		return tv::none;
	} else if (type == "-item") {
		auto const party = party_from_player_id(message.pop());
		auto const item = from_string<Item>(message.pop());
		return ItemMessage(party, item);
	} else if (type == "-mega") {
		return tv::none;
	} else if (type == "-message") {
		return tv::none;
	} else if (type == "-miss") {
		// miss information is always sent with a move as well
		return tv::none;
	} else if (type == "move") {
		auto const party = party_from_player_id(message.pop());
		auto const move = from_string<MoveName>(message.pop());
		[[maybe_unused]] auto const target = message.pop();
		auto const miss_or_nothing = message.pop();
		// TODO: validate it's not some other string?
		auto const missed = miss_or_nothing == "[miss]"sv;
		return MoveMessage(
			party,
			move,
			missed
		);
	} else if (type == "-mustrecharge") {
		// After moves like Hyper Beam
		return tv::none;
	} else if (type == "-notarget") {
		// When you use a move, but there is no one to target
		return tv::none;
	} else if (type == "-nothing") {
		return tv::none;
	} else if (type == "-prepare") {
		// From moves like SolarBeam on the charge turn. We already get this
		// information from the move message.
		return tv::none;
	} else if (type == "-resisted") {
		auto const party = party_from_player_id(message.pop());
		return EffectivenessMessage(
			party,
			EffectivenessMessage::not_very
		);
	} else if (type == "-setboost") {
		// Belly Drum
		return tv::none;
	} else if (type == "-sethp") {
		auto const parsed = parse_set_hp_message(message);
		// TODO: verify the source isn't anything meaningful
		return HPMessage(parsed.party, parsed.status, parsed.hp);
	} else if (type == "-sideend") {
		auto const party = party_from_side_id(message.pop());
		auto const condition = parse_condition(message.pop());
		if (condition) {
			return ScreenEndMessage(
				party,
				*condition
			);
		} else {
			return tv::none;
		}
	} else if (type == "-sidestart") {
		return tv::none;
	} else if (type == "-supereffective") {
		auto const party = party_from_player_id(message.pop());
		return EffectivenessMessage(
			party,
			EffectivenessMessage::super
		);
	} else if (type == "switch") {
		return parse_switch(message);
	} else if (type == "drag") {
		return PhazeMessage(parse_switch(message));
	} else if (type == "replace") {
		// Illusion ended
		return tv::none;
	} else if (type == "-singleturn") {
		// Received for things like Protect that last the rest of the turn
		return tv::none;
	} else if (type == "-start") {
		auto const party = party_from_player_id(message.pop());
		auto const first_part_of_source = message.pop();
		auto const source = [&] {
			if (first_part_of_source == "typechange") {
				// TODO: return this information for Hidden Power
				[[maybe_unused]] auto const changed_type = message.pop();
				return parse_from_source(message.pop());
			} else {
				auto const [source_type, string_source] = split_view(first_part_of_source, ": "sv);
				return parse_effect_source(source_type, string_source);
			}
		}();
		return tv::visit(source, tv::overload(
			[&](FromConfusion) -> tv::optional<ParsedMessage> {
				auto const how = message.pop();
				if (how == "[fatigue]") {
					return RampageEndMessage(party);
				} else {
					return StartConfusionMessage(party);
				}
			},
			[&](Item const value) -> tv::optional<ParsedMessage> {
				return ItemMessage(party, value);
			},
			[&](Ability const value) -> tv::optional<ParsedMessage> {
				return AbilityMessage(party, value);
			},
			[](MainEffect) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -start source MainEffect");
			},
			[](FromRecoil) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Unexpected -start source FromRecoil");
			},
			[](auto) -> tv::optional<ParsedMessage> {
				return tv::none;
			}
		));
	} else if (type == "-status") {
		auto const party = party_from_player_id(message.pop());
		auto const status = parse_status(message.pop());
		auto const source = parse_from_source(message.pop());
		return tv::visit(source, tv::overload(
			[&](FromMove) -> tv::optional<ParsedMessage> {
				return MoveStatus(party, status);
			},
			[&](Ability const ability) -> tv::optional<ParsedMessage> {
				check_ability_can_status(ability, status);
				return AbilityStatusMessage(other(party), ability, status);
			},
			[&](Item const item) -> tv::optional<ParsedMessage> {
				return ItemMessage(party, item);
			},
			[&](MainEffect) -> tv::optional<ParsedMessage> {
				return MoveStatus(party, status);
			},
			[](FromEntryHazards) -> tv::optional<ParsedMessage> {
				return tv::none;
			},
			[](FromRecoil) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Recoil cannot cause another status");
			},
			[](FromConfusion) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Confusion cannot cause another status");
			},
			[](FromSubstitute) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Substitute cannot cause another status");
			},
			[](FromMiscellaneous) -> tv::optional<ParsedMessage> {
				throw std::runtime_error("Invalid status source");
			}
		));
	} else if (type == "swap") {
		return tv::none;
	} else if (type == "t:") {
		return tv::none;
	} else if (type == "-transform") {
		return tv::none;
	} else if (type == "turn") {
		return TurnMessage(bounded::to_integer<TurnCount>(message.pop()));
	} else if (type == "-unboost") {
		return tv::none;
	} else if (type == "upkeep") {
		return EndOfTurnMessage();
	} else if (type == "-weather") {
		auto const weather = from_string<Weather>(message.pop());
		auto const source = parse_from_source(message.pop());
		return tv::visit(source, tv::overload(
			[&](Ability const ability) -> tv::optional<ParsedMessage> {
				[[maybe_unused]] auto const of = message.pop(' ');
				auto const party = party_from_player_id(message.pop());
				return AbilityMessage(party, ability);
			},
			[&](auto) -> tv::optional<ParsedMessage> {
				if (weather == Weather::clear) {
					return WeatherMessage(weather);
				}
				auto const maybe_upkeep = message.pop();
				if (maybe_upkeep == "[upkeep]"sv) {
					return WeatherMessage(weather);
				}
				return tv::none;
			}
		));
	} else if (type == "tie" or type == "win") {
		return BattleFinishedMessage();
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Received battle progress message of unknown type: "sv,
			type,
			": "sv,
			message.remainder()
		));
	}
}

// TODO: Max size?
export using EventBlock = containers::vector<ParsedMessage, 1000_bi>;
export constexpr auto make_event_block(MessageBlock messages) -> EventBlock {
	return EventBlock(
		containers::remove_none(containers::transform(
			messages,
			parse_message
		))
	);
}

} // namespace technicalmachine::ps
