// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.battle;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.causes_recoil;
import tm.move.do_switch;
import tm.move.future_selection;
import tm.move.irrelevant_action;
import tm.move.known_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_result;
import tm.move.other_action;
import tm.move.switch_;
import tm.move.used_move;

import tm.pokemon.any_pokemon;
import tm.pokemon.faint;
import tm.pokemon.find_required_pokemon_index;
import tm.pokemon.get_hidden_power_type;
import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.status.status_name;

import tm.string_conversions.status_name;

import tm.type.effectiveness;
import tm.type.move_type;

import tm.ability;
import tm.ability_blocks_move;
import tm.activate_ability_on_switch;
import tm.any_team;
import tm.end_of_turn;
import tm.end_of_turn_flags;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.other_team;
import tm.team;
import tm.to_used_move;
import tm.visible_damage_to_actual_damage;
import tm.visible_hp;
import tm.visible_state;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto get_actual_damage(
	bool const ai_is_user,
	MoveName const executed,
	Damage const damage,
	auto const user_pokemon,
	auto const other_pokemon
) -> ActualDamage {
	auto const damaged_is_user = executed == MoveName::Hit_Self;
	auto const damaged_is_ai = !ai_is_user xor damaged_is_user;
	auto const old_hp = damaged_is_user ? user_pokemon.hp() : other_pokemon.hp();
	return visible_damage_to_actual_damage(
		damage,
		damaged_is_ai,
		old_hp,
		other_pokemon.substitute()
	);
}

constexpr auto move_should_have_recoil(
	Generation const generation,
	Used const move,
	auto const user_pokemon,
	auto const other_pokemon
) -> bool {
	if (move.miss) {
		return false;
	}
	if (generation == Generation::one and move.damage.index() == bounded::type<SubstituteBroke>) {
		return false;
	}
	auto const user_hidden_power_type = get_hidden_power_type(user_pokemon);
	auto const type = move_type(generation, move.executed, user_hidden_power_type);
	auto const other_types = other_pokemon.types();
	if (Effectiveness(generation, type, other_types).has_no_effect()) {
		return false;
	}
	if (ability_blocks_move(
		generation,
		other_pokemon.ability(),
		KnownMove(move.executed, type),
		other_pokemon.status().name(),
		other_types
	)) {
		return false;
	}
	return causes_recoil(move.executed);
}

constexpr auto ability_from_recoil(
	Generation const generation,
	Used const move,
	auto const user_pokemon,
	auto const other_pokemon
) -> tv::optional<Ability> {
	if (move.recoil) {
		// TODO: This can let you know they don't have Rock Head
		return tv::none;
	}
	if (!move_should_have_recoil(generation, move, user_pokemon, other_pokemon)) {
		return tv::none;
	}
	// TODO: Could be Magic Guard
	return Ability::Rock_Head;
}

template<typename PokemonType>
constexpr auto other_action(PokemonType const other_pokemon, ActualDamage const damage) -> OtherAction {
	auto const last_used_move = other_pokemon.last_used_move();
	if (!last_used_move.moved_this_turn()) {
		return FutureSelection(damage.did_any_damage());
	}
	auto const move_name = last_used_move.name();
	if (!move_name) {
		return IrrelevantAction();
	}
	auto const type = move_type(
		generation_from<PokemonType>,
		*move_name,
		get_hidden_power_type(other_pokemon)
	);
	return KnownMove(*move_name, type);
}

