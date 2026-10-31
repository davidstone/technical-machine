export module tm.move.move_cures_target_status;

import tm.move.move_name;

import tm.status.status_name;

import tm.type.move_type;
import tm.type.type;

import tm.generation;

import tv;

namespace technicalmachine {

export constexpr auto move_cures_target_status(Generation const generation, MoveName const move_name, tv::optional<Type> const hidden_power_type, StatusName const status) -> bool {
	switch (status) {
		case StatusName::freeze:
			return
				move_type(generation, move_name, hidden_power_type) == Type::Fire or
				(generation == Generation::two and move_name == MoveName::Tri_Attack);
		case StatusName::paralysis:
			return move_name == MoveName::Smelling_Salts;
		case StatusName::rest:
		case StatusName::sleep:
			return move_name == MoveName::Wake_Up_Slap;
		case StatusName::clear:
		case StatusName::burn:
		case StatusName::poison:
		case StatusName::toxic:
			return false;
	}
}

} // namespace technicalmachine
