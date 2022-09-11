// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/active_status.hpp>
#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/confusion.hpp>
#include <tm/pokemon/disable.hpp>
#include <tm/pokemon/embargo.hpp>
#include <tm/pokemon/encore.hpp>
#include <tm/pokemon/heal_block.hpp>
#include <tm/pokemon/last_used_move.hpp>
#include <tm/pokemon/magnet_rise.hpp>
#include <tm/pokemon/partial_trap.hpp>
#include <tm/pokemon/perish_song.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/slow_start.hpp>
#include <tm/pokemon/stockpile.hpp>
#include <tm/pokemon/substitute.hpp>
#include <tm/pokemon/taunt.hpp>
#include <tm/pokemon/yawn.hpp>

#include <tm/move/damage_type.hpp>
#include <tm/move/move.hpp>
#include <tm/move/move_name.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/stage.hpp>

#include <tm/type/pokemon_types.hpp>

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/other_team.hpp>
#include <tm/weather.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/range_value_t.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

template<Generation generation>
struct ActivePokemonFlags {
	auto reset_start_of_turn() & {
		last_used_move.reset_start_of_turn();
		damaged = false;
		direct_damage_received = 0_bi;
		flinched = false;
		me_first_is_active = false;
		is_loafing_turn = !is_loafing_turn;
	}

	friend auto operator==(ActivePokemonFlags, ActivePokemonFlags) -> bool = default;
	friend auto compress(ActivePokemonFlags const value) {
		auto const result = bounded::tuple(
			compress_combine(
				value.ability,
				value.confusion,
				value.disable
			),
			compress_combine(
				value.embargo,
				value.encore,
				value.heal_block,
				value.last_used_move,
				value.magnet_rise,
				value.substitute,
				value.partial_trap,
				value.perish_song,
				value.stages,
				value.status,
				value.slow_start,
				value.stockpile,
				value.taunt,
				value.types,
				value.yawn,
				value.direct_damage_received,
				value.aqua_ring,
				value.attracted,
				value.charged,
				value.is_cursed,
				value.defense_curled,
				value.flash_fire,
				value.has_focused_energy,
				value.fully_trapped,
				value.gastro_acid,
				value.identified,
				value.used_imprison,
				value.ingrained,
				value.leech_seeded,
				value.is_loafing_turn,
				value.minimized,
				value.mud_sport,
				value.power_trick_is_active,
				value.is_tormented,
				value.unburdened,
				value.water_sport
			)
		);
		if constexpr (generation == Generation::one) {
			return bounded::tuple(compress_combine(result[0_bi], result[1_bi]));
		} else {
			return result;
		}
	}

private:
	template<typename>
	friend struct ActivePokemonImpl;
	template<typename>
	friend struct AnyMutableActivePokemon;

