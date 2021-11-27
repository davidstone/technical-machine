// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/move/category.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/priority.hpp>

#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species.hpp>

#include <tm/type/type.hpp>

#include <tm/ability.hpp>
#include <tm/any_team.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <operators/arrow.hpp>

#include <utility>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto is_boosted_by_deep_sea_scale(Species const species) {
	return species == Species::Clamperl;
}

constexpr auto is_boosted_by_deep_sea_tooth(Species const species) {
	return species == Species::Clamperl;
}

constexpr auto is_boosted_by_light_ball(Species const species) {
	return species == Species::Pikachu;
}

constexpr auto is_boosted_by_metal_powder(Species const species) {
	return species == Species::Ditto;
}

constexpr auto is_boosted_by_quick_powder(Species const species) {
	return species == Species::Ditto;
}

constexpr auto is_boosted_by_soul_dew(Species const species) {
	switch (species) {
		case Species::Latias:
		case Species::Latios:
			return true;
		default:
			return false;
	}
}

constexpr auto is_boosted_by_thick_club(Species const species) {
	switch (species) {
		case Species::Cubone:
		case Species::Marowak:
			return true;
		default:
			return false;
	}
}

constexpr bool is_boosted_by_guts(Generation const generation, Statuses const status) {
	switch (status) {
		case Statuses::clear:
			return false;
		case Statuses::freeze:
			return generation <= Generation::four;
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::toxic:
		case Statuses::sleep:
		case Statuses::rest:
			return true;
	}
}

// TODO: Reduce duplication here vs. in power
template<any_active_pokemon PokemonType>
auto pinch_ability_activates(Type const ability_type, PokemonType const pokemon, Type const move_type) {
	return generation_from<PokemonType> >= Generation::five and move_type == ability_type and hp_ratio(pokemon) <= rational(1_bi, 3_bi);
}

bool boosts_special_attack(any_active_pokemon auto const pokemon, Type const move_type, Ability const other_ability, Weather const weather) {
	auto const ability = pokemon.ability();
	auto pinch_ability = [&](Type const type) {
		return pinch_ability_activates(type, pokemon, move_type);
	};
	switch (ability) {
		case Ability::Solar_Power: return weather.sun(weather_is_blocked_by_ability(ability, other_ability));
		case Ability::Blaze: return pinch_ability(Type::Fire);
		case Ability::Overgrow: return pinch_ability(Type::Grass);
		case Ability::Swarm: return pinch_ability(Type::Bug);
		case Ability::Torrent: return pinch_ability(Type::Water);
		default: return false;
	}
}

constexpr bool boosts_special_defense(Ability const ability, Ability const other_ability, Weather const weather) {
	return ability == Ability::Flower_Gift and weather.sun(weather_is_blocked_by_ability(ability, other_ability));
}

template<any_active_pokemon PokemonType>
auto attack_ability_modifier(PokemonType const pokemon, Type const move_type, Ability const other_ability, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&]() -> bounded::integer<1, 4> {
		auto const ability = pokemon.ability();
		auto pinch_ability = [&](Type const type) {
			return BOUNDED_CONDITIONAL(pinch_ability_activates(type, pokemon, move_type), 3_bi, denominator);
		};
		switch (ability) {
			case Ability::Blaze: return pinch_ability(Type::Fire);
			case Ability::Overgrow: return pinch_ability(Type::Grass);
			case Ability::Swarm: return pinch_ability(Type::Bug);
			case Ability::Torrent: return pinch_ability(Type::Water);
			case Ability::Flower_Gift: return BOUNDED_CONDITIONAL(
				weather.sun(weather_is_blocked_by_ability(ability, other_ability)),
				3_bi,
				denominator
			);
			case Ability::Guts: return BOUNDED_CONDITIONAL(
				is_boosted_by_guts(generation_from<PokemonType>, pokemon.status().name()),
				3_bi,
				denominator
			);
			case Ability::Hustle: return 3_bi;
			case Ability::Huge_Power:
			case Ability::Pure_Power:
				return denominator * 2_bi;
			case Ability::Slow_Start: return BOUNDED_CONDITIONAL(
				pokemon.slow_start_is_active(),
				1_bi,
				denominator
			);
			default: return denominator;
		}
	}();
	return rational(numerator, denominator);
}

auto defense_ability_modifier(any_active_pokemon auto const pokemon) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_defense(pokemon.ability(), pokemon.status()),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

