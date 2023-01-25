// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.no_effect_function;

namespace technicalmachine {

// Not a lambda because of https://github.com/llvm/llvm-project/issues/59513
struct no_effect_function_t {
	static constexpr auto operator()(auto &, auto &, auto &, auto) -> void {
	}
};
export constexpr auto no_effect_function = no_effect_function_t();

} // namespace technicalmachine
