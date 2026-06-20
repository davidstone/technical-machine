// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(SpecialPermanentStat const stat) -> containers::string_view {
	switch (stat) {
		case SpecialPermanentStat::hp: return "HP"_s;
		case SpecialPermanentStat::atk: return "Atk"_s;
		case SpecialPermanentStat::def: return "Def"_s;
		case SpecialPermanentStat::spe: return "Spe"_s;
		case SpecialPermanentStat::spc: return "Spc"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SpecialPermanentStat>>({
			{"atk"_s, SpecialPermanentStat::atk},
			{"def"_s, SpecialPermanentStat::def},
			{"hp"_s, SpecialPermanentStat::hp},
			{"spc"_s, SpecialPermanentStat::spc},
			{"spe"_s, SpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialPermanentStat"_s, str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialPermanentStat const stat) -> containers::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP"_s;
		case SplitSpecialPermanentStat::atk: return "Atk"_s;
		case SplitSpecialPermanentStat::def: return "Def"_s;
		case SplitSpecialPermanentStat::spa: return "SpA"_s;
		case SplitSpecialPermanentStat::spd: return "SpD"_s;
		case SplitSpecialPermanentStat::spe: return "Spe"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SplitSpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SplitSpecialPermanentStat>>({
			{"atk"_s, SplitSpecialPermanentStat::atk},
			{"def"_s, SplitSpecialPermanentStat::def},
			{"hp"_s, SplitSpecialPermanentStat::hp},
			{"spa"_s, SplitSpecialPermanentStat::spa},
			{"spd"_s, SplitSpecialPermanentStat::spd},
			{"spe"_s, SplitSpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialPermanentStat"_s, str);
	}
	return *result;
}

export constexpr auto to_string(SpecialRegularStat const stat) -> containers::string_view {
	switch (stat) {
		case SpecialRegularStat::atk: return "Atk"_s;
		case SpecialRegularStat::def: return "Def"_s;
		case SpecialRegularStat::spe: return "Spe"_s;
		case SpecialRegularStat::spc: return "Spc"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SpecialRegularStat>>({
			{"atk"_s, SpecialRegularStat::atk},
			{"def"_s, SpecialRegularStat::def},
			{"spc"_s, SpecialRegularStat::spc},
			{"spe"_s, SpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialRegularStat"_s, str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialRegularStat const stat) -> containers::string_view {
	switch (stat) {
		case SplitSpecialRegularStat::atk: return "Atk"_s;
		case SplitSpecialRegularStat::def: return "Def"_s;
		case SplitSpecialRegularStat::spa: return "SpA"_s;
		case SplitSpecialRegularStat::spd: return "SpD"_s;
		case SplitSpecialRegularStat::spe: return "Spe"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SplitSpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SplitSpecialRegularStat>>({
			{"atk"_s, SplitSpecialRegularStat::atk},
			{"def"_s, SplitSpecialRegularStat::def},
			{"spa"_s, SplitSpecialRegularStat::spa},
			{"spd"_s, SplitSpecialRegularStat::spd},
			{"spe"_s, SplitSpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialRegularStat"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
