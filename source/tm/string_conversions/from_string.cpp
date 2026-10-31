export module tm.string_conversions.from_string;

import containers;
import std_module;

namespace technicalmachine {

export template<typename T>
constexpr auto from_string(containers::string_view str) -> T;

} // namespace technicalmachine
