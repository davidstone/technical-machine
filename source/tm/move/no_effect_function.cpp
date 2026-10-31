export module tm.move.no_effect_function;

namespace technicalmachine {

// Not a lambda because of https://github.com/llvm/llvm-project/issues/59513
struct no_effect_function_t {
	static constexpr auto operator()(auto &, auto &, auto &, auto) -> void {
	}
};
export constexpr auto no_effect_function = no_effect_function_t();

} // namespace technicalmachine
