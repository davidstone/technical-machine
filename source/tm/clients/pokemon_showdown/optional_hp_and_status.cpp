// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.optional_hp_and_status;

import tm.status.status_name;

import tm.visible_hp;

import tv;

namespace technicalmachine::ps {

export struct OptionalHPAndStatus {
	tv::optional<VisibleHP> hp;
	tv::optional<StatusName> status;
};

} // namespace technicalmachine::ps