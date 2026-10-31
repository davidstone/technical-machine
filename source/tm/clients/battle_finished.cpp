export module tm.clients.battle_finished;

namespace technicalmachine {

export struct BattleFinished {
	friend auto operator==(BattleFinished, BattleFinished) -> bool = default;
};

} // namespace technicalmachine
