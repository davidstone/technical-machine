// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/stat_names.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

enum class Generation : std::uint8_t;

struct BaseStats {
	explicit BaseStats(Generation generation, Species species);

	auto hp() const { return m_hp; }
	auto atk() const { return m_atk; }
	auto def() const { return m_def; }
	auto spa() const { return m_spa; }
	auto spd() const { return m_spd; }
	auto spe() const { return m_spe; }

	using regular_value_type = bounded::integer<5, 230>;

	auto operator[](RegularStat const stat_name) const -> regular_value_type {
		switch (stat_name) {
			case RegularStat::atk: return atk();
			case RegularStat::def: return def();
			case RegularStat::spa: return spa();
			case RegularStat::spd: return spd();
			case RegularStat::spe: return spe();
		}
	}

private:
	using HP = bounded::integer<1, 255>;
	using Atk = bounded::integer<5, 190>;
	using Def = bounded::integer<5, 230>;
	using SpA = bounded::integer<10, 194>;
	using SpD = bounded::integer<20, 230>;
	using Spe = bounded::integer<5, 180>;

	BaseStats(HP hp_, Atk atk_, Def def_, SpA spa_, SpD spd_, Spe spe_):
		m_hp(hp_),
		m_atk(atk_),
		m_def(def_),
		m_spa(spa_),
		m_spd(spd_),
		m_spe(spe_)
	{
	}

	HP m_hp;
	Atk m_atk;
	Def m_def;
	SpA m_spa;
	SpD m_spd;
	Spe m_spe;
};

}	// namespace technicalmachine