auto special_attack_ability_modifier(any_active_pokemon auto const pokemon, Type const move_type, Ability const other_ability, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_special_attack(pokemon, move_type, other_ability, weather),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

auto special_defense_ability_modifier(any_active_pokemon auto const pokemon, Ability const other_ability, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = BOUNDED_CONDITIONAL(
		boosts_special_defense(pokemon.ability(), other_ability, weather),
		denominator * 3_bi / 2_bi,
		denominator
	);
	return rational(numerator, denominator);
}

auto speed_ability_modifier(any_active_pokemon auto const pokemon, Ability const other_ability, Weather const weather) {
	constexpr auto denominator = 2_bi;
	auto const numerator = [&]() -> bounded::integer<1, 4> {
		auto const ability = pokemon.ability();
		switch (ability) {
			case Ability::Chlorophyll: return BOUNDED_CONDITIONAL(
				weather.sun(weather_is_blocked_by_ability(ability, other_ability)),
				denominator * 2_bi,
				denominator
			);
			case Ability::Swift_Swim: return BOUNDED_CONDITIONAL(
				weather.rain(weather_is_blocked_by_ability(ability, other_ability)),
				denominator * 2_bi,
				denominator
			);
			case Ability::Unburden: return BOUNDED_CONDITIONAL(
				pokemon.is_unburdened(),
				denominator * 2_bi,
				denominator
			);
			case Ability::Quick_Feet: return BOUNDED_CONDITIONAL(
				!is_clear(pokemon.status()),
				denominator * 3_bi / 2_bi,
				denominator
			);
			case Ability::Slow_Start: return BOUNDED_CONDITIONAL(
				pokemon.slow_start_is_active(),
				denominator * 1_bi / 2_bi,
				denominator
			);
			default: return denominator;
		}
	}();
	return rational(numerator, denominator);
}

template<SplitSpecialRegularStat stat>
auto offensive_ability_modifier(any_active_pokemon auto const pokemon, Type const move_type, Ability const other_ability, Weather const weather) {
	if constexpr (stat == SplitSpecialRegularStat::atk) {
		return attack_ability_modifier(pokemon, move_type, other_ability, weather);
	} else {
		static_assert(stat == SplitSpecialRegularStat::spa);
		return special_attack_ability_modifier(pokemon, move_type, other_ability, weather);
	}
}

template<SplitSpecialRegularStat stat, any_active_pokemon PokemonType>
auto item_modifier(PokemonType const pokemon, Weather const weather) {
	constexpr auto generation = generation_from<PokemonType>;
	constexpr auto denominator = 2_bi;
	auto const species [[maybe_unused]] = pokemon.species();
	auto const item = pokemon.item(weather);
	auto const numerator = [&]{
		if constexpr (stat == SplitSpecialRegularStat::atk) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
				case Item::Choice_Band:
					return 3_bi;
				case Item::Light_Ball:
					if (generation <= Generation::three) {
						return denominator;
					}
					return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(species), 2_bi * denominator, denominator);
				case Item::Thick_Club:
					return BOUNDED_CONDITIONAL(is_boosted_by_thick_club(species), 2_bi * denominator, denominator);
				default:
					return denominator;
				}
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::def) {
			auto const boosted = item == Item::Metal_Powder and is_boosted_by_metal_powder(species);
			return [&]() -> bounded::integer<2, 4> {
				// TODO: Boost in Generation 3+ only if not transformed
				if (!boosted) {
					return denominator;
				}
				return BOUNDED_CONDITIONAL(generation <= Generation::two, 3_bi, 4_bi);
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::spa) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
				case Item::Soul_Dew:
					return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(species), 3_bi, denominator);
				case Item::Choice_Specs:
					return 3_bi;
				case Item::Deep_Sea_Tooth:
					return BOUNDED_CONDITIONAL(is_boosted_by_deep_sea_tooth(species), 2_bi * denominator, denominator);
				case Item::Light_Ball:
					return BOUNDED_CONDITIONAL(is_boosted_by_light_ball(species), 2_bi * denominator, denominator);
				default:
					return denominator;
				}
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::spd) {
			return [&]() -> bounded::integer<2, 4> {
				switch (item) {
				case Item::Deep_Sea_Scale:
					return BOUNDED_CONDITIONAL(is_boosted_by_deep_sea_scale(species), 2_bi * denominator, denominator);
				case Item::Metal_Powder:
					return BOUNDED_CONDITIONAL(generation <= Generation::two and is_boosted_by_metal_powder(species), 3_bi, denominator);
				case Item::Soul_Dew:
					return BOUNDED_CONDITIONAL(is_boosted_by_soul_dew(species), 3_bi, denominator);
				default:
					return denominator;
				}
			}();
		} else if constexpr (stat == SplitSpecialRegularStat::spe) {
			return [&]() -> bounded::integer<1, 4> {
				switch (item) {
				case Item::Quick_Powder:
					return BOUNDED_CONDITIONAL(is_boosted_by_quick_powder(species), 2_bi * denominator, denominator);
				case Item::Choice_Scarf:
					return 3_bi;
				case Item::Iron_Ball:
				case Item::Macho_Brace:
				case Item::Power_Anklet:
				case Item::Power_Band:
				case Item::Power_Belt:
				case Item::Power_Bracer:
				case Item::Power_Lens:
				case Item::Power_Weight:
					return 1_bi;
				default:
					return denominator;
				}
			}();
		}
	}();
	return rational(numerator, denominator);
}

