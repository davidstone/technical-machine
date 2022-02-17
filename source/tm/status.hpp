// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/type/type.hpp>
#include <tm/ability.hpp>
#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <cstdint>

namespace technicalmachine {

struct Weather;

enum class Statuses : uint8_t {
	clear,
	burn,
	freeze,
	paralysis,
	poison,
	toxic,
	sleep,
	rest
};

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::Statuses> = technicalmachine::Statuses();

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::Statuses> = technicalmachine::Statuses::rest;

namespace technicalmachine {

struct Status {
private:
	struct Clear {
		friend auto operator==(Clear, Clear) -> bool = default;
		friend constexpr auto compress(Clear) {
			return 0_bi;
		}
	};
	struct Burn {
		friend auto operator==(Burn, Burn) -> bool = default;
		friend constexpr auto compress(Burn) {
			return 0_bi;
		}
	};
	struct Freeze {
		friend auto operator==(Freeze, Freeze) -> bool = default;
		friend constexpr auto compress(Freeze) {
			return 0_bi;
		}
	};
	struct Paralysis {
		friend auto operator==(Paralysis, Paralysis) -> bool = default;
		friend constexpr auto compress(Paralysis) {
			return 0_bi;
		}
	};
	struct Poison {
		friend auto operator==(Poison, Poison) -> bool = default;
		friend constexpr auto compress(Poison) {
			return 0_bi;
		}
	};
	struct Toxic {
		friend auto operator==(Toxic, Toxic) -> bool = default;
		friend constexpr auto compress(Toxic) {
			return 0_bi;
		}
	};
	struct Sleep {
		bounded::integer<0, 4> turns_slept = 0_bi;
		friend auto operator==(Sleep, Sleep) -> bool = default;
		friend constexpr auto compress(Sleep const value) {
			return compress(value.turns_slept);
		}
	};
	struct Rest {
		bounded::integer<0, 2> turns_slept = 0_bi;
		friend auto operator==(Rest, Rest) -> bool = default;
		friend constexpr auto compress(Rest const value) {
			return compress(value.turns_slept);
		}
	};

public:
	constexpr Status() = default;
	explicit Status(Statuses const status);

	constexpr auto name() const {
		return static_cast<Statuses>(m_state.index());
	}

	Status & operator=(Statuses const status) & {
		*this = Status(status);
		return *this;
	}

	auto advance_from_move(Ability ability, bool clear) & -> void;

	// If the current status is sleep or rest, returns the probability the
	// status can change from sleeping to awake on this move. If the current
	// status is freeze, returns the probability of thawing. Returns 0.0 if the
	// Pokemon is not asleep or frozen or if, due to the sleep counter, they
	// will definitely not awaken.
	auto probability_of_clearing(Generation, Ability) const -> double;

	friend auto operator==(Status, Status) -> bool = default;
	friend constexpr auto compress(Status const value) {
		return compress(value.m_state);
	}

private:
	using State = bounded::variant<
		Clear,
		Burn,
		Freeze,
		Paralysis,
		Poison,
		Toxic,
		Sleep,
		Rest
	>;
	State m_state{Clear{}};
};


constexpr auto is_clear(Status const status) {
	return status.name() == Statuses::clear;
}
constexpr auto is_frozen(Status const status) {
	return status.name() == Statuses::freeze;
}
constexpr auto is_sleeping(Status const status) {
	switch (status.name()) {
		case Statuses::rest:
		case Statuses::sleep:
			return true;
		default:
			return false;
	}
}
constexpr auto weakens_physical_attacks(Status const status) {
	return status.name() == Statuses::burn;
}
constexpr auto boosts_smellingsalt(Status const status) {
	return status.name() == Statuses::paralysis;
}

auto lowers_speed(Status status, Ability ability) -> bool;
auto boosts_facade(Status status) -> bool;


bool blocks_status(Ability ability, Ability other_ability, Statuses status, Weather weather);

constexpr auto blocks_status(Type const type, Statuses const status) {
	switch (status) {
		case Statuses::burn:
			return type == Type::Fire;
		case Statuses::freeze:
			return type == Type::Ice;
		case Statuses::poison:
		case Statuses::toxic:
			return type == Type::Poison or type == Type::Steel;
		default:
			return false;
	}
}

constexpr auto clears_status(Item const item, Statuses const status) -> bool {
	switch (item) {
		case Item::Ice_Berry:
		case Item::Rawst_Berry:
			return status == Statuses::burn;
		case Item::Aspear_Berry:
		case Item::Burnt_Berry:
			return status == Statuses::freeze;
		case Item::Cheri_Berry:
		case Item::PRZCureBerry:
			return status == Statuses::paralysis;
		case Item::Chesto_Berry:
		case Item::Mint_Berry:
			return status == Statuses::rest or status == Statuses::sleep;
		case Item::Pecha_Berry:
		case Item::PSNCureBerry:
			return status == Statuses::poison or status == Statuses::toxic;
		case Item::Lum_Berry:
		case Item::MiracleBerry:
			return true;
		default:
			return false;
	}
}

constexpr bool can_clear_status(Ability const ability, Status const status) {
	return ability == Ability::Shed_Skin and !is_clear(status);
}

constexpr bool boosts_defense(Ability const ability, Status const status) {
	return ability == Ability::Marvel_Scale and !is_clear(status);
}

} // namespace technicalmachine
