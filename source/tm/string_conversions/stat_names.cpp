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
		case SpecialPermanentStat::hp: return "HP"_sv;
		case SpecialPermanentStat::atk: return "Atk"_sv;
		case SpecialPermanentStat::def: return "Def"_sv;
		case SpecialPermanentStat::spe: return "Spe"_sv;
		case SpecialPermanentStat::spc: return "Spc"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SpecialPermanentStat>>({
			{"atk"_sv, SpecialPermanentStat::atk},
			{"def"_sv, SpecialPermanentStat::def},
			{"hp"_sv, SpecialPermanentStat::hp},
			{"spc"_sv, SpecialPermanentStat::spc},
			{"spe"_sv, SpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialPermanentStat"_sv, str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialPermanentStat const stat) -> containers::string_view {
	switch (stat) {
		case SplitSpecialPermanentStat::hp: return "HP"_sv;
		case SplitSpecialPermanentStat::atk: return "Atk"_sv;
		case SplitSpecialPermanentStat::def: return "Def"_sv;
		case SplitSpecialPermanentStat::spa: return "SpA"_sv;
		case SplitSpecialPermanentStat::spd: return "SpD"_sv;
		case SplitSpecialPermanentStat::spe: return "Spe"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SplitSpecialPermanentStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SplitSpecialPermanentStat>>({
			{"atk"_sv, SplitSpecialPermanentStat::atk},
			{"def"_sv, SplitSpecialPermanentStat::def},
			{"hp"_sv, SplitSpecialPermanentStat::hp},
			{"spa"_sv, SplitSpecialPermanentStat::spa},
			{"spd"_sv, SplitSpecialPermanentStat::spd},
			{"spe"_sv, SplitSpecialPermanentStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialPermanentStat"_sv, str);
	}
	return *result;
}

export constexpr auto to_string(SpecialRegularStat const stat) -> containers::string_view {
	switch (stat) {
		case SpecialRegularStat::atk: return "Atk"_sv;
		case SpecialRegularStat::def: return "Def"_sv;
		case SpecialRegularStat::spe: return "Spe"_sv;
		case SpecialRegularStat::spc: return "Spc"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SpecialRegularStat>>({
			{"atk"_sv, SpecialRegularStat::atk},
			{"def"_sv, SpecialRegularStat::def},
			{"spc"_sv, SpecialRegularStat::spc},
			{"spe"_sv, SpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SpecialRegularStat"_sv, str);
	}
	return *result;
}

export constexpr auto to_string(SplitSpecialRegularStat const stat) -> containers::string_view {
	switch (stat) {
		case SplitSpecialRegularStat::atk: return "Atk"_sv;
		case SplitSpecialRegularStat::def: return "Def"_sv;
		case SplitSpecialRegularStat::spa: return "SpA"_sv;
		case SplitSpecialRegularStat::spd: return "SpD"_sv;
		case SplitSpecialRegularStat::spe: return "Spe"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> SplitSpecialRegularStat {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, SplitSpecialRegularStat>>({
			{"atk"_sv, SplitSpecialRegularStat::atk},
			{"def"_sv, SplitSpecialRegularStat::def},
			{"spa"_sv, SplitSpecialRegularStat::spa},
			{"spd"_sv, SplitSpecialRegularStat::spd},
			{"spe"_sv, SplitSpecialRegularStat::spe},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<3_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("SplitSpecialRegularStat"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