constexpr auto applies_to_physical(SplitSpecialRegularStat const stat) {
	switch (stat) {
	case SplitSpecialRegularStat::atk:
	case SplitSpecialRegularStat::def:
		return true;
	default:
		return false;
	}
}

constexpr auto other_physical_stat(SplitSpecialRegularStat const stat) {
	switch (stat) {
		case SplitSpecialRegularStat::atk: return SplitSpecialRegularStat::def;
		case SplitSpecialRegularStat::def: return SplitSpecialRegularStat::atk;
		default: bounded::unreachable();
	}
}

auto determine_initial_stat(SplitSpecialRegularStat const name, any_active_pokemon auto const pokemon) {
	return !applies_to_physical(name) or !pokemon.power_trick_is_active() ?
		pokemon.stat(name) :
		pokemon.stat(other_physical_stat(name));
}

template<SplitSpecialRegularStat stat>
auto calculate_common_offensive_stat(any_active_pokemon auto const pokemon, Type const move_type, Ability const other_ability, Weather const weather, bool const critical_hit) {
	auto const attack =
		determine_initial_stat(stat, pokemon) *
		modifier<BoostableStat(stat)>(pokemon.stages(), critical_hit) *
		offensive_ability_modifier<stat>(pokemon, move_type, other_ability, weather) *
		item_modifier<stat>(pokemon, weather);
	
	return bounded::max(attack, 1_bi);
}

auto calculate_attack(any_active_pokemon auto const attacker, Type const move_type, Ability const other_ability, Weather const weather, bool const critical_hit = false) {
	// Cast here because it looks as though the strongest attacker would hold a
	// Light Ball, but because of the restriction on the attacker being Pikachu,
	// it is better to use a Power Trick Shuckle with a Choice Band.
	return bounded::assume_in_range(
		calculate_common_offensive_stat<SplitSpecialRegularStat::atk>(attacker, move_type, other_ability, weather, critical_hit),
		1_bi,
		7368_bi
	);
}


auto calculate_special_attack(any_active_pokemon auto const attacker, Type const move_type, Ability const other_ability, Weather const weather, bool const critical_hit = false) {
	// see above comment about Light Ball, except the strongest Special Attack
	// Pokemon is actually a Choice Specs Deoxys-Attack.
	return bounded::assume_in_range(
		calculate_common_offensive_stat<SplitSpecialRegularStat::spa>(attacker, move_type, other_ability, weather, critical_hit),
		1_bi,
		4536_bi
	);
}


constexpr auto is_self_KO(Moves const move) {
	switch (move) {
		case Moves::Explosion:
		case Moves::Self_Destruct:
			return true;
		default:
			return false;
	}
}

auto calculate_defense(any_active_pokemon auto const defender, Moves const move, Weather const weather, bool const critical_hit = false) {
	constexpr auto stat = SplitSpecialRegularStat::def;
	auto const defense =
		determine_initial_stat(stat, defender) *
		modifier<BoostableStat(stat)>(defender.stages(), critical_hit) *
		defense_ability_modifier(defender) *
		item_modifier<stat>(defender, weather);
	
	// Cast here because it looks as though the strongest defender would hold
	// Metal Powder, but because of the restriction on the attacker being Ditto,
	// it is better to use a Shuckle with no boosting item available.
	return bounded::assume_in_range(
		bounded::max(BOUNDED_CONDITIONAL(is_self_KO(move), defense / 2_bi, defense), 1_bi),
		1_bi,
		3684_bi
	);
}


template<any_active_pokemon PokemonType>
auto special_defense_sandstorm_boost(PokemonType const defender, Ability const attacker_ability, Weather const weather) {
	auto const blocks_weather = weather_is_blocked_by_ability(defender.ability(), attacker_ability);
	return rational(BOUNDED_CONDITIONAL(is_type(defender, Type::Rock) and weather.sand(blocks_weather) and generation_from<PokemonType> >= Generation::four, 3_bi, 2_bi), 2_bi);
}

