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

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct DamageBlocker {
	constexpr auto is_enduring() const {
		return m_category == Category::endure;
	}
	constexpr auto endure() {
		m_category = Category::endure;
	}

	constexpr auto is_protecting() const {
		return m_category == Category::protect;
	}
	constexpr auto protect() {
		m_category = Category::protect;
	}
	constexpr auto break_protect() {
		if (m_category == Category::protect) {
			m_category = Category::none;
		}
	}
private:
	enum class Category { none, protect, endure };
	Category m_category = Category::none;
};

}	// namespace technicalmachine
