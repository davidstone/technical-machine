// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

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
using namespace std::string_view_literals;

export struct MainEffect {};
export struct FromMove {};
export struct FromConfusion {};
export struct FromEntryHazards {};
export struct FromMiscellaneous {};
export struct FromRecoil {};
export struct FromSubstitute {};

export using EffectSource = tv::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromEntryHazards, FromMiscellaneous, FromRecoil, FromSubstitute>;

export constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) -> EffectSource {
	return
		(type == "") ? EffectSource(MainEffect()) :
		(type == "item") ? EffectSource(from_string<Item>(source)) :
		(type == "ability") ? EffectSource(from_string<Ability>(source)) :
		(type == "move") ? EffectSource(FromMove()) :
		(type == "confusion") ? EffectSource(FromConfusion()) :
		(type == "Recoil") ? EffectSource(FromRecoil()) :
		(type == "Spikes") ? EffectSource(FromEntryHazards()) :
		(type == "Stealth Rock") ? EffectSource(FromEntryHazards()) :
		(type == "Substitute") ? EffectSource(FromSubstitute()) :
		(
			type == "brn" or
			type == "psn" or
			type == "tox" or
			type == "drain" or
			type == "perish3" or
			type == "perish2" or
			type == "perish1" or
			type == "perish0" or
			type == "trapped" or
			type == "Encore" or
			type == "Hail" or
			type == "Leech Seed" or
			type == "Light Screen" or
			type == "Mimic" or // TODO: implement
			type == "Protect" or // Includes Detect
			type == "Reflect" or
			type == "Sandstorm"
		) ? EffectSource(FromMiscellaneous()) :
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unhandled effect source type: "sv,
			type
		));
}

export constexpr auto parse_from_source(std::string_view const message) -> EffectSource {
	// [from]
	auto const [bracketed_text_or_nothing, remainder] = split_view(message, ' ');
	auto const [type, source] = split_view(remainder, ": "sv);
	return parse_effect_source(type, source);
}

} // namespace technicalmachine::ps
