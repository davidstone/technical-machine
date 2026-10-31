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