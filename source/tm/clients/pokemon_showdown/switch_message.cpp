export module tm.clients.ps.switch_message;

import tm.clients.party;

import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.status.status_name;

import tm.gender;
import tm.visible_hp;

namespace technicalmachine::ps {

export struct SwitchMessage {
	Party party;
	Species species;
	Nickname nickname;
	Level level;
	Gender gender;
	VisibleHP hp;
	StatusName status;

	friend constexpr auto operator==(SwitchMessage, SwitchMessage) -> bool = default;
};

} // namespace technicalmachine::ps