export template<Generation generation>
struct Battle {
	// TODO: Properly order instead of having a default
	Battle(
		KnownTeam<generation> ai_,
		SeenTeam<generation> foe_,
		bool const ai_first = true
	):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_))
	{
		auto const ai_pokemon = m_ai.pokemon();
		auto const foe_pokemon = m_foe.pokemon();
		auto do_switch = [&](auto const switcher, auto const other) {
			switcher.switch_in(m_environment, true);
			activate_ability_on_switch(switcher, other, m_environment);
		};
		if (ai_first) {
			do_switch(ai_pokemon, foe_pokemon);
			do_switch(foe_pokemon, ai_pokemon);
		} else {
			do_switch(ai_pokemon, foe_pokemon);
			do_switch(foe_pokemon, ai_pokemon);
		}
	}

	auto ai() const -> KnownTeam<generation> const & {
		return m_ai;
	}
	auto foe() const -> SeenTeam<generation> const & {
		return m_foe;
	}
	auto environment() const -> Environment {
		return m_environment;
	}
	auto state() const -> VisibleState<generation> {
		return VisibleState<generation>(
			m_ai,
			m_foe,
			m_environment
		);
	}

	// This assumes Species Clause is in effect. Throws if the Species is not in
	// the team.
	auto ai_has(Species const species, Nickname, Level, Gender) const -> TeamIndex {
		// TODO: Validate nickname, level, and gender?
		return find_required_pokemon_index(m_ai.all_pokemon(), species);
	}
	// This assumes Species Clause is in effect. Adds a Pokemon to the team if
	// Species has not been seen yet.
	auto foe_has(
		Species const species,
		Nickname const nickname,
		Level const level,
		Gender const gender,
		MaxVisibleHP const max_hp
	) & -> TeamIndex {
		auto const it = containers::find_if(m_foe.all_pokemon(), [=](SeenPokemon<generation> const & pokemon) {
			return pokemon.species() == species;
		});
		if (it == containers::end(m_foe.all_pokemon())) {
			m_foe.all_pokemon().add({species, nickname, level, gender, max_hp});
		}
		return bounded::assume_in_range<TeamIndex>(it - containers::begin(m_foe.all_pokemon()));
	}

	auto active_has(bool const is_ai, Ability const ability) -> void {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().set_base_ability(ability);
		});
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Ability const ability) -> void {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_initial_ability(ability);
		});
	}

	auto active_has(bool const is_ai, Item const item) -> void {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.all_pokemon()().set_item(item);
		});
	}
	auto replacement_has(bool const is_ai, TeamIndex const index, Item const item) -> void {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon(index).set_item(item);
		});
	}

	auto active_has(bool const is_ai, MoveName const move_name) & -> void {
		apply_to_teams(is_ai, [=](auto & team, auto const &) {
			team.pokemon().add_move(Move(generation, move_name));
		});
	}
	auto use_move(bool const ai_is_user, MoveResult const move_result, bool const status_clears) & -> void {
		apply_to_teams(ai_is_user, [&](auto & user_team, auto & other_team) {
			auto const user_pokemon = user_team.pokemon();
			auto const other_pokemon = other_team.pokemon();

			// TODO: Handle the other states better
			auto const move = tv::visit(move_result, tv::overload(
				[](Used const used) {
					return used;
				},
				[&](Recharging) {
					auto const last_move = user_pokemon.last_used_move().name();
					if (!last_move) {
						throw std::runtime_error("Tried to recharge without having just used a move");
					}
					return Used(*last_move);
				},
				[](auto) {
					return Used(MoveName::Struggle);
				}
			));

			user_pokemon.add_move(Move(generation, move.selected));
			if (move.selected == MoveName::Sleep_Talk) {
				user_pokemon.add_move(Move(generation, move.executed));
			}

			auto const recoil_ability = ability_from_recoil(
				generation,
				move,
				user_pokemon,
				other_pokemon
			);
			if (recoil_ability) {
				user_pokemon.set_base_ability(*recoil_ability);
			}

			auto const damage = get_actual_damage(
				ai_is_user,
				move.executed,
				move.damage,
				user_pokemon,
				other_pokemon
			);
			call_move(
				user_team,
				to_used_move(move, user_team, other_team, m_environment),
				other_team,
				other_action(other_pokemon, damage),
				m_environment,
				status_clears,
				damage,
				move_result.index() == bounded::type<FullyParalyzed>
			);
		});
	}

	auto use_switch(bool const ai_is_user, Switch const switch_) & -> void {
		apply_to_teams(ai_is_user, [&](auto & user_team, auto & other_team) {
			do_switch(user_team, switch_, other_team, m_environment);
		});
	}

	auto end_turn(bool const ai_went_first, EndOfTurnFlags const first_flags, EndOfTurnFlags const last_flags) & -> void {
		apply_to_teams(ai_went_first, [&](auto & first, auto & last) {
			end_of_turn(first, first_flags, last, last_flags, m_environment);
		});
	}

	// TODO: What happens here if a Pokemon has a pinch item?
	auto correct_hp(bool const is_ai, VisibleHP const visible_hp, auto... maybe_index) & -> void {
		apply_to_teams(is_ai, [=]<any_team TeamType>(TeamType & team, auto const &) {
			auto & pokemon = team.all_pokemon()(maybe_index...);
			if constexpr (any_seen_team<TeamType>) {
				auto const original_hp = pokemon.visible_hp();
				auto const current_hp = original_hp.current;
				auto const hp_acceptable = current_hp.value() - 1_bi <= visible_hp.current.value() and visible_hp.current.value() <= current_hp.value() + 1_bi;
				if (!hp_acceptable) {
					std::cerr << "Seen HP out of sync with server messages. Expected " << current_hp.value() << " but received " << visible_hp.current.value() << '\n';
				}
				pokemon.set_hp(visible_hp.current);
			} else {
				static_assert(any_known_team<TeamType>);
				if (pokemon.hp().current() != visible_hp.current.value()) {
					std::cerr << "Known HP out of sync with server messages. Expected " << pokemon.hp().current() << " but received " << visible_hp.current.value() << " (max of " << pokemon.hp().max() << ")\n";
					pokemon.set_hp(visible_hp.current.value());
					//throw std::runtime_error("ahh");
				}
			}
		});
	}

	auto correct_status(bool const is_ai, StatusName const visible_status, auto... maybe_index) & -> void {
		apply_to_teams(is_ai, [=](auto const & team, auto &) {
			auto const original_status = team.all_pokemon()(maybe_index...).status().name();
			auto const normalized_original_status = (original_status == StatusName::rest) ? StatusName::sleep : original_status;
			if (normalized_original_status != visible_status) {
				throw std::runtime_error(containers::concatenate<std::string>(
					"Status out of sync with server messages: expected "sv,
					to_string(original_status),
					" but received "sv,
					to_string(visible_status)
				));
			}
		});
	}

private:
	auto apply_to_teams(bool const is_ai, auto function) -> decltype(auto) {
		if (is_ai) {
			return function(m_ai, m_foe);
		} else {
			return function(m_foe, m_ai);
		}
	}

	KnownTeam<generation> m_ai;
	SeenTeam<generation> m_foe;
	Environment m_environment;
};

} // namespace technicalmachine
