export module tm.clients.battle_continues;

namespace technicalmachine {

export struct BattleContinues {
	friend auto operator==(BattleContinues, BattleContinues) -> bool = default;
};

} // namespace technicalmachine
