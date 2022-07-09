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
#include <tm/status_name.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

namespace technicalmachine {

struct Weather;

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
	explicit Status(StatusName const status);

	constexpr auto name() const {
		return static_cast<StatusName>(m_state.index());
	}

	Status & operator=(StatusName const status) & {
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
	return status.name() == StatusName::clear;
}
constexpr auto is_frozen(Status const status) {
	return status.name() == StatusName::freeze;
}
constexpr auto is_sleeping(Status const status) {
	switch (status.name()) {
		case StatusName::rest:
		case StatusName::sleep:
			return true;
		default:
			return false;
	}
}
constexpr auto weakens_physical_attacks(Status const status) {
	return status.name() == StatusName::burn;
}
constexpr auto boosts_smellingsalt(Status const status) {
	return status.name() == StatusName::paralysis;
}

auto lowers_speed(Status status, Ability ability) -> bool;
auto boosts_facade(Status status) -> bool;


bool blocks_status(Ability ability, Ability other_ability, StatusName status, Weather weather);

constexpr auto blocks_status(Type const type, StatusName const status) {
	switch (status) {
		case StatusName::burn:
			return type == Type::Fire;
		case StatusName::freeze:
			return type == Type::Ice;
		case StatusName::poison:
		case StatusName::toxic:
			return type == Type::Poison or type == Type::Steel;
		default:
			return false;
	}
}

constexpr auto clears_status(Item const item, StatusName const status) -> bool {
	switch (item) {
		case Item::Ice_Berry:
		case Item::Rawst_Berry:
			return status == StatusName::burn;
		case Item::Aspear_Berry:
		case Item::Burnt_Berry:
			return status == StatusName::freeze;
		case Item::Cheri_Berry:
		case Item::PRZCureBerry:
			return status == StatusName::paralysis;
		case Item::Chesto_Berry:
		case Item::Mint_Berry:
			return status == StatusName::rest or status == StatusName::sleep;
		case Item::Pecha_Berry:
		case Item::PSNCureBerry:
			return status == StatusName::poison or status == StatusName::toxic;
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
