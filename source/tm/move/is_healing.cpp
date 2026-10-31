export module tm.move.is_healing;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto is_healing(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Heal_Order:
		case MoveName::Heal_Pulse:
		case MoveName::Jungle_Healing:
		case MoveName::Life_Dew:
		case MoveName::Milk_Drink:
		case MoveName::Moonlight:
		case MoveName::Morning_Sun:
		// TODO: Pollen_Puff?
		case MoveName::Purify:
		case MoveName::Recover:
		case MoveName::Rest:
		case MoveName::Roost:
		case MoveName::Shore_Up:
		case MoveName::Slack_Off:
		case MoveName::Soft_Boiled:
		case MoveName::Swallow:
		case MoveName::Synthesis:
		case MoveName::Wish:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
