export module tm.clients.ps.end_of_turn_state;

import tm.clients.party;

import tm.status.status_name;

import tm.end_of_turn_flags;
import tm.visible_hp;
import tm.weather;

import tv;

namespace technicalmachine::ps {

export struct EndOfTurnState {
	struct Individual {
		tv::optional<VisibleHP> hp;
		tv::optional<StatusName> status;
		EndOfTurnFlags flags = EndOfTurnFlags(false, false, false);
	};
	tv::optional<Party> first_party;
	Weather weather = Weather::clear;
	Individual first;
	Individual last;
};

} // namespace technicalmachine::ps
