// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parse_effect_source;

import tm.string_conversions.ability;
import tm.string_conversions.item;

import tm.ability;
import tm.item;
import tm.split_view;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

export struct MainEffect {};
export struct FromMove {};
export struct FromBide {};
export struct FromConfusion {};
export struct FromDisable {};
export struct FromEntryHazards {};
export struct FromMiscellaneous {};
export struct FromRecoil {};
export struct FromSubstitute {};

export using EffectSource = tv::variant<
	MainEffect,
	Item,
	Ability,
	FromMove,
	FromBide,
	FromConfusion,
	FromDisable,
	FromEntryHazards,
	FromMiscellaneous,
	FromRecoil,
	FromSubstitute
>;

export constexpr auto parse_effect_source(containers::string_view const type, containers::string_view const source) -> EffectSource {
	return
		(type == ""_s) ? EffectSource(MainEffect()) :
		(type == "item"_s) ? EffectSource(from_string<Item>(source)) :
		(type == "ability"_s) ? EffectSource(from_string<Ability>(source)) :
		(type == "move"_s) ? EffectSource(FromMove()) :
		(type == "confusion"_s) ? EffectSource(FromConfusion()) :
		(type == "Bide"_s) ? EffectSource(FromBide()) :
		(type == "Disable"_s) ? EffectSource(FromDisable()) :
		(type == "Recoil"_s) ? EffectSource(FromRecoil()) :
		(type == "Spikes"_s) ? EffectSource(FromEntryHazards()) :
		(type == "Stealth Rock"_s) ? EffectSource(FromEntryHazards()) :
		(type == "Substitute"_s) ? EffectSource(FromSubstitute()) :
		(
			type == "brn"_s or
			type == "psn"_s or
			type == "tox"_s or
			type == "drain"_s or
			type == "perish3"_s or
			type == "perish2"_s or
			type == "perish1"_s or
			type == "perish0"_s or
			type == "trapped"_s or
			type == "Encore"_s or
			type == "Hail"_s or
			type == "Leech Seed"_s or
			type == "Light Screen"_s or
			type == "Mimic"_s or // TODO: implement
			type == "Mist"_s or
			type == "Protect"_s or // Includes Detect
			type == "Reflect"_s or
			type == "Sandstorm"_s
		) ? EffectSource(FromMiscellaneous()) :
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unhandled effect source type: "_s,
			type
		));
}

export constexpr auto parse_from_source(containers::string_view const message) -> EffectSource {
	// [from]
	auto const [bracketed_text_or_nothing, remainder] = split_view(message, ' ');
	auto const [type, source] = split_view(remainder, ": "_s);
	return parse_effect_source(type, source);
}

} // namespace technicalmachine::ps
