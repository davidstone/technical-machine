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
		(type == ""_sv) ? EffectSource(MainEffect()) :
		(type == "item"_sv) ? EffectSource(from_string<Item>(source)) :
		(type == "ability"_sv) ? EffectSource(from_string<Ability>(source)) :
		(type == "move"_sv) ? EffectSource(FromMove()) :
		(type == "confusion"_sv) ? EffectSource(FromConfusion()) :
		(type == "Bide"_sv) ? EffectSource(FromBide()) :
		(type == "Disable"_sv) ? EffectSource(FromDisable()) :
		(type == "Recoil"_sv) ? EffectSource(FromRecoil()) :
		(type == "Spikes"_sv) ? EffectSource(FromEntryHazards()) :
		(type == "Stealth Rock"_sv) ? EffectSource(FromEntryHazards()) :
		(type == "Substitute"_sv) ? EffectSource(FromSubstitute()) :
		(
			type == "brn"_sv or
			type == "psn"_sv or
			type == "tox"_sv or
			type == "drain"_sv or
			type == "perish3"_sv or
			type == "perish2"_sv or
			type == "perish1"_sv or
			type == "perish0"_sv or
			type == "trapped"_sv or
			type == "Encore"_sv or
			type == "Hail"_sv or
			type == "Leech Seed"_sv or
			type == "Light Screen"_sv or
			type == "Mimic"_sv or // TODO: implement
			type == "Mist"_sv or
			type == "Protect"_sv or // Includes Detect
			type == "Reflect"_sv or
			type == "Sandstorm"_sv
		) ? EffectSource(FromMiscellaneous()) :
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unhandled effect source type: "_sv,
			type
		));
}

export constexpr auto parse_from_source(containers::string_view const message) -> EffectSource {
	// [from]
	auto const [bracketed_text_or_nothing, remainder] = split_view(message, ' ');
	auto const [type, source] = split_view(remainder, ": "_sv);
	return parse_effect_source(type, source);
}

} // namespace technicalmachine::ps
