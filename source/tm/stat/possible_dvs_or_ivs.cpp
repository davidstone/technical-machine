// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/possible_dvs_or_ivs.hpp>

#include <containers/algorithms/maybe_find.hpp>

namespace technicalmachine {

static_assert(detail::all_possible<DV> == detail::Possible<DV>({
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

static_assert(detail::possible_atk_dvs_type(Type::Dark) == detail::Possible<DV>({
	DV(3_bi),
	DV(7_bi),
	DV(11_bi),
	DV(15_bi),
}));

static_assert(detail::possible_atk_dvs_type(Type::Water) == detail::Possible<DV>({
	DV(2_bi),
	DV(6_bi),
	DV(10_bi),
	DV(14_bi),
}));

static_assert(detail::possible_atk_dvs_type(Type::Rock) == detail::Possible<DV>({
	DV(1_bi),
	DV(5_bi),
	DV(9_bi),
	DV(13_bi),
}));

static_assert(detail::possible_atk_dvs_type(Type::Fighting) == detail::Possible<DV>({
	DV(0_bi),
	DV(4_bi),
	DV(8_bi),
	DV(12_bi),
}));

static_assert(detail::possible_def_dvs_type(Type::Dark) == detail::Possible<DV>({
	DV(3_bi),
	DV(7_bi),
	DV(11_bi),
	DV(15_bi),
}));

static_assert(detail::possible_def_dvs_type(Type::Grass) == detail::Possible<DV>({
	DV(2_bi),
	DV(6_bi),
	DV(10_bi),
	DV(14_bi),
}));

static_assert(detail::possible_def_dvs_type(Type::Bug) == detail::Possible<DV>({
	DV(1_bi),
	DV(5_bi),
	DV(9_bi),
	DV(13_bi),
}));

static_assert(detail::possible_def_dvs_type(Type::Fighting) == detail::Possible<DV>({
	DV(0_bi),
	DV(4_bi),
	DV(8_bi),
	DV(12_bi),
}));


static_assert(possible_dvs_or_ivs(bounded::optional<HiddenPower<Generation::two>>()) == detail::PossibleDVs(
	detail::all_possible<DV>,
	detail::all_possible<DV>,
	detail::all_possible<DV>,
	detail::all_possible<DV>
));





static_assert(detail::all_possible<IV> == detail::Possible<IV>({
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

static_assert(detail::possible_ivs_type_impl(detail::AllowedIVs::any) == detail::all_possible<IV>);

static_assert(detail::possible_ivs_type_impl(detail::AllowedIVs::even) == detail::Possible<IV>({
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

static_assert(detail::possible_ivs_type_impl(detail::AllowedIVs::odd) == detail::Possible<IV>({
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

static_assert(detail::possible_hp_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));
static_assert(detail::possible_atk_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));
static_assert(detail::possible_def_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));
static_assert(detail::possible_spa_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));
static_assert(detail::possible_spd_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));
static_assert(detail::possible_spe_ivs_type(Type::Dark) == detail::possible_ivs_type_impl(detail::AllowedIVs::odd));

constexpr auto possible_70_power_ivs = detail::Possible<IV>({
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

constexpr auto possible_70_power_sums = detail::possible_power_sums(70_bi);

static_assert(detail::possible_hp_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(detail::possible_atk_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(detail::possible_def_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(detail::possible_spa_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(detail::possible_spd_ivs_power(possible_70_power_sums) == possible_70_power_ivs);
static_assert(detail::possible_spe_ivs_power(possible_70_power_sums) == possible_70_power_ivs);

static_assert(possible_dvs_or_ivs(bounded::optional<HiddenPower<Generation::four>>()) == detail::PossibleIVs(
	detail::all_possible<IV>,
	detail::all_possible<IV>,
	detail::all_possible<IV>,
	detail::all_possible<IV>,
	detail::all_possible<IV>,
	detail::all_possible<IV>
));

constexpr auto possible_hp_dark_70_ivs = detail::Possible<IV>({
	IV(3_bi),
	IV(7_bi),
	IV(11_bi),
	IV(15_bi),
	IV(19_bi),
	IV(23_bi),
	IV(27_bi),
	IV(31_bi),
});

static_assert(possible_dvs_or_ivs(bounded::optional(HiddenPower<Generation::four>(70_bi, Type::Dark))) == detail::PossibleIVs({
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
	possible_hp_dark_70_ivs,
}));

constexpr auto hp_ghost_54 = possible_dvs_or_ivs(bounded::optional(HiddenPower<Generation::three>(54_bi, Type::Ghost)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(31_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(30_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(30_bi)));
static_assert(containers::maybe_find(hp_ghost_54.hp(), IV(29_bi)));

static_assert(detail::possible_power_sums(64_bi) == detail::PossiblePowerSums({54_bi, 55_bi}));

} // namespace technicalmachine