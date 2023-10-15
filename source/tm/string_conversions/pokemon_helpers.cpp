// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.pokemon_helpers;

import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export constexpr auto species_hp = " ("sv;
export constexpr auto hp_item = "% HP) @ "sv;
export constexpr auto item_ability = "\n\tAbility: "sv;
export constexpr auto ability_status = "\n\tStatus: "sv;

export constexpr auto status_nature = "\n\tNature: "sv;
export constexpr auto nature_hp_iv = "\n\tIVs: "sv;
export constexpr auto hp_iv_atk_iv = " HP / "sv;
export constexpr auto atk_iv_def_iv = " Atk / "sv;
export constexpr auto def_iv_spa_iv = " Def / "sv;
export constexpr auto spa_iv_spd_iv = " SpA / "sv;
export constexpr auto spd_iv_spe_iv = " SpD / "sv;
export constexpr auto spe_iv_hp_ev = " Spe\n\tEVs: "sv;
export constexpr auto hp_ev_atk_ev = " HP / "sv;
export constexpr auto atk_ev_def_ev = " Atk / "sv;
export constexpr auto def_ev_spa_ev = " Def / "sv;
export constexpr auto spa_ev_spd_ev = " SpA / "sv;
export constexpr auto spd_ev_spe_ev = " SpD / "sv;
export constexpr auto spe_ev_moves = " Spe"sv;

export constexpr auto status_atk_dv = "\n\tDVs: "sv;
export constexpr auto atk_dv_def_dv = " Atk / "sv;
export constexpr auto def_dv_spe_dv = " Def / "sv;
export constexpr auto spe_dv_spc_dv = " Spe / "sv;
export constexpr auto spc_dv_old_hp_ev = " Spc\n\tEVs: "sv;
export constexpr auto old_hp_ev_atk_ev = " HP / "sv;
export constexpr auto old_atk_ev_def_ev = " Atk / "sv;
export constexpr auto old_def_ev_spe_ev = " Def / "sv;
export constexpr auto old_spe_ev_spc_ev = " Spe / "sv;
export constexpr auto old_spc_ev_moves = " Spc"sv;

export constexpr auto moves_separator = "\n\t- "sv;

} // namespace technicalmachine
