export module tm.type.type;

import numeric_traits;
import std_module;

namespace technicalmachine {

export enum class Type : std::uint8_t {
	Bug,
	Dark,
	Dragon,
	Electric,
	Fairy,
	Fighting,
	Fire,
	Flying,
	Ghost,
	Grass,
	Ground,
	Ice,
	Normal,
	Poison,
	Psychic,
	Rock,
	Steel,
	Water,
	Typeless
};

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::Type> = technicalmachine::Type();

template<>
constexpr auto numeric_traits::max_value<technicalmachine::Type> = technicalmachine::Type::Typeless;
