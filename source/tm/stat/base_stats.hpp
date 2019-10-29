// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/pokemon/species_forward.hpp>
#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct BaseStats {
	explicit BaseStats(Generation generation, Species species);

	auto hp() const { return m_hp; }
	auto atk() const { return m_atk; }
	auto def() const { return m_def; }
	auto spa() const { return m_spa; }
	auto spd() const { return m_spd; }
	auto spe() const { return m_spe; }

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
