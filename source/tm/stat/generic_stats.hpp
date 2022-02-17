// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/stat_names.hpp>

namespace technicalmachine {

template<typename HP, typename Stat = HP>
struct GenericStats {
	constexpr GenericStats(HP const hp_, Stat const atk_, Stat const def_, Stat const spa_, Stat const spd_, Stat const spe_):
		m_hp(hp_),
		m_atk(atk_),
		m_def(def_),
		m_spa(spa_),
		m_spd(spd_),
		m_spe(spe_)
	{
	}

	constexpr auto && hp() const {
		return m_hp;
	}
	constexpr auto && atk() const {
		return m_atk;
	}
	constexpr auto && def() const {
		return m_def;
	}
	constexpr auto && spa() const {
		return m_spa;
	}
	constexpr auto && spd() const {
		return m_spd;
	}
	constexpr auto && spe() const {
		return m_spe;
	}
	constexpr auto && hp() {
		return m_hp;
	}
	constexpr auto && atk() {
		return m_atk;
	}
	constexpr auto && def() {
		return m_def;
	}
	constexpr auto && spa() {
		return m_spa;
	}
	constexpr auto && spd() {
		return m_spd;
	}
	constexpr auto && spe() {
		return m_spe;
	}

	constexpr auto && operator[](auto const index) const {
		return index_stat(*this, index);
	}
	constexpr auto && operator[](auto const index) {
		return index_stat(*this, index);
	}

	friend auto operator==(GenericStats, GenericStats) -> bool = default;

private:
	HP m_hp;
	Stat m_atk;
	Stat m_def;
	Stat m_spa;
	Stat m_spd;
	Stat m_spe;
};

template<typename Stat>
struct GenericDVStats {
	constexpr GenericDVStats(Stat const atk_, Stat const def_, Stat const spe_, Stat const spc_):
		m_atk(atk_),
		m_def(def_),
		m_spe(spe_),
		m_spc(spc_)
	{
	}

	constexpr auto && atk() const {
		return m_atk;
	}
	constexpr auto && def() const {
		return m_def;
	}
	constexpr auto && spe() const {
		return m_spe;
	}
	constexpr auto && spc() const {
		return m_spc;
	}
	constexpr auto && spa() const {
		return m_spc;
	}
	constexpr auto && spd() const {
		return m_spc;
	}
	constexpr auto && atk() {
		return m_atk;
	}
	constexpr auto && def() {
		return m_def;
	}
	constexpr auto && spe() {
		return m_spe;
	}
	constexpr auto && spc() {
		return m_spc;
	}

	constexpr auto && operator[](auto const index) const {
		return index_stat(*this, index);
	}
	constexpr auto && operator[](auto const index) {
		return index_stat(*this, index);
	}

	friend auto operator==(GenericDVStats, GenericDVStats) -> bool = default;

private:
	Stat m_atk;
	Stat m_def;
	Stat m_spe;
	Stat m_spc;
};

} // namespace technicalmachine