auto calculate_special_defense(any_active_pokemon auto const defender, Ability const attacker_ability, Weather const weather, bool const critical_hit = false) {
	constexpr auto stat = SplitSpecialRegularStat::spd;
	auto const defense =
		defender.stat(stat) *	
		modifier<BoostableStat(stat)>(defender.stages(), critical_hit) *
		special_defense_ability_modifier(defender, attacker_ability, weather) *
		item_modifier<stat>(defender, weather) *
		special_defense_sandstorm_boost(defender, attacker_ability, weather);
	
	// Cast here because it looks as though the strongest defender would hold
	// Deep Sea Scale, but because of the restriction on the defender being
	// Clamperl, it is better to use a Shuckle with no boosting item available.
	// This also gives more Special Defense than Latias with Soul Dew. It also
	// looks like the best ability would be Flower Gift in the Sun, but this is
	// just as good as Sandstorm's Special Defense boost.
	return bounded::assume_in_range(
		bounded::max(defense, 1_bi),
		1_bi,
		3684_bi
	);
}


auto calculate_speed(any_team auto const & team, Ability const other_ability, Weather const weather) {
	constexpr auto stat = SplitSpecialRegularStat::spe;
	auto const & pokemon = team.pokemon();
	auto const paralysis_divisor = BOUNDED_CONDITIONAL(lowers_speed(pokemon.status(), pokemon.ability()), 4_bi, 1_bi);
	auto const tailwind_multiplier = BOUNDED_CONDITIONAL(team.tailwind(), 2_bi, 1_bi);
	auto const speed =
		pokemon.stat(stat) *
		modifier<BoostableStat(stat)>(pokemon.stages()) *
		speed_ability_modifier(pokemon, other_ability, weather) *
		item_modifier<stat>(pokemon, weather) /
		paralysis_divisor *
		tailwind_multiplier;

	// Cast here because it looks as though the fastest Pokemon would hold Quick
	// Powder, but because of the restriction on the Pokemon being Ditto, it is
	// better to use a Deoxys-Speed with Choice Scarf.
	return bounded::assume_in_range(
		bounded::max(speed, 1_bi),
		1_bi,
		12096_bi
	);
}


template<Generation generation>
struct Faster : operators::arrow<Faster<generation>> {
	Faster(Team<generation> const & team1, Team<generation> const & team2, Weather const weather) {
		auto result = before_trick_room(team1, team2, weather);
		if (!result) {
			return;
		}
		bounded::insert(m_teams, weather.trick_room() ? pair(result->second, result->first) : pair(result->first, result->second));
	}

	constexpr explicit operator bool() const {
		return static_cast<bool>(m_teams);
	}
	constexpr auto const & operator*() const {
		return *m_teams;
	}
private:
	using pair = std::pair<Team<generation> const &, Team<generation> const &>;

	Faster() = default;
	constexpr Faster(Team<generation> const & faster, Team<generation> const & slower):
		m_teams(pair(faster, slower))
	{
	}
	static auto before_trick_room(Team<generation> const & team1, Team<generation> const & team2, Weather const weather) -> Faster {
		auto const speed1 = calculate_speed(team1, team2.pokemon().ability(), weather);
		auto const speed2 = calculate_speed(team2, team1.pokemon().ability(), weather);

		return
			speed1 > speed2 ? Faster(team1, team2) :
			speed1 < speed2 ? Faster(team2, team1) :
			Faster();
	}


	bounded::optional<pair> m_teams;
};

template<Generation generation>
struct Order : operators::arrow<Order<generation>> {
	Order(Team<generation> const & team1, Moves const move1, Team<generation> const & team2, Moves const move2, Weather const weather):
		Order([&]{
			auto const priority1 = Priority(generation, move1);
			auto const priority2 = Priority(generation, move2);
			
			auto const lhs = Element{team1, move1};
			auto const rhs = Element{team2, move2};
			
			auto const lhs_first = Order(lhs, rhs);
			auto const rhs_first = Order(rhs, lhs);

			if (priority1 > priority2) {
				return lhs_first;
			} else if (priority1 < priority2) {
				return rhs_first;
			} else if (auto const ordered = Faster(team1, team2, weather)) {
				return (std::addressof(ordered->first) == std::addressof(team1)) ? lhs_first : rhs_first;
			} else {
				return Order();
			}
		}())
	{
	}


	constexpr explicit operator bool() const {
		return static_cast<bool>(m_elements);
	}
	constexpr auto const & operator*() const {
		return *m_elements;
	}
private:
	struct Element {
		Team<generation> const & team;
		Moves move;
	};
	using pair = std::pair<Element, Element>;

	Order() = default;
	constexpr Order(Element const lhs, Element const rhs):
		m_elements(pair(lhs, rhs))
	{
	}

	bounded::optional<pair> m_elements;
};

}	// namespace technicalmachine
