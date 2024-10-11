// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.string_conversions.stat_names;

import tm.stat.stat_names;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto to_string(SpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SpecialPermanentStat::hp: return "HP";
		case SpecialPermanentStat::atk: return "Atk";
		case SpecialPermanentStat::def: return "Def";
		case SpecialPermanentStat::spe: return "Spe";
		case SpecialPermanentStat::spc: return "Spc";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> SpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, SpecialPermanentStat>>({
			{"atk", SpecialPermanentStat::atk},
			{"def", SpecialPermanentStat::def},
			{"hp", SpecialPermanentStat::hp},
			{"spc", SpecialPermanentStat::spc},
			{"spe", SpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialPermanentStat", str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialPermanentStat const stat) -> std::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP";
		case SplitSpecialPermanentStat::atk: return "Atk";
		case SplitSpecialPermanentStat::def: return "Def";
		case SplitSpecialPermanentStat::spa: return "SpA";
		case SplitSpecialPermanentStat::spd: return "SpD";
		case SplitSpecialPermanentStat::spe: return "Spe";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> SplitSpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, SplitSpecialPermanentStat>>({
			{"atk", SplitSpecialPermanentStat::atk},
			{"def", SplitSpecialPermanentStat::def},
			{"hp", SplitSpecialPermanentStat::hp},
			{"spa", SplitSpecialPermanentStat::spa},
			{"spd", SplitSpecialPermanentStat::spd},
			{"spe", SplitSpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialPermanentStat", str);
	}
	return *result;
}

export constexpr auto to_string(SpecialRegularStat const stat) -> std::string_view {
	switch (stat) {
		case SpecialRegularStat::atk: return "Atk";
		case SpecialRegularStat::def: return "Def";
		case SpecialRegularStat::spe: return "Spe";
		case SpecialRegularStat::spc: return "Spc";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> SpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, SpecialRegularStat>>({
			{"atk", SpecialRegularStat::atk},
			{"def", SpecialRegularStat::def},
			{"spc", SpecialRegularStat::spc},
			{"spe", SpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialRegularStat", str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialRegularStat const stat) -> std::string_view {
	switch (stat) {
		case SplitSpecialRegularStat::atk: return "Atk";
		case SplitSpecialRegularStat::def: return "Def";
		case SplitSpecialRegularStat::spa: return "SpA";
		case SplitSpecialRegularStat::spd: return "SpD";
		case SplitSpecialRegularStat::spe: return "Spe";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> SplitSpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, SplitSpecialRegularStat>>({
			{"atk", SplitSpecialRegularStat::atk},
			{"def", SplitSpecialRegularStat::def},
			{"spa", SplitSpecialRegularStat::spa},
			{"spd", SplitSpecialRegularStat::spd},
			{"spe", SplitSpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialRegularStat", str);
	}
	return *result;
}

} // namespace technicalmachine
