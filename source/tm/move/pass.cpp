export module tm.move.pass;

namespace technicalmachine {

export struct Pass {
	friend constexpr auto operator==(Pass, Pass) -> bool = default;
};

export constexpr auto pass = Pass();

} // namespace technicalmachine
