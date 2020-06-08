// Copyright (C) 2015 David Stone
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

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Stat {
	using base_type = bounded::checked_integer<5, 230>;

	constexpr Stat(base_type const base_, IV const iv_, EV const ev_):
		m_base(base_),
		m_iv(iv_),
		m_ev(ev_)
	{
	}

	constexpr auto base() const {
		return m_base;
	}
	constexpr auto ev() const {
		return m_ev;
	}
	constexpr auto iv() const {
		return m_iv;
	}

	friend auto operator==(Stat const &, Stat const &) -> bool = default;
private:
	base_type m_base;
	IV m_iv;
	EV m_ev;
};

}	// namespace technicalmachine
