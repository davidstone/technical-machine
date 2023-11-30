// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.possible_dvs_or_ivs;

import tm.pokemon.hidden_power;

import tm.stat.generic_stats;
import tm.stat.iv;

import tm.type.type;

import tm.generation;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<typename T>
using Possible = containers::static_vector<T, numeric_traits::max_value<typename T::value_type> + 1_bi>;

template<typename T>
constexpr auto all_possible = Possible<T>(
	containers::transform(
		containers::inclusive_integer_range(numeric_traits::max_value<typename T::value_type>),
		bounded::construct<T>
	)
);

template<typename Integer>
struct matches_mod_4 {
	explicit constexpr matches_mod_4(Integer value):
		m_value(value)
	{
	}
	constexpr auto operator()(DV const dv) const {
		return dv.value() % 4_bi == m_value;
	}
private:
	[[no_unique_address]] Integer m_value;
};
constexpr auto possible_atk_dvs_type(Type const hidden_power_type) {
	constexpr auto result = [](auto const value) {
		return Possible<DV>(containers::filter(
			all_possible<DV>,
			matches_mod_4(value)
		));
	};
	switch (hidden_power_type) {
		case Type::Fighting: return result(0_bi);
		case Type::Flying: return result(0_bi);
		case Type::Poison: return result(0_bi);
		case Type::Ground: return result(0_bi);
		case Type::Rock: return result(1_bi);
		case Type::Bug: return result(1_bi);
		case Type::Ghost: return result(1_bi);
		case Type::Steel: return result(1_bi);
		case Type::Fire: return result(2_bi);
		case Type::Water: return result(2_bi);
		case Type::Grass: return result(2_bi);
		case Type::Electric: return result(2_bi);
		case Type::Psychic: return result(3_bi);
		case Type::Ice: return result(3_bi);
		case Type::Dragon: return result(3_bi);
		case Type::Dark: return result(3_bi);
		default: std::unreachable();
	}
}
constexpr auto possible_def_dvs_type(Type const hidden_power_type) {
	constexpr auto result = [](auto const value) {
		return Possible<DV>(containers::filter(
			all_possible<DV>,
			matches_mod_4(value)
		));
	};
	switch (hidden_power_type) {
		case Type::Fighting: return result(0_bi);
		case Type::Flying: return result(1_bi);
		case Type::Poison: return result(2_bi);
		case Type::Ground: return result(3_bi);
		case Type::Rock: return result(0_bi);
		case Type::Bug: return result(1_bi);
		case Type::Ghost: return result(2_bi);
		case Type::Steel: return result(3_bi);
		case Type::Fire: return result(0_bi);
		case Type::Water: return result(1_bi);
		case Type::Grass: return result(2_bi);
		case Type::Electric: return result(3_bi);
		case Type::Psychic: return result(0_bi);
		case Type::Ice: return result(1_bi);
		case Type::Dragon: return result(2_bi);
		case Type::Dark: return result(3_bi);
		default: std::unreachable();
	}
}
constexpr auto possible_spe_dvs_type(Type) {
	return all_possible<DV>;
}
constexpr auto possible_spc_dvs_type(Type) {
	return all_possible<DV>;
}

constexpr auto possible_dvs_power_impl(HiddenPowerDVPower const power, auto const contribution) {
	return Possible<DV>(containers::filter(all_possible<DV>, [=](DV const dv) {
		return (((power - 31_bi) * 2_bi / 5_bi) % (contribution * 2_bi) >= contribution) == (dv.value() >= 8_bi);
	}));
}
constexpr auto possible_atk_dvs_power(HiddenPowerDVPower const power) {
	return possible_dvs_power_impl(power, 8_bi);
}
constexpr auto possible_def_dvs_power(HiddenPowerDVPower const power) {
	return possible_dvs_power_impl(power, 4_bi);
}
constexpr auto possible_spe_dvs_power(HiddenPowerDVPower const power) {
	return possible_dvs_power_impl(power, 2_bi);
}
constexpr auto possible_spc_dvs_power(HiddenPowerDVPower const power) {
	return Possible<DV>(containers::filter(all_possible<DV>, [=](DV const dv) {
		auto const remainder = (power - 31_bi) % 5_bi;
		switch (remainder.value()) {
			case 0: return dv.value() < 2_bi;
			case 1: return 2_bi <= dv.value() and dv.value() < 8_bi;
			case 2: std::unreachable();
			case 3: std::unreachable();
			case 4: return 8_bi <= dv.value();
			default: std::unreachable();
		}
	}));
}

