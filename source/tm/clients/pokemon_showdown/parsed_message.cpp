// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_message;

import tm.clients.ps.switch_message;

import tm.clients.party;
import tm.clients.turn_count;

import tm.move.move_name;

import tm.status.status_name;

import tm.ability;
import tm.item;
import tm.visible_hp;
import tm.weather;

import bounded;
import std_module;
import tv;

namespace technicalmachine::ps {

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

export struct PartiallyTrappedMessage {
	Party party;
	friend auto operator==(PartiallyTrappedMessage, PartiallyTrappedMessage) -> bool = default;
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
	PartiallyTrappedMessage,
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
	WeatherMessage,
	BattleFinishedMessage
>;

static_assert(bounded::equality_comparable<ParsedMessage>);

} // namespace technicalmachine::ps
