export module tm.move.recharging;

namespace technicalmachine {

export struct Recharging {
	friend auto operator==(Recharging, Recharging) -> bool = default;
};

} // namespace technicalmachine