export using PossibleDVs = GenericDVStats<Possible<DV>>;

template<Generation generation> requires(generation <= Generation::two)
constexpr auto possible_dvs(tv::optional<HiddenPower<generation>> const hidden_power) -> PossibleDVs {
	constexpr auto no_hidden_power = PossibleDVs{
		all_possible<DV>,
		all_possible<DV>,
		all_possible<DV>,
		all_possible<DV>
	};
	if constexpr (generation == Generation::one) {
		return no_hidden_power;
	} else {
		if (!hidden_power) {
			return no_hidden_power;
		}
		auto calculate = [=](auto with_type, auto with_power) {
			return Possible<DV>(containers::transform(
				containers::set_intersection_pair(with_type(hidden_power->type()), with_power(hidden_power->power())),
				[](auto const pair) { return pair.first; }
			));
		};
		return PossibleDVs{
			calculate(possible_atk_dvs_type, possible_atk_dvs_power),
			calculate(possible_def_dvs_type, possible_def_dvs_power),
			calculate(possible_spe_dvs_type, possible_spe_dvs_power),
			calculate(possible_spc_dvs_type, possible_spc_dvs_power)
		};
	}
}

export using PossibleIVs = GenericStats<Possible<IV>>;

enum class AllowedIVs { any, even, odd };

constexpr auto possible_ivs_type_impl(AllowedIVs const allowed) {
	return Possible<IV>(containers::filter(all_possible<IV>, [=](IV const iv) {
		switch (allowed) {
			case AllowedIVs::any:
				return true;
			case AllowedIVs::even:
				return iv.value() % 2_bi == 0_bi;
			case AllowedIVs::odd:
				return iv.value() % 2_bi == 1_bi;
		}
	}));
}

constexpr auto possible_hp_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

constexpr auto possible_atk_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

constexpr auto possible_def_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

constexpr auto possible_spe_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

constexpr auto possible_spa_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

constexpr auto possible_spd_ivs_type(Type const hidden_power_type) {
	switch (hidden_power_type) {
		case Type::Fighting: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Flying: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Poison: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Ground: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Rock: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Bug: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Ghost: return possible_ivs_type_impl(AllowedIVs::even);
		case Type::Steel: return possible_ivs_type_impl(AllowedIVs::any);
		case Type::Fire: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Water: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Grass: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Electric: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Psychic: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Ice: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Dragon: return possible_ivs_type_impl(AllowedIVs::odd);
		case Type::Dark: return possible_ivs_type_impl(AllowedIVs::odd);
		default: std::unreachable();
	}
}

using PossiblePowerSums = containers::static_vector<bounded::integer<0, 63>, 2_bi>;
constexpr auto possible_power_sums(HiddenPowerIVPower const power) -> PossiblePowerSums {
	constexpr auto divisor = 63_bi;
	auto const min_intermediate = (power - 30_bi) * divisor;
	auto const max_intermediate = min_intermediate + divisor - 1_bi;
	constexpr auto scalar = 40_bi;

	auto const min_possible_value = (min_intermediate + scalar - 1_bi) / scalar;

	constexpr auto max_value = 63;
	auto result = PossiblePowerSums({min_possible_value});

	auto const max_possible_value = min_possible_value + 1_bi;
	if (max_possible_value * scalar <= max_intermediate and max_possible_value <= bounded::constant<max_value>) {
		containers::push_back_into_capacity(result, bounded::assume_in_range<containers::range_value_t<PossiblePowerSums>>(max_possible_value));
	}
	return result;
}

constexpr auto possible_ivs_power_impl(PossiblePowerSums const power_sums, auto const contribution) {
	auto include_low = false;
	auto include_high = false;
	for (auto const sum : power_sums) {
		if ((sum / contribution) % 2_bi == 0_bi) {
			include_low = true;
		} else {
			include_high = true;
		}
	}
	return Possible<IV>(containers::filter(
		all_possible<IV>,
		[=](auto const iv) { return iv.value() % 4_bi <= 1_bi ? include_low : include_high; }
	));
}

constexpr auto possible_hp_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 1_bi);
}

constexpr auto possible_atk_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 2_bi);
}

constexpr auto possible_def_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 4_bi);
}

constexpr auto possible_spe_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 8_bi);
}

