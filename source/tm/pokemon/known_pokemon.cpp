// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.known_pokemon;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.any_pokemon;
import tm.pokemon.happiness;
import tm.pokemon.has_hidden_power;
import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.status.status;
import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.move_name;
import tm.string_conversions.species;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;

import bounded;
import containers;
import tv;
import std_module;

using namespace bounded::literal;

namespace technicalmachine {
using namespace std::string_view_literals;

export template<Generation generation>
struct KnownPokemon {
	constexpr KnownPokemon(Species const species_, Nickname nickname_, Level const level_, Gender const gender_, Item const item_, Ability const ability_, CombinedStats<generation> const stat_inputs, RegularMoves regular_moves_, Happiness const happiness_ = Happiness()):
		m_pokemon(species_, level_, gender_, item_, ability_, stat_inputs, regular_moves_, happiness_),
		m_nickname(std::move(nickname_))
	{
		check_no_generation_one_hidden_power();
	}

	explicit constexpr KnownPokemon(Pokemon<generation> pokemon):
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

	constexpr auto stat(SplitSpecialRegularStat const stat_name) const {
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
	constexpr auto add_move(Move const move) const -> void {
		if (!containers::any_equal(regular_moves(), move.name()) and is_regular(move.name())) {
			using namespace std::string_view_literals;
			throw std::runtime_error(containers::concatenate<std::string>(
				"Tried to add "sv,
				to_string(move.name()),
				" to a KnownPokemon of "sv,
				to_string(species()),
				" even though it does not know this move."
			));
		}
	}

	constexpr auto reduce_pp(MoveName const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & -> void {
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

	constexpr auto hidden_power() const -> tv::optional<HiddenPower<generation>> {
		return m_pokemon.hidden_power();
	}

	constexpr auto item(bool const embargo, bool const magic_room) const -> Item {
		return m_pokemon.item(embargo, magic_room);
	}
	constexpr auto remove_item() & -> tv::optional<Item> {
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
	constexpr auto set_status(StatusName const status) & -> void {
		m_pokemon.set_status(status);
	}

	friend auto operator==(KnownPokemon const &, KnownPokemon const &) -> bool = default;

private:
	constexpr auto check_no_generation_one_hidden_power() const {
		// TODO: More general check that they don't have later generation moves
		if (generation == Generation::one and has_hidden_power(regular_moves())) {
			throw std::runtime_error("Generation 1 Pokemon cannot have Hidden Power");
		}
	}

	constexpr explicit KnownPokemon(bounded::tombstone_tag, auto const make):
		m_pokemon(make()),
		m_nickname()
	{
	}

	Pokemon<generation> m_pokemon;
	Nickname m_nickname;

	friend bounded::tombstone_traits<KnownPokemon<generation>>;
	friend bounded::tombstone_traits_composer<&KnownPokemon<generation>::m_pokemon>;
};

template<Generation generation>
constexpr auto is_known_pokemon<KnownPokemon<generation>> = true;

} // namespace technicalmachine

template<technicalmachine::Generation generation>
struct bounded::tombstone_traits<technicalmachine::KnownPokemon<generation>> : bounded::tombstone_traits_composer<&technicalmachine::KnownPokemon<generation>::m_pokemon> {
};

namespace technicalmachine {

static_assert(bounded::tombstone_traits<KnownPokemon<Generation::three>>::spare_representations > 0_bi);

} // namespace technicalmachine