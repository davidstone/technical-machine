// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/pokemon.hpp>
#include <tm/string_conversions/ability.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

#include <stdexcept>
#include <string_view>

namespace technicalmachine {

template<Generation generation>
struct KnownPokemon {
	constexpr KnownPokemon(Species const species_, containers::string nickname_, Level const level_, Gender const gender_, Item const item_, Ability const ability_, CombinedStats<generation> const stat_inputs, RegularMoves regular_moves_, Happiness const happiness_ = Happiness()):
		m_pokemon(species_, level_, gender_, item_, ability_, stat_inputs, regular_moves_, happiness_),
		m_nickname(std::move(nickname_))
	{
		check_no_generation_one_hidden_power();
	}

	explicit KnownPokemon(Pokemon<generation> pokemon):
		m_pokemon(pokemon),
		m_nickname(to_string(m_pokemon.species()))
	{
		check_no_generation_one_hidden_power();
	}

	constexpr explicit operator Pokemon<generation>() const {
		return m_pokemon;
	}

	constexpr auto nickname() const -> std::string_view {
		return m_nickname;
	}

	auto stat(SplitSpecialRegularStat const stat_name) const {
		return m_pokemon.stat(stat_name);
	}
	constexpr auto hp() const {
		return m_pokemon.hp();
	}

	constexpr auto advance_status_from_move(Ability const ability, bool const clear_status) & -> void {
		m_pokemon.advance_status_from_move(ability, clear_status);
	}

	constexpr auto has_been_seen() const -> bool {
		return m_pokemon.has_been_seen();
	}
	constexpr auto mark_as_seen() & -> void {
		m_pokemon.mark_as_seen();
	}

	constexpr auto regular_moves() const -> RegularMoves {
		return m_pokemon.regular_moves();
	}
	auto add_move(Move const move) const -> void {
		if (!containers::any_equal(regular_moves(), move.name()) and is_regular(move.name())) {
			throw std::runtime_error("Tried to add non-existent move to a KnownPokemon");
		}
	}

	constexpr auto reduce_pp(Moves const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & -> void {
		m_pokemon.reduce_pp(move_name, embargo, magic_room, amount);
	}

	constexpr auto set_hp(bounded::bounded_integer auto const hp) & -> void {
		m_pokemon.set_hp(hp);
	}

	constexpr auto initial_ability() const -> Ability {
		return m_pokemon.initial_ability();
	}
	constexpr auto set_initial_ability(Ability const ability) const -> void {
		if (ability != initial_ability()) {
			throw std::runtime_error(containers::concatenate<std::string>("Inconsistent abilities. Tried to change "sv, nickname(), " from "sv, to_string(initial_ability()), " to "sv, to_string(ability)));
		}
	}

	constexpr auto gender() const -> Gender {
		return m_pokemon.gender();
	}

	constexpr auto happiness() const -> Happiness {
		return m_pokemon.happiness();
	}

	auto hidden_power() const -> bounded::optional<HiddenPower<generation>> {
		return m_pokemon.hidden_power();
	}

	constexpr auto item(bool const embargo, bool const magic_room) const -> Item {
		return m_pokemon.item(embargo, magic_room);
	}
	constexpr auto remove_item() & -> bounded::optional<Item> {
		return m_pokemon.remove_item();
	}
	constexpr auto destroy_item() & -> bool {
		return m_pokemon.destroy_item();
	}
	constexpr auto recycle_item() & -> void {
		m_pokemon.recycle_item();
	}
	constexpr auto set_item(Item const item_) & -> void {
		// TODO: validate this somehow
		m_pokemon.set_item(item_);
	}

	constexpr auto level() const -> Level {
		return m_pokemon.level();
	}

	constexpr auto nature() const -> Nature {
		return m_pokemon.nature();
	}
	constexpr auto species() const -> Species {
		return m_pokemon.species();
	}

	constexpr auto status() const -> Status {
		return m_pokemon.status();
	}
	constexpr auto set_status(Statuses const status) & -> void {
		m_pokemon.set_status(status);
	}

	friend auto operator==(KnownPokemon const &, KnownPokemon const &) -> bool = default;
private:
	constexpr auto check_no_generation_one_hidden_power() const {
		// TODO: More general check that they don't have later generation moves
		if (generation == Generation::one and has_hidden_power(*this)) {
			throw std::runtime_error("Generation 1 Pokemon cannot have Hidden Power");
		}
	}

	Pokemon<generation> m_pokemon;
	containers::string m_nickname;
};

} // namespace technicalmachine