constexpr auto possible_spa_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 16_bi);
}

constexpr auto possible_spd_ivs_power(PossiblePowerSums const power_sums) {
	return possible_ivs_power_impl(power_sums, 32_bi);
}

template<Generation generation> requires(Generation::three <= generation and generation <= Generation::five)
constexpr auto possible_ivs_with_powered_hidden_power(tv::optional<HiddenPower<generation>> const hidden_power) -> PossibleIVs {
	if (!hidden_power) {
		return PossibleIVs(
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>
		);
	}
	auto const power_sums = possible_power_sums(hidden_power->power());
	auto const type = hidden_power->type();
	auto calculate = [=](auto with_type, auto with_power) {
		return Possible<IV>(containers::transform(
			containers::set_intersection_pair(with_type(type), with_power(power_sums)),
			[](auto const pair) { return pair.first; }
		));
	};
	return PossibleIVs(
		calculate(possible_hp_ivs_type, possible_hp_ivs_power),
		calculate(possible_atk_ivs_type, possible_atk_ivs_power),
		calculate(possible_def_ivs_type, possible_def_ivs_power),
		calculate(possible_spa_ivs_type, possible_spa_ivs_power),
		calculate(possible_spd_ivs_type, possible_spd_ivs_power),
		calculate(possible_spe_ivs_type, possible_spe_ivs_power)
	);
}

template<Generation generation> requires(Generation::six <= generation)
constexpr auto possible_ivs_with_type_only_hidden_power(tv::optional<HiddenPower<generation>> const hidden_power) -> PossibleIVs {
	if (!hidden_power) {
		return PossibleIVs(
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>,
			all_possible<IV>
		);
	}
	auto const type = hidden_power->type();
	return PossibleIVs(
		possible_hp_ivs_type(type),
		possible_atk_ivs_type(type),
		possible_def_ivs_type(type),
		possible_spa_ivs_type(type),
		possible_spd_ivs_type(type),
		possible_spe_ivs_type(type)
	);
}

export template<Generation generation>
constexpr auto possible_dvs_or_ivs(tv::optional<HiddenPower<generation>> const hidden_power) {
	if constexpr (generation <= Generation::two) {
		return possible_dvs(hidden_power);
	} else if constexpr (generation <= Generation::five) {
		return possible_ivs_with_powered_hidden_power(hidden_power);
	} else {
		return possible_ivs_with_type_only_hidden_power(hidden_power);
	}
}


static_assert(all_possible<DV> == Possible<DV>({
	DV(0_bi),
	DV(1_bi),
	DV(2_bi),
	DV(3_bi),
	DV(4_bi),
	DV(5_bi),
	DV(6_bi),
	DV(7_bi),
	DV(8_bi),
	DV(9_bi),
	DV(10_bi),
	DV(11_bi),
	DV(12_bi),
	DV(13_bi),
	DV(14_bi),
	DV(15_bi),
}));

static_assert(possible_atk_dvs_type(Type::Dark) == Possible<DV>({
	DV(3_bi),
	DV(7_bi),
	DV(11_bi),
	DV(15_bi),
}));

static_assert(possible_atk_dvs_type(Type::Water) == Possible<DV>({
	DV(2_bi),
	DV(6_bi),
	DV(10_bi),
	DV(14_bi),
}));

static_assert(possible_atk_dvs_type(Type::Rock) == Possible<DV>({
	DV(1_bi),
	DV(5_bi),
	DV(9_bi),
	DV(13_bi),
}));

static_assert(possible_atk_dvs_type(Type::Fighting) == Possible<DV>({
	DV(0_bi),
	DV(4_bi),
	DV(8_bi),
	DV(12_bi),
}));

static_assert(possible_def_dvs_type(Type::Dark) == Possible<DV>({
	DV(3_bi),
	DV(7_bi),
	DV(11_bi),
	DV(15_bi),
}));

static_assert(possible_def_dvs_type(Type::Grass) == Possible<DV>({
	DV(2_bi),
	DV(6_bi),
	DV(10_bi),
	DV(14_bi),
}));

static_assert(possible_def_dvs_type(Type::Bug) == Possible<DV>({
	DV(1_bi),
	DV(5_bi),
	DV(9_bi),
	DV(13_bi),
}));

static_assert(possible_def_dvs_type(Type::Fighting) == Possible<DV>({
	DV(0_bi),
	DV(4_bi),
	DV(8_bi),
	DV(12_bi),
}));


