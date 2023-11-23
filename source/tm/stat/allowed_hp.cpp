// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.allowed_hp;

import tm.stat.current_hp;

namespace technicalmachine {

export struct AllowedHP {
	constexpr AllowedHP(CurrentHP const min_, CurrentHP const value_, CurrentHP const max_):
		min(min_),
		value(value_),
		max(max_)
	{
	}
	constexpr explicit AllowedHP(CurrentHP const value_):
		AllowedHP(value_, value_, value_)
	{
	}
	CurrentHP min;
	CurrentHP value;
	CurrentHP max;
};

} // namespace technicalmachine