	[[no_unique_address]] ExistsIf<Ability, generation >= Generation::three> ability{};
	Confusion confusion;
	Disable disable;
	[[no_unique_address]] EmbargoCounter<generation> embargo;
	[[no_unique_address]] EncoreCounter<generation> encore;
	[[no_unique_address]] HealBlock<generation> heal_block;
	LastUsedMove last_used_move;
	[[no_unique_address]] MagnetRise<generation> magnet_rise;
	Substitute substitute;
	PartialTrap partial_trap;
	[[no_unique_address]] PerishSong<generation> perish_song;
	Stages stages;
	ActiveStatus status;
	[[no_unique_address]] SlowStart<generation> slow_start;
	[[no_unique_address]] Stockpile<generation> stockpile;
	[[no_unique_address]] TauntCounter<generation> taunt;
	PokemonTypes types{Type::Typeless};
	[[no_unique_address]] YawnCounter<generation> yawn;
	HP::current_type direct_damage_received = 0_bi;
	[[no_unique_address]] BoolIf<generation >= Generation::four> aqua_ring{};
	[[no_unique_address]] BoolIf<generation >= Generation::two> attracted{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> charged{};
	[[no_unique_address]] BoolIf<generation >= Generation::two> is_cursed{};
	bool damaged = false;
	[[no_unique_address]] BoolIf<generation >= Generation::two> defense_curled{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> flash_fire{};
	bool flinched = false;
	bool has_focused_energy = false;
	// Block, Mean Look, Spider Web
	[[no_unique_address]] BoolIf<generation >= Generation::two> fully_trapped{};
	[[no_unique_address]] BoolIf<generation >= Generation::four> gastro_acid{};
	[[no_unique_address]] BoolIf<generation >= Generation::two> identified{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> used_imprison{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> ingrained{};
	bool leech_seeded = false;
	[[no_unique_address]] BoolIf<generation >= Generation::three> is_loafing_turn{};
	[[no_unique_address]] BoolIf<generation >= Generation::four> me_first_is_active{};
	[[no_unique_address]] BoolIf<generation >= Generation::two> minimized{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> mud_sport{};
	[[no_unique_address]] BoolIf<generation >= Generation::four> power_trick_is_active{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> is_tormented{};
	[[no_unique_address]] BoolIf<generation >= Generation::four> unburdened{};
	[[no_unique_address]] BoolIf<generation >= Generation::three> water_sport{};
};

template<typename PokemonType>
struct ActivePokemonImpl {
private:
	static constexpr auto generation = generation_from<PokemonType>;
	using FlagsRef = std::conditional_t<std::is_const_v<PokemonType>, ActivePokemonFlags<generation> const &, ActivePokemonFlags<generation> &>;

public:
	ActivePokemonImpl(PokemonType & pokemon, FlagsRef flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	ActivePokemonImpl(ActivePokemonImpl const & other) = default;
	ActivePokemonImpl(ActivePokemonImpl && other) = default;
	ActivePokemonImpl & operator=(ActivePokemonImpl const & other) = delete;
	ActivePokemonImpl & operator=(ActivePokemonImpl && other) = delete;

	auto regular_moves() const -> RegularMoves {
		return m_pokemon.regular_moves();
	}

	auto hp() const {
		return m_pokemon.hp();
	}
	auto stat(SplitSpecialRegularStat const stat_name) const {
		return m_pokemon.stat(stat_name);
	}

	auto last_used_move() const -> LastUsedMove {
		return m_flags.last_used_move;
	}
	auto substitute() const -> Substitute const & {
		return m_flags.substitute;
	}

	auto ability() const -> Ability {
		if constexpr (exists<decltype(m_flags.ability)>) {
			return m_flags.ability;
		} else {
			return Ability::Honey_Gather;
		}
	}

	auto aqua_ring_is_active() const -> bool {
		return m_flags.aqua_ring;
	}

	auto charge_boosted(Type const move_type) const -> bool {
		return m_flags.charged and move_type == Type::Electric;
	}

	auto is_confused() const -> bool {
		return m_flags.confusion.is_active();
	}

	auto is_cursed() const -> bool {
		return m_flags.is_cursed;
	}

	auto defense_curled() const -> bool {
		return m_flags.defense_curled;
	}

	auto damaged() const -> bool {
		return m_flags.damaged;
	}

	auto direct_damage_received() const -> HP::current_type {
		return m_flags.direct_damage_received;
	}

	// Requires that move is actually one of this Pokemon's moves
	auto is_disabled(MoveName const move_name) const -> bool {
		return m_flags.disable.move_is_disabled(move_name);
	}

	auto is_encored() const -> bool {
		return m_flags.encore.is_active();
	}

	auto flash_fire_is_active() const -> bool {
		return m_flags.flash_fire;
	}

	auto flinched() const -> bool {
		return m_flags.flinched;
	}

	auto has_focused_energy() const -> bool {
		return m_flags.has_focused_energy;
	}

	auto gender() const -> Gender {
		return m_pokemon.gender();
	}

	auto happiness() const -> Happiness {
		return m_pokemon.happiness();
	}

	auto heal_block_is_active() const -> bool {
		return m_flags.heal_block.is_active();
	}

	auto hidden_power() const -> bounded::optional<HiddenPower<generation>> {
		return m_pokemon.hidden_power();
	}

	auto used_imprison() const -> bool {
		return m_flags.used_imprison;
	}

	auto ingrained() const -> bool {
		return m_flags.ingrained;
	}

	auto item(Weather const weather) const -> Item {
		return m_pokemon.item(m_flags.embargo.is_active(), weather.magic_room());
	}
	auto unrestricted_item() const -> Item {
		return m_pokemon.item(false, false);
	}

	auto leech_seeded() const -> bool {
		return m_flags.leech_seeded;
	}

	auto level() const -> Level {
		return m_pokemon.level();
	}

	auto is_loafing() const -> bool {
		return ability() == Ability::Truant and m_flags.is_loafing_turn;
	}

	auto magnet_rise() const {
		return m_flags.magnet_rise;
	}

	auto me_first_is_active() const -> bool {
		return m_flags.me_first_is_active;
	}

	auto minimized() const -> bool {
		return m_flags.minimized;
	}

	auto power_trick_is_active() const -> bool {
		return m_flags.power_trick_is_active;
	}

	auto slow_start_is_active() const -> bool {
		return m_flags.slow_start.is_active();
	}

	auto species() const -> Species {
		return m_pokemon.species();
	}

	auto sport_is_active(Type const move_type) const -> bool {
		switch (move_type) {
			case Type::Electric: return m_flags.mud_sport;
			case Type::Fire: return m_flags.water_sport;
			default: return false;
		}
	}

	auto stages() const -> Stages const & {
		return m_flags.stages;
	}

	auto spit_up_power() const {
		return m_flags.stockpile.spit_up_power();
	}

	auto status() const -> Status {
		return m_pokemon.status();
	}

	auto fully_trapped() const -> bool {
		return m_flags.fully_trapped;
	}

	auto trapped() const -> bool {
		return m_flags.fully_trapped or ingrained() or m_flags.partial_trap.is_active();
	}

	auto is_tormented() const -> bool {
		return m_flags.is_tormented;
	}

	auto is_taunted() const -> bool {
		return m_flags.taunt.is_active();
	}

	auto types() const {
		return m_flags.types;
	}

	auto is_unburdened() const -> bool {
		return m_flags.unburdened;
	}

protected:
	PokemonType & m_pokemon;
	FlagsRef m_flags;
};

auto apply_own_mental_herb(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
	if (pokemon.item(weather) == Item::Mental_Herb) {
		pokemon.apply_mental_herb();
		pokemon.remove_item();
	}
}

// A reference to the currently active Pokemon
template<typename PokemonType>
struct AnyActivePokemon : ActivePokemonImpl<PokemonType const> {
	AnyActivePokemon(PokemonType const & pokemon, ActivePokemonFlags<generation_from<PokemonType>> const & flags):
		ActivePokemonImpl<PokemonType const>(pokemon, flags)
	{
	}
};

auto is_type(any_active_pokemon auto const pokemon, auto const... types) -> bool requires(sizeof...(types) > 0) {
	return (... or (
		(types != Type::Flying or !pokemon.last_used_move().is_roosting()) and
		containers::any_equal(pokemon.types(), types)
	));
}

template<any_active_pokemon PokemonType>
auto grounded(PokemonType const pokemon, Weather const weather) -> bool {
	auto item_grounds = [=] {
		auto const item = generation_from<PokemonType> <= Generation::four ?
			pokemon.unrestricted_item() :
			pokemon.item(weather);
		return item == Item::Iron_Ball;
	};

	return
		!(
			is_type(pokemon, Type::Flying) or
			is_immune_to_ground(pokemon.ability()) or
			pokemon.magnet_rise().is_active() or
			pokemon.item(weather) == Item::Air_Balloon
		) or
		weather.gravity() or
		item_grounds() or
		pokemon.ingrained();
}

auto activate_berserk_gene(any_mutable_active_pokemon auto pokemon, Weather const weather) -> void {
	pokemon.stages()[BoostableStat::atk] += 2_bi;
	// TODO: Berserk Gene causes 256-turn confusion, unless the Pokemon
	// switching out was confused.
	pokemon.confuse(weather);
	pokemon.remove_item();
}

constexpr bool cannot_ko(MoveName const move) {
	return move == MoveName::False_Swipe;
}

auto indirect_status_can_apply(StatusName const status, any_active_pokemon auto const target, Weather const weather) {
	return
		is_clear(target.status()) and
		!blocks_status(target.ability(), Ability::Honey_Gather, status, weather) and
		!containers::any(target.types(), [=](Type const type) { return blocks_status(type, status); });
}

auto yawn_can_apply(any_active_pokemon auto const target, Weather const weather, bool const either_is_uproaring, bool const sleep_clause_activates) {
	return
		!sleep_clause_activates and
		!either_is_uproaring and
		indirect_status_can_apply(StatusName::sleep, target, weather);
}

// A mutable reference to the currently active Pokemon
template<typename PokemonType>
struct AnyMutableActivePokemon : ActivePokemonImpl<PokemonType> {
private:
	static constexpr auto generation = generation_from<PokemonType>;
public:
	AnyMutableActivePokemon(PokemonType & pokemon, ActivePokemonFlags<generation_from<PokemonType>> & flags):
		ActivePokemonImpl<PokemonType>(pokemon, flags)
	{
	}
	
	auto as_const() const {
		return AnyActivePokemon<PokemonType>(this->m_pokemon, this->m_flags);
	}

	auto add_move(Move const move) const -> void {
		return this->m_pokemon.add_move(move);
	}
	auto reduce_pp(MoveName const move_name, Weather const weather, bounded::bounded_integer auto const amount) const -> void {
		this->m_pokemon.reduce_pp(move_name, this->m_flags.embargo.is_active(), weather.magic_room(), amount);
	}

	auto stages() const -> Stages & {
		return this->m_flags.stages;
	}

	auto clear_field() const {
		this->m_flags.leech_seeded = false;
		this->m_flags.partial_trap = {};
	}

	auto set_ability(Ability const ability) const -> void {
		this->m_flags.ability = ability;
	}
	auto set_base_ability(Ability const ability) const -> void requires requires(PokemonType & pokemon) { pokemon.set_initial_ability(ability); } {
		this->m_pokemon.set_initial_ability(ability);
		set_ability(ability);
	}
	auto set_ability_to_base_ability() const -> void {
		set_ability(this->m_pokemon.initial_ability());
	}

	auto activate_aqua_ring() const {
		this->m_flags.aqua_ring = true;
	}
	auto attract(OtherMutableActivePokemon<PokemonType> other, Weather const weather) const -> void {
		auto handle_item = [&] {
			switch (this->item(weather)) {
				case Item::Mental_Herb:
					apply_own_mental_herb(*this, weather);
					break;
				case Item::Destiny_Knot:
					remove_item();
					other.attract(*this, weather);
					break;
				default:
					break;
			}
		};
		auto const ability_cures_attract = this->ability() == Ability::Oblivious;
		if constexpr (generation <= Generation::four) {
			if (!ability_cures_attract) {
				this->m_flags.attracted = true;
				handle_item();
			}
		} else {
			this->m_flags.attracted = true;
			handle_item();
			if (ability_cures_attract) {
				this->m_flags.attracted = false;
			}
		}
	}

	auto charge() const {
		this->m_flags.charged = true;
	}
	auto use_charge_up_move() const -> void {
		this->m_flags.last_used_move.use_charge_up_move();
	}
	auto confuse(Weather const weather) const -> void {
		if (blocks_confusion(this->ability())) {
			return;
		}
		if (clears_confusion(this->item(weather))) {
			remove_item();
		} else {
			this->m_flags.confusion.activate();
		}
	}
	auto handle_confusion() const {
		this->m_flags.confusion.do_turn();
	}
	auto curse() const {
		this->m_flags.is_cursed = true;
	}
	auto defense_curl() const {
		this->m_flags.defense_curled = true;
	}
	auto disable(MoveName const move, Weather const weather) const {
		if (this->m_flags.disable.activate(move)) {
			apply_own_mental_herb(*this, weather);
		}
	}
	auto advance_disable() const {
		this->m_flags.disable.advance_one_turn();
	}
	auto activate_embargo() const {
		this->m_flags.embargo.activate();
	}
	auto advance_embargo() const {
		this->m_flags.embargo.advance_one_turn();
	}
	auto activate_encore(Weather const weather) const {
		this->m_flags.encore.activate();
		apply_own_mental_herb(*this, weather);
	}
	auto advance_encore() const {
		this->m_flags.encore.advance_one_turn();
	}
	auto activate_flash_fire() const {
		this->m_flags.flash_fire = true;
	}
	auto flinch() const {
		this->m_flags.flinched = true;
	}
	auto focus_energy() const {
		this->m_flags.has_focused_energy = true;
	}
	auto fully_trap() const {
		this->m_flags.fully_trapped = true;
	}
	auto activate_heal_block() const {
		this->m_flags.heal_block.activate();
	}
	auto advance_heal_block() const {
		this->m_flags.heal_block.advance_one_turn();
	}
	auto identify() const {
		this->m_flags.identified = true;
	}
	auto use_imprison() const {
		this->m_flags.used_imprison = true;
	}
	auto ingrain() const {
		this->m_flags.ingrained = true;
	}

	auto remove_item() const -> bounded::optional<Item> {
		auto result = this->m_pokemon.remove_item();
		if (result) {
			this->m_flags.unburdened = true;
		}
		return result;
	}
	auto destroy_item() const -> bool {
		auto result = this->m_pokemon.destroy_item();
		if (result) {
			this->m_flags.unburdened = true;
		}
		return result;
	}
	auto recycle_item() const -> void {
		this->m_pokemon.recycle_item();
	}
	auto steal_item(OtherMutableActivePokemon<PokemonType> other) const -> void {
		if (this->unrestricted_item() != Item::None) {
			return;
		}
		if (auto const other_item = other.remove_item()) {
			this->m_pokemon.set_item(*other_item);
		}
	}
	friend auto swap_items(AnyMutableActivePokemon<OtherPokemon<PokemonType>> user, AnyMutableActivePokemon<PokemonType> other) -> void;
	friend auto swap_items(AnyMutableActivePokemon<PokemonType> user, AnyMutableActivePokemon<OtherPokemon<PokemonType>> other) -> void {
		auto const user_item = user.unrestricted_item();
		auto const other_item = other.unrestricted_item();
		if (!cannot_be_lost(user_item) and !cannot_be_lost(other_item)) {
			user.m_pokemon.set_item(other_item);
			other.m_pokemon.set_item(user_item);
		}
	}



	auto hit_with_leech_seed() const {
		this->m_flags.leech_seeded = true;
	}
	auto advance_lock_in(bool const ending, Weather const weather) const {
		auto const confused = this->m_flags.last_used_move.advance_lock_in(ending);
		if (confused) {
			confuse(weather);
		}
	}
	auto activate_magnet_rise() const {
		this->m_flags.magnet_rise.activate();
	}
	auto advance_magnet_rise() const {
		this->m_flags.magnet_rise.advance_one_turn();
	}
	auto apply_mental_herb() const {
		this->m_flags.attracted = false;
		if (generation >= Generation::five) {
			this->m_flags.disable = {};
			this->m_flags.encore = {};
			this->m_flags.taunt = {};
			this->m_flags.is_tormented = false;
		}
	}
	auto minimize() const {
		this->m_flags.minimized = true;
		stages()[BoostableStat::eva] += BOUNDED_CONDITIONAL(generation <= Generation::four, 1_bi, 2_bi);
	}
	auto activate_mud_sport() const {
		this->m_flags.mud_sport = true;
	}
	auto try_to_give_nightmares() const {
		if (is_sleeping(this->status())) {
			this->m_flags.status.give_nightmares();
		}
	}
	auto partially_trap() const {
		this->m_flags.partial_trap.activate();
	}
	auto partial_trap_damage(Weather const weather) const {
		this->m_flags.partial_trap.damage(*this, weather);
	}
	auto activate_perish_song() const {
		this->m_flags.perish_song.activate();
	}
	auto perish_song_turn() const -> void {
		auto is_active = [&] {
			return this->m_flags.perish_song.is_active();
		};

		auto const was_active = is_active();
		this->m_flags.perish_song.advance_one_turn();
		if (was_active and !is_active()) {
			faint(this->m_pokemon);
		}
	}
	auto activate_power_trick() const {
		this->m_flags.power_trick_is_active = !this->m_flags.power_trick_is_active;
	}
	auto protect() const {
		this->m_flags.last_used_move.protect();
	}
	auto break_protect() const {
		this->m_flags.last_used_move.break_protect();
	}
	auto activate_rampage() const {
		this->m_flags.last_used_move.activate_rampage();
	}
	auto recharge() const -> bool {
		return this->m_flags.last_used_move.recharge();
	}
	auto use_recharge_move() const {
		this->m_flags.last_used_move.use_recharge_move();
	}

	auto set_status(StatusName const status_name, Weather const weather) const -> void {
		BOUNDED_ASSERT_OR_ASSUME(status_name != StatusName::clear);
		BOUNDED_ASSERT_OR_ASSUME(status_name != StatusName::rest);
		set_status_impl(status_name, weather);
	}

	auto rest(Weather const weather, bool const other_is_uproaring) const -> void {
		if (other_is_uproaring) {
			return;
		}
		if (generation >= Generation::three and is_sleeping(this->m_pokemon.status())) {
			return;
		}
		if (blocks_status(this->ability(), this->ability(), StatusName::rest, weather)) {
			return;
		}
		auto const active_hp = this->hp();
		if (active_hp.current() == active_hp.max() or healing_move_fails_in_generation_1(active_hp)) {
			return;
		}
		this->m_pokemon.set_hp(active_hp.max());
		set_status_impl(StatusName::rest, weather);
	}

	auto status_and_leech_seed_effects(OtherMutableActivePokemon<PokemonType> const other, Weather const weather, bool const uproar = false) const {
		this->m_flags.status.status_and_leech_seed_effects(*this, other, weather, uproar);
	}
	auto clear_status() const -> void {
		this->m_pokemon.set_status(StatusName::clear);
		this->m_flags.status.set(StatusName::clear);
	}
	auto advance_status_from_move(bool const clear_status) & {
		this->m_pokemon.advance_status_from_move(this->ability(), clear_status);
	}


	auto increment_stockpile() const -> void {
		bool const increased = this->m_flags.stockpile.increment();
		if (increased) {
			boost_defensive(stages(), 1_bi);
		}
	}
	auto release_stockpile() const {
		auto const stockpiled_stages = this->m_flags.stockpile.release();
		boost_defensive(stages(), -stockpiled_stages);
		return stockpiled_stages;
	}

	auto use_substitute(Weather const weather) const -> void {
		auto const can_use_substitute = this->hp().current() > this->hp().max() / 4_bi;
		if (!can_use_substitute) {
			return;
		}
		indirect_damage(weather, this->m_flags.substitute.create(this->hp().max()));
	}

	auto switch_in(Weather const weather) const {
		this->m_pokemon.mark_as_seen();
		this->m_flags.ability = this->m_pokemon.initial_ability();

		// The exact switch is irrelevant
		this->m_flags.last_used_move.successful_move(MoveName::Switch0);
		this->m_flags.types = PokemonTypes(generation, this->m_pokemon.species());
		if (generation <= Generation::two and this->m_pokemon.status().name() == StatusName::toxic) {
			this->m_pokemon.set_status(StatusName::poison);
		}
		this->m_flags.status.set(this->m_pokemon.status().name());
		if (this->item(weather) == Item::Berserk_Gene) {
			activate_berserk_gene(*this, weather);
		}
	}

	auto switch_out() const -> void {
		if (clears_status_on_switch(this->ability())) {
			clear_status();
		}
		// TODO: remove some of these when the foe switches, too
		if (!this->last_used_move().is_baton_passing()) {
			this->m_flags.aqua_ring = false;
			this->m_flags.confusion = {};
			this->m_flags.is_cursed = false;
			this->m_flags.embargo = {};
			if constexpr (generation <= Generation::four) {
				this->m_flags.fully_trapped = false;
			}
			this->m_flags.gastro_acid = false;
			this->m_flags.has_focused_energy = false;
			this->m_flags.ingrained = false;
			this->m_flags.leech_seeded = false;
			this->m_flags.magnet_rise = {};
			this->m_flags.perish_song = {};
			this->m_flags.power_trick_is_active = false;
			this->m_flags.stages = {};
			this->m_flags.substitute = {};
		}
		this->m_flags.attracted = false;
		this->m_flags.charged = false;
		this->m_flags.defense_curled = false;
		this->m_flags.disable = Disable{};
		this->m_flags.encore = {};
		this->m_flags.flash_fire = false;
		this->m_flags.flinched = false;
		if constexpr (generation >= Generation::five) {
			this->m_flags.fully_trapped = false;
		}
		this->m_flags.heal_block = {};
		this->m_flags.identified = false;
		this->m_flags.used_imprison = false;
		this->m_flags.last_used_move = {};
		this->m_flags.is_loafing_turn = true;
		this->m_flags.minimized = false;
		this->m_flags.me_first_is_active = false;
		this->m_flags.mud_sport = false;
		this->m_flags.partial_trap = {};
		this->m_flags.slow_start = {};
		this->m_flags.stockpile = {};
		this->m_flags.is_tormented = false;
		this->m_flags.unburdened = false;
		this->m_flags.water_sport = false;
		this->m_flags.taunt = {};
		this->m_flags.yawn = {};
	}


	auto taunt(Weather const weather) const {
		this->m_flags.taunt.activate();
		apply_own_mental_herb(*this, weather);
	}
	auto advance_taunt() const {
		this->m_flags.taunt.advance_one_turn();
	}
	auto torment(Weather const weather) const {
		this->m_flags.is_tormented = true;
		apply_own_mental_herb(*this, weather);
	}
	auto set_type(Type const type) const {
		this->m_flags.types = PokemonTypes(type);
	}
	auto use_uproar() const -> void {
		this->m_flags.last_used_move.use_uproar();
	}
	auto activate_water_sport() const {
		this->m_flags.water_sport = true;
	}
	auto hit_with_yawn() const {
		this->m_flags.yawn.activate();
	}
	auto try_to_activate_yawn(Weather const weather, bool const either_is_uproaring, bool const sleep_clause_activates) const -> void {
		auto is_active = [&] {
			return this->m_flags.yawn.is_active();
		};
		auto const was_active = is_active();
		if (!was_active) {
			return;
		}
		this->m_flags.yawn.advance_one_turn();
		// TODO: There are a lot of edge cases in different generations
		if (is_active()) {
			return;
		}
		if (yawn_can_apply(as_const(), weather, either_is_uproaring, sleep_clause_activates)) {
			set_status(StatusName::sleep, weather);
		}
	}

	// Returns whether the move hits
	auto use_vanish_move(Weather const weather) const -> bool {
		auto result = this->m_flags.last_used_move.use_vanish_move(this->item(weather));
		switch (result) {
			case VanishOutcome::vanishes: return false;
			case VanishOutcome::attacks: return true;
			case VanishOutcome::consumes_item: remove_item(); return true;
		}
	}

	auto use_bide(OtherMutableActivePokemon<PokemonType> target, Weather const weather) const -> void {
		if (auto const damage = this->m_flags.last_used_move.use_bide()) {
			change_hp(target, weather, -*damage * 2_bi);
		}
	}

	auto set_hp(Weather const weather, auto const new_hp) const -> void {
		this->m_pokemon.set_hp(new_hp);
		if (new_hp != 0_bi) {
			activate_pinch_item(weather);
		}
	}
	auto indirect_damage(Weather const weather, auto const damage) const -> void {
		change_hp(*this, weather, -damage);
		this->m_flags.damaged = true;
	}

	auto direct_damage(MoveName const move, any_mutable_active_pokemon auto user, Weather const weather, damage_type const damage) const -> HP::current_type {
		auto const interaction = substitute_interaction(generation, move);
		BOUNDED_ASSERT(!this->m_flags.substitute or interaction != Substitute::causes_failure);
		if (this->m_flags.substitute and interaction == Substitute::absorbs) {
			return this->m_flags.substitute.damage(damage);
		}
		auto const original_hp = this->hp().current();
		auto const block_ko = original_hp <= damage and handle_ko(move, weather);
		auto const applied_damage = block_ko ?
			bounded::assume_in_range<HP::current_type>(original_hp - 1_bi) :
			bounded::min(damage, original_hp);

		indirect_damage(weather, applied_damage);
		this->m_flags.direct_damage_received = applied_damage;
		this->m_flags.last_used_move.direct_damage(applied_damage);

		// TODO: Resolve ties properly
		if (this->last_used_move().is_destiny_bonded() and applied_damage == original_hp) {
			faint(user);
		}
		return applied_damage;
	}

	auto successfully_use_move(MoveName const move) const {
		this->m_flags.last_used_move.successful_move(move);
	}
	auto unsuccessfully_use_move(MoveName const move) const {
		this->m_flags.last_used_move.unsuccessful_move(move);
	}

private:
	auto handle_ko(MoveName const move, Weather const weather) const {
		if (cannot_ko(move) or this->last_used_move().is_enduring()) {
			return true;
		}
		auto const hp = this->hp();
		if (hp.current() != hp.max()) {
			return false;
		}
		if (generation >= Generation::five and this->ability() == Ability::Sturdy) {
			return true;
		}
		if (this->item(weather) == Item::Focus_Sash) {
			remove_item();
			return true;
		}
		return false;
	}

	auto activate_pinch_item(Weather const weather) const -> void {
		// TODO: Confusion damage does not activate healing berries in Generation 5+
		auto consume = [&] { remove_item(); };

		auto const current_hp = hp_ratio(this->m_pokemon);

		auto quarter_threshold = [&] {
			return BOUNDED_CONDITIONAL(this->ability() == Ability::Gluttony, rational(1_bi, 2_bi), rational(1_bi, 4_bi));
		};

		auto healing_berry = [&](auto const threshold, auto const amount) {
			if (current_hp > threshold) {
				return false;
			}
			consume();
			this->m_pokemon.set_hp(this->hp().current() + amount);
			return true;
		};

		auto confusion_berry = [&](SplitSpecialRegularStat const stat) {
			auto const amount = this->hp().max() / BOUNDED_CONDITIONAL(generation <= Generation::six, 8_bi, 2_bi);
			auto const threshold = generation <= Generation::six ? rational(1_bi, 2_bi) : quarter_threshold();
			auto const activated = healing_berry(threshold, amount);
			if (activated and lowers_stat(this->m_pokemon.nature(), stat)) {
				confuse(weather);
			}
		};

		auto stat_boost_berry = [&](BoostableStat const stat) {
			if (current_hp > quarter_threshold()) {
				return;
			}
			consume();
			stages()[stat] += 1_bi;
		};

		switch (this->item(weather)) {
			case Item::Aguav_Berry:
				confusion_berry(SplitSpecialRegularStat::spd);
				break;
			case Item::Apicot_Berry:
				stat_boost_berry(BoostableStat::spd);
				break;
			case Item::Berry:
			case Item::Oran_Berry:
				healing_berry(rational(1_bi, 2_bi), 10_bi);
				break;
			case Item::Berry_Juice:
				healing_berry(rational(1_bi, 2_bi), 20_bi);
				break;
			case Item::Custap_Berry:
				consume();
				break;
			case Item::Figy_Berry:
				confusion_berry(SplitSpecialRegularStat::atk);
				break;
			case Item::Ganlon_Berry:
				stat_boost_berry(BoostableStat::def);
				break;
			case Item::Gold_Berry:
				healing_berry(rational(1_bi, 2_bi), 30_bi);
				break;
			case Item::Iapapa_Berry:
				confusion_berry(SplitSpecialRegularStat::def);
				break;
			case Item::Lansat_Berry:
				consume();
				break;
			case Item::Liechi_Berry:
				stat_boost_berry(BoostableStat::atk);
				break;
			case Item::Mago_Berry:
				confusion_berry(SplitSpecialRegularStat::spe);
				break;
			case Item::Micle_Berry:
				consume();
				break;
			case Item::Petaya_Berry:
				stat_boost_berry(BoostableStat::spa);
				break;
			case Item::Salac_Berry:
				stat_boost_berry(BoostableStat::spe);
				break;
			case Item::Sitrus_Berry:
				healing_berry(
					rational(1_bi, 2_bi),
					BOUNDED_CONDITIONAL(generation <= Generation::three, 30_bi, this->hp().max() / 4_bi)
				);
				break;
			case Item::Starf_Berry:
				// TODO: Raise Atk, Def, SpA, SpD, or Spe +2
				consume();
				break;
			case Item::Wiki_Berry:
				confusion_berry(SplitSpecialRegularStat::spa);
				break;
			default:
				break;
		}
	}

	auto set_status_impl(StatusName const status_name, Weather const weather) const -> void {
		auto unconditional_set_status = [&](StatusName const unconditional_status) {
			this->m_pokemon.set_status(unconditional_status);
			this->m_flags.status.set(unconditional_status);
		};
		if (clears_status(this->item(weather), status_name)) {
			unconditional_set_status(StatusName::clear);
			remove_item();
		} else {
			unconditional_set_status(status_name);
		}
	}
};

template<any_active_pokemon PokemonType>
auto all_moves(PokemonType const pokemon, TeamSize const team_size) {
	return MoveContainer<generation_from<PokemonType>>(pokemon.regular_moves(), team_size);
}

auto change_hp(any_mutable_active_pokemon auto const pokemon, Weather const weather, bounded::bounded_integer auto const change) {
	pokemon.set_hp(weather, pokemon.hp().current() + change);
}

auto apply_white_herb(any_mutable_active_pokemon auto const pokemon) {
	for (auto & stage : pokemon.stages()) {
		if (stage < 0_bi) {
			stage = Stage(0_bi);
		}
	}
}

auto apply_own_white_herb(any_mutable_active_pokemon auto const pokemon, Weather const weather) {
	if (pokemon.item(weather) == Item::White_Herb) {
		apply_white_herb(pokemon);
		pokemon.remove_item();
	}
}

template<any_active_pokemon ActivePokemonType>
bool blocks_switching(Ability const ability, ActivePokemonType const switcher, Weather const weather) {
	constexpr auto generation = generation_from<ActivePokemonType>;
	auto ghost_immunity = [&] {
		return generation >= Generation::six and is_type(switcher, Type::Ghost);
	};
	switch (ability) {
		case Ability::Shadow_Tag:
			return (generation <= Generation::three or switcher.ability() != Ability::Shadow_Tag) and !ghost_immunity();
		case Ability::Arena_Trap:
			return grounded(switcher, weather) and !ghost_immunity();
		case Ability::Magnet_Pull:
			return is_type(switcher, Type::Steel) and !ghost_immunity();
		default:
			return false;
	}
}

template<any_mutable_active_pokemon PokemonType>
void activate_ability_on_switch(PokemonType const switcher, OtherMutableActivePokemon<PokemonType> const other, Weather & weather) {
	constexpr auto generation = generation_from<PokemonType>;
	auto const switcher_ability = switcher.ability();
	switch (switcher_ability) {
		case Ability::Download: {
			// Move is irrelevant here
			constexpr auto move = MoveName::Switch0;
			// TODO: Should not take into account items, abilities, or Wonder Room
			auto const defense = calculate_defense(other.as_const(), move, weather);
			auto const special_defense = calculate_special_defense(other.as_const(), switcher_ability, weather);
			auto const boosted_stat = defense >= special_defense ? BoostableStat::spa : BoostableStat::atk;
			switcher.stages()[boosted_stat] += 1_bi;
			break;
		}
		case Ability::Drizzle:
			weather.activate_rain_from_ability(generation, extends_rain(switcher.item(weather)));
			break;
		case Ability::Drought:
			weather.activate_sun_from_ability(generation, extends_sun(switcher.item(weather)));
			break;
		case Ability::Forecast:
			break;
		case Ability::Intimidate: {
			if (blocks_intimidate(generation, other.ability())) {
				break;
			}
			auto & attack = other.stages()[BoostableStat::atk];
			if (attack == numeric_traits::min_value<Stage::value_type>) {
				break;
			}
			attack -= 1_bi;
			auto & speed = other.stages()[BoostableStat::spe];
			if (other.item(weather) == Item::Adrenaline_Orb and speed != numeric_traits::max_value<Stage::value_type>) {
				speed += 1_bi;
				other.remove_item();
			}
			break;
		}
		case Ability::Sand_Stream:
			weather.activate_sand_from_ability(generation, extends_sand(switcher.item(weather)));
			break;
		case Ability::Snow_Warning:
			weather.activate_hail_from_ability(generation, extends_hail(switcher.item(weather)));
			break;
		case Ability::Trace: {
			auto const other_ability = other.ability();
			if (traceable(generation, other_ability)) {
				switcher.set_ability(other_ability);
			}
			break;
		}
		default:
			break;
	}
}

constexpr auto faint(any_mutable_active_pokemon auto const pokemon) -> void {
	pokemon.set_hp(Weather(), 0_bi);
}

} // namespace technicalmachine