static_assert(possible_dvs_or_ivs(tv::optional<HiddenPower<Generation::two>>()) == PossibleDVs(
	all_possible<DV>,
	all_possible<DV>,
	all_possible<DV>,
	all_possible<DV>
));





static_assert(all_possible<IV> == Possible<IV>({
	IV(0_bi),
	IV(1_bi),
	IV(2_bi),
	IV(3_bi),
	IV(4_bi),
	IV(5_bi),
	IV(6_bi),
	IV(7_bi),
	IV(8_bi),
	IV(9_bi),
	IV(10_bi),
	IV(11_bi),
	IV(12_bi),
	IV(13_bi),
	IV(14_bi),
	IV(15_bi),
	IV(16_bi),
	IV(17_bi),
	IV(18_bi),
	IV(19_bi),
	IV(20_bi),
	IV(21_bi),
	IV(22_bi),
	IV(23_bi),
	IV(24_bi),
	IV(25_bi),
	IV(26_bi),
	IV(27_bi),
	IV(28_bi),
	IV(29_bi),
	IV(30_bi),
	IV(31_bi),
}));

static_assert(possible_ivs_type_impl(AllowedIVs::any) == all_possible<IV>);

static_assert(possible_ivs_type_impl(AllowedIVs::even) == Possible<IV>({
	IV(0_bi),
	IV(2_bi),
	IV(4_bi),
	IV(6_bi),
	IV(8_bi),
	IV(10_bi),
	IV(12_bi),
	IV(14_bi),
	IV(16_bi),
	IV(18_bi),
	IV(20_bi),
	IV(22_bi),
	IV(24_bi),
	IV(26_bi),
	IV(28_bi),
	IV(30_bi),
}));

static_assert(possible_ivs_type_impl(AllowedIVs::odd) == Possible<IV>({
	IV(1_bi),
	IV(3_bi),
	IV(5_bi),
	IV(7_bi),
	IV(9_bi),
	IV(11_bi),
	IV(13_bi),
	IV(15_bi),
	IV(17_bi),
	IV(19_bi),
	IV(21_bi),
	IV(23_bi),
	IV(25_bi),
	IV(27_bi),
	IV(29_bi),
	IV(31_bi),
}));

static_assert(possible_hp_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));
static_assert(possible_atk_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));
static_assert(possible_def_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));
static_assert(possible_spa_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));
static_assert(possible_spd_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));
static_assert(possible_spe_ivs_type(Type::Dark) == possible_ivs_type_impl(AllowedIVs::odd));

constexpr auto possible_70_power_ivs = Possible<IV>({
	IV(2_bi),
	IV(3_bi),
	IV(6_bi),
	IV(7_bi),
	IV(10_bi),
	IV(11_bi),
	IV(14_bi),
	IV(15_bi),
	IV(18_bi),
	IV(19_bi),
	IV(22_bi),
	IV(23_bi),
	IV(26_bi),
	IV(27_bi),
	IV(30_bi),
	IV(31_bi),
});

constexpr auto possible_70_power_sums = possible_power_sums(70_bi);

static_assert(possible_hp_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(possible_atk_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(possible_def_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(possible_spa_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(possible_spd_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(possible_spe_ivs_power(possible_70_power_sums) == possible_70_power_ivs);

static_assert(possible_dvs_or_ivs(tv::optional<HiddenPower<Generation::four>>()) == PossibleIVs(
	all_possible<IV>,
	all_possible<IV>,
	all_possible<IV>,
	all_possible<IV>,
	all_possible<IV>,
	all_possible<IV>
));

constexpr auto possible_hp_dark_70_ivs = Possible<IV>({
	IV(3_bi),
	IV(7_bi),
	IV(11_bi),
	IV(15_bi),
	IV(19_bi),
	IV(23_bi),
	IV(27_bi),
	IV(31_bi),
});

static_assert(possible_dvs_or_ivs(tv::optional(HiddenPower<Generation::four>(70_bi, Type::Dark))) == PossibleIVs({
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
}));

constexpr auto hp_ghost_54 = possible_dvs_or_ivs(tv::optional(HiddenPower<Generation::three>(54_bi, Type::Ghost)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(31_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(30_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(30_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));

static_assert(possible_power_sums(64_bi) == PossiblePowerSums({54_bi, 55_bi}));

} // namespace technicalmachine