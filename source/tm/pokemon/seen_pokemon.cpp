// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.seen_pokemon;

import tm.move.move;
import tm.move.move_name;
import tm.move.pp;
import tm.move.regular_moves;

import tm.pokemon.any_pokemon;
import tm.pokemon.happiness;
import tm.pokemon.has_hidden_power;
import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.hp;
import tm.stat.make_stats;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stats;
import tm.stat.to_real_hp;

import tm.status.status;
import tm.status.status_name;

import tm.string_conversions.species;

import tm.type.type;

import tm.ability;
import tm.exists_if;
import tm.gender;
import tm.generation;
import tm.held_item;
import tm.item;
import tm.visible_hp;

import bounded;
import containers;
import numeric_traits;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

export template<Generation generation>
struct SeenPokemon {
	constexpr SeenPokemon(Species const species, Nickname const nickname, Level const level, Gender const gender, MaxVisibleHP const hp_resolution = MaxVisibleHP(100_bi)):
		m_nickname(nickname),
		m_species(species),
		m_gender(gender),
		m_level(level),
		m_hp{CurrentVisibleHP(hp_resolution.value()), hp_resolution},
		m_stats(make_stats<stat_style_for(generation)>(
			BaseStats(generation, m_species),
			m_level,
			CombinedStatsFor<generation>()
		))
	{
	}
	constexpr SeenPokemon(Species const species, Level const level, Gender const gender, MaxVisibleHP const hp_resolution = MaxVisibleHP(100_bi)):
		SeenPokemon(species, Nickname(to_string(species)), level, gender, hp_resolution)
	{
	}

	constexpr auto nickname() const -> Nickname {
		return m_nickname;
	}

	constexpr auto stats() const -> Stats<stat_style_for(generation)> {
		auto result = m_stats;
		result.hp() = hp();
		return result;
	}
	constexpr auto visible_hp() const -> VisibleHP {
		return m_hp;
	}
	constexpr auto hp() const -> HP {
		auto const max_hp = m_stats.hp().max();
		auto temp = HP(max_hp);
		temp = to_real_hp(max_hp, visible_hp()).value;
		return temp;
	}
	constexpr auto set_hp(CurrentVisibleHP const visible_hp) & -> void {
		// TODO: Do something smart with actual
		if (visible_hp > m_hp.max) {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Tried to set a visible HP of "sv,
				containers::to_string(visible_hp.value()),
				", is greater than visible max HP of "sv,
				containers::to_string(m_hp.max.value())
			));
		}
		m_hp.current = visible_hp;
	}
	constexpr auto set_hp(bounded::bounded_integer auto const given_real_hp) & -> void {
		auto const max_real_hp = m_stats.hp().max();
		auto const real_hp = bounded::clamp(given_real_hp, 0_bi, max_real_hp);
		set_hp(CurrentVisibleHP(bounded::assume_in_range(m_hp.max.value() * real_hp / max_real_hp, 0_bi, m_hp.max.value())));
	}

	constexpr auto advance_status_from_move(Ability const ability, bool const clear_status) & -> void {
		m_status.advance_from_move(ability, clear_status);
	}

	constexpr auto mark_as_seen() & -> void {
	}

	constexpr auto regular_moves() const -> RegularMoves {
		return m_regular_moves;
	}
	constexpr auto add_move(Move const move) & -> void {
		if (containers::any_equal(regular_moves(), move.name()) or !is_regular(move.name())) {
			return;
		}
		m_regular_moves.push_back(move);
	}

	constexpr auto reduce_pp(MoveName const move_name, bool const embargo, bool const magic_room, bounded::bounded_integer auto const amount) & -> void {
		auto const maybe_move = containers::maybe_find(m_regular_moves, move_name);
		if (!maybe_move) {
			return;
		}
		auto & move = *maybe_move;
		move.reduce_pp(amount);
		activate_pp_restore_berry(move, embargo, magic_room);
	}

	constexpr auto initial_ability() const -> Ability {
		if constexpr (exists<decltype(m_ability)>) {
			// TODO: ???
			return m_ability ? *m_ability : Ability::Honey_Gather;
		} else {
			return Ability::Honey_Gather;
		}
	}
	constexpr auto set_initial_ability(Ability const ability) & -> void {
		if constexpr (exists<decltype(m_ability)>) {
			m_ability = ability;
		}
	}

	constexpr auto gender() const -> Gender {
		if constexpr (exists<decltype(m_gender)>) {
			return m_gender;
		} else {
			return Gender::genderless;
		}
	}

	// TODO: ???
	constexpr auto happiness() const -> Happiness {
		return Happiness();
	}

	constexpr auto item(bool const embargo, bool const magic_room) const -> Item {
		if constexpr (exists<decltype(m_item)>) {
			// TODO: ???
			return m_item ? m_item->get(generation, embargo, magic_room) : Item::None;
		} else {
			return Item::None;
		}
	}
	constexpr auto actual_item() const -> HeldItem {
		// TODO: ???
		if constexpr (exists<decltype(m_item)>) {
			return m_item ? *m_item : HeldItem(Item::None);
		} else {
			return HeldItem(Item::None);
		}
	}
	constexpr auto remove_item() & -> tv::optional<Item> {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			return m_item->remove();
		} else {
			return tv::none;
		}
	}
	constexpr auto destroy_item() & -> bool {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			return m_item->destroy();
		} else {
			return false;
		}
	}
	constexpr auto recycle_item() & -> void {
		if constexpr (exists<decltype(m_item)>) {
			if (!m_item) {
				// https://github.com/davidstone/technical-machine/issues/116
				throw std::runtime_error("Tried to remove an unknown item");
			}
			m_item->recycle();
		}
	}
	constexpr auto set_item(Item const item) & -> void {
		if constexpr (exists<decltype(m_item)>) {
			m_item = HeldItem(item);
		}
	}

	constexpr auto level() const -> Level {
		return m_level;
	}

	constexpr auto species() const -> Species {
		return m_species;
	}

	constexpr auto status() const -> Status {
		return m_status;
	}
	constexpr auto set_status(StatusName const status) & -> void {
		m_status = status;
	}

	constexpr auto ability_is_known() const -> bool {
		if constexpr (exists<decltype(m_ability)>) {
			return static_cast<bool>(m_ability);
		} else {
			return true;
		}
	}
	constexpr auto item_is_known() const -> bool {
		if constexpr (exists<decltype(m_item)>) {
			return static_cast<bool>(m_item);
		} else {
			return true;
		}
	}

	constexpr auto nature() const -> Nature {
		if constexpr (exists<decltype(m_nature)>) {
			return m_nature ? *m_nature : Nature::Hardy;
		} else {
			return Nature::Hardy;
		}
	}

	constexpr auto set_ivs_and_evs(CombinedStatsFor<generation> const stat_inputs) -> void {
		if constexpr (exists<decltype(m_nature)>) {
			m_nature = stat_inputs.nature;
		}
		m_stats = make_stats<stat_style_for(generation)>(
			BaseStats(generation, species()),
			level(),
			stat_inputs
		);
	}

	constexpr auto hidden_power() const -> tv::optional<HiddenPower<generation>> {
		// TODO: ???
		return has_hidden_power(regular_moves()) ?
			HiddenPower<generation>(numeric_traits::max_value<typename HiddenPower<generation>::Power>, Type::Dark) :
			tv::optional<HiddenPower<generation>>();
	}

	friend auto operator==(SeenPokemon, SeenPokemon) -> bool = default;

private:
	constexpr auto activate_pp_restore_berry(Move & move, bool const embargo, bool const magic_room) & -> void {
		if (no_pp(move.pp())) {
			switch (item(embargo, magic_room)) {
				case Item::Leppa_Berry:
					move.restore_pp(10_bi);
					remove_item();
					break;
				case Item::MysteryBerry:
					move.restore_pp(5_bi);
					remove_item();
					break;
				default:
					break;
			}
		}
	}

	constexpr explicit SeenPokemon(bounded::tombstone_tag, auto const make):
		m_nickname(),
		m_regular_moves(make()),
		m_species(),
		m_item(bounded::tombstone_traits<HeldItem>::make(0_bi)),
		m_ability(),
		m_gender(),
		m_nature(),
		m_status(),
		m_level(bounded::tombstone_traits<Level>::make(0_bi)),
		m_hp(CurrentVisibleHP(0_bi), MaxVisibleHP(100_bi)),
		m_stats(bounded::tombstone_traits<Stats<stat_style_for(generation)>>::make(0_bi))
	{
	}

	Nickname m_nickname;
	RegularMoves m_regular_moves;

	Species m_species;
	// TODO: https://github.com/llvm/llvm-project/issues/61360
	[[no_unique_address]] ExistsIf<tv::optional<HeldItem>, generation >= Generation::two> m_item;
	[[no_unique_address]] ExistsIf<tv::optional<Ability>, generation >= Generation::three> m_ability;
	[[no_unique_address]] ExistsIf<Gender, generation >= Generation::two> m_gender;
	[[no_unique_address]] ExistsIf<tv::optional<Nature>, generation >= Generation::three> m_nature;
	Status m_status;

	Level m_level;

	VisibleHP m_hp;
	// TODO: Make this a set of possible ranges. The current HP is never used
	// from this. When the representation is changed to store possible ranges
	// instead of an exact value, that useless state will go away.
	Stats<stat_style_for(generation)> m_stats;

	// TODO: Possible Hidden Power types

	friend bounded::tombstone<SeenPokemon<generation>>;
	friend bounded::tombstone_member<&SeenPokemon<generation>::m_regular_moves>;
};

template<Generation generation>
constexpr auto is_seen_pokemon<SeenPokemon<generation>> = true;

} // namespace technicalmachine

template<technicalmachine::Generation generation>
struct bounded::tombstone<technicalmachine::SeenPokemon<generation>> : bounded::tombstone_member<&technicalmachine::SeenPokemon<generation>::m_regular_moves> {
};
