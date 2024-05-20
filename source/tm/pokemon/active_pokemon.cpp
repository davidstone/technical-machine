// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.pokemon.active_pokemon;

import tm.move.damage_type;
import tm.move.healing_move_fails_in_generation_1;
import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.active_status;
import tm.pokemon.any_pokemon;
import tm.pokemon.applied_damage;
import tm.pokemon.change_hp;
import tm.pokemon.confusion;
import tm.pokemon.disable;
import tm.pokemon.embargo;
import tm.pokemon.encore;
import tm.pokemon.faint;
import tm.pokemon.happiness;
import tm.pokemon.heal_block;
import tm.pokemon.hidden_power;
import tm.pokemon.hp_ratio;
import tm.pokemon.indirect_status_can_apply;
import tm.pokemon.is_type;
import tm.pokemon.last_used_move;
import tm.pokemon.level;
import tm.pokemon.magnet_rise;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.other_pokemon;
import tm.pokemon.partial_trap;
import tm.pokemon.perish_song;
import tm.pokemon.pokemon;
import tm.pokemon.slow_start;
import tm.pokemon.species;
import tm.pokemon.stockpile;
import tm.pokemon.substitute;
import tm.pokemon.taunt;
import tm.pokemon.yawn;

import tm.stat.current_hp;
import tm.stat.nature;
import tm.stat.stage;
import tm.stat.stat_names;

import tm.status.blocks_status;
import tm.status.clears_status;
import tm.status.status;
import tm.status.status_name;
import tm.status.toxic_resets_on_switch;

import tm.type.pokemon_types;
import tm.type.type;

import tm.ability;
import tm.compress;
import tm.environment;
import tm.exists_if;
import tm.gender;
import tm.generation;
import tm.heal;
import tm.held_item;
import tm.item;
import tm.rational;
import tm.visible_hp;

import bounded;
import containers;
import numeric_traits;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export template<typename PokemonType>
struct AnyActivePokemon;

template<typename PokemonType>
constexpr auto generation_from<AnyActivePokemon<PokemonType>> = generation_from<PokemonType>;

template<typename PokemonType>
constexpr auto is_any_active_pokemon<AnyActivePokemon<PokemonType>> = true;

export template<typename PokemonType>
struct AnyMutableActivePokemon;

template<typename PokemonType>
constexpr auto generation_from<AnyMutableActivePokemon<PokemonType>> = generation_from<PokemonType>;

template<typename PokemonType>
constexpr auto is_any_mutable_active_pokemon<AnyMutableActivePokemon<PokemonType>> = true;

export template<Generation generation>
using ActivePokemon = AnyActivePokemon<Pokemon<generation>>;

export template<Generation generation>
using MutableActivePokemon = AnyMutableActivePokemon<Pokemon<generation>>;

template<typename>
constexpr auto always_false = false;

template<typename T>
struct OtherActivePokemonImpl {
	static_assert(always_false<T>);
};
template<typename PokemonType>
struct OtherActivePokemonImpl<AnyActivePokemon<PokemonType>> {
	using type = AnyActivePokemon<OtherPokemon<PokemonType>>;
};
template<typename PokemonType>
struct OtherActivePokemonImpl<AnyMutableActivePokemon<PokemonType>> {
	using type = AnyActivePokemon<OtherPokemon<PokemonType>>;
};

export template<typename T>
using OtherActivePokemon = typename OtherActivePokemonImpl<T>::type;

template<typename T>
struct OtherMutableActivePokemonImpl {
	static_assert(always_false<T>);
};
template<typename PokemonType>
struct OtherMutableActivePokemonImpl<AnyActivePokemon<PokemonType>> {
	using type = AnyMutableActivePokemon<OtherPokemon<PokemonType>>;
};
template<typename PokemonType>
struct OtherMutableActivePokemonImpl<AnyMutableActivePokemon<PokemonType>> {
	using type = AnyMutableActivePokemon<OtherPokemon<PokemonType>>;
};

export template<typename T>
using OtherMutableActivePokemon = typename OtherMutableActivePokemonImpl<T>::type;

export template<Generation generation>
struct ActivePokemonFlags {
	constexpr auto reset_end_of_turn() & {
		last_used_move.reset_end_of_turn();
		damaged = false;
		direct_damage_received = 0_bi;
		flinched = false;
		me_first_is_active = false;
		is_loafing_turn = !is_loafing_turn;
	}

	friend auto operator==(ActivePokemonFlags, ActivePokemonFlags) -> bool = default;
	friend constexpr auto compress(ActivePokemonFlags const value) {
		auto const result = tv::tuple(
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
			return tv::tuple(compress_combine(result[0_bi], result[1_bi]));
		} else {
			return result;
		}
	}

private:
	template<typename>
	friend struct ActivePokemonImpl;
	template<typename>
	friend struct AnyMutableActivePokemon;

	// TODO: https://github.com/llvm/llvm-project/issues/61360
	// Disabled for https://github.com/llvm/llvm-project/issues/60711
	/*[[no_unique_address]]*/ ExistsIf<Ability, generation >= Generation::three, struct active_ability> ability{};
	Confusion confusion;
	Disable disable;
	/*[[no_unique_address]]*/ EmbargoCounter<generation> embargo;
	/*[[no_unique_address]]*/ EncoreCounter<generation> encore;
	/*[[no_unique_address]]*/ HealBlock<generation> heal_block;
	LastUsedMove last_used_move;
	/*[[no_unique_address]]*/ MagnetRise<generation> magnet_rise;
	Substitute substitute;
	PartialTrap partial_trap;
	/*[[no_unique_address]]*/ PerishSong<generation> perish_song;
	Stages stages;
	ActiveStatus status;
	/*[[no_unique_address]]*/ SlowStart<generation> slow_start;
	/*[[no_unique_address]]*/ Stockpile<generation> stockpile;
	/*[[no_unique_address]]*/ TauntCounter<generation> taunt;
	PokemonTypes types{Type::Typeless};
	/*[[no_unique_address]]*/ YawnCounter<generation> yawn;
	CurrentHP direct_damage_received = 0_bi;
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::four, struct aqua_ring> aqua_ring{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct attracted> attracted{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct charged> charged{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct cursed> is_cursed{};
	bool damaged = false;
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct defense_curled> defense_curled{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct flash_fire> flash_fire{};
	bool flinched = false;
	bool has_focused_energy = false;
	// Block, Mean Look, Spider Web
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct fully_trapped> fully_trapped{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::four, struct gastro_acid> gastro_acid{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct identified> identified{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct used_imprison> used_imprison{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct ingrained> ingrained{};
	bool leech_seeded = false;
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct is_loafing_turn> is_loafing_turn{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::four, struct me_first_is_active> me_first_is_active{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::two, struct minimized> minimized{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct mud_sport> mud_sport{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::four, struct power_trick_is_active> power_trick_is_active{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct is_tormented> is_tormented{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::four, struct unburdened> unburdened{};
	/*[[no_unique_address]]*/ BoolIf<generation >= Generation::three, struct water_sport> water_sport{};
};

template<typename PokemonType>
struct ActivePokemonImpl {
private:
	static constexpr auto generation = generation_from<PokemonType>;
	using FlagsRef = std::conditional_t<std::is_const_v<PokemonType>, ActivePokemonFlags<generation> const &, ActivePokemonFlags<generation> &>;

public:
	constexpr ActivePokemonImpl(PokemonType & pokemon, FlagsRef flags):
		m_pokemon(pokemon),
		m_flags(flags)
	{
	}
	ActivePokemonImpl(ActivePokemonImpl const & other) = default;
	ActivePokemonImpl(ActivePokemonImpl && other) = default;
	ActivePokemonImpl & operator=(ActivePokemonImpl const & other) = delete;
	ActivePokemonImpl & operator=(ActivePokemonImpl && other) = delete;

	constexpr auto regular_moves() const -> RegularMoves {
		return m_pokemon.regular_moves();
	}

	constexpr auto hp() const {
		return m_pokemon.hp();
	}
	constexpr auto visible_hp() const -> VisibleHP requires any_seen_pokemon<PokemonType> {
		return this->m_pokemon.visible_hp();
	}
	constexpr auto stat(SplitSpecialRegularStat const stat_name) const {
		return m_pokemon.stat(stat_name);
	}

	constexpr auto last_used_move() const -> LastUsedMove {
		return m_flags.last_used_move;
	}
	constexpr auto substitute() const -> Substitute const & {
		return m_flags.substitute;
	}

	constexpr auto ability() const -> Ability {
		if constexpr (exists<decltype(m_flags.ability)>) {
			return m_flags.ability;
		} else {
			return Ability::Honey_Gather;
		}
	}

	constexpr auto aqua_ring_is_active() const -> bool {
		return m_flags.aqua_ring;
	}

	constexpr auto charge_boosted(Type const move_type) const -> bool {
		return m_flags.charged and move_type == Type::Electric;
	}

	constexpr auto is_confused() const -> bool {
		return m_flags.confusion.is_active();
	}

	constexpr auto is_cursed() const -> bool {
		return m_flags.is_cursed;
	}

	constexpr auto defense_curled() const -> bool {
		return m_flags.defense_curled;
	}

	constexpr auto damaged() const -> bool {
		return m_flags.damaged;
	}

	constexpr auto direct_damage_received() const -> CurrentHP {
		return m_flags.direct_damage_received;
	}

	// Requires that move is actually one of this Pokemon's moves
	constexpr auto is_disabled(MoveName const move_name) const -> bool {
		return m_flags.disable.move_is_disabled(move_name);
	}

	constexpr auto is_encored() const -> bool {
		return m_flags.encore.is_active();
	}

	constexpr auto flash_fire_is_active() const -> bool {
		return m_flags.flash_fire;
	}

	constexpr auto flinched() const -> bool {
		return m_flags.flinched;
	}

	constexpr auto has_focused_energy() const -> bool {
		return m_flags.has_focused_energy;
	}

	constexpr auto gender() const -> Gender {
		return m_pokemon.gender();
	}

	constexpr auto happiness() const -> Happiness {
		return m_pokemon.happiness();
	}

	constexpr auto heal_block_is_active() const -> bool {
		return m_flags.heal_block.is_active();
	}

	constexpr auto hidden_power() const -> tv::optional<HiddenPower<generation>> {
		return m_pokemon.hidden_power();
	}

	constexpr auto used_imprison() const -> bool {
		return m_flags.used_imprison;
	}

	constexpr auto ingrained() const -> bool {
		return m_flags.ingrained;
	}

	constexpr auto item(Environment const environment) const -> Item {
		return m_pokemon.item(m_flags.embargo.is_active(), environment.magic_room());
	}
	constexpr auto unrestricted_item() const -> Item {
		return m_pokemon.item(false, false);
	}

	constexpr auto leech_seeded() const -> bool {
		return m_flags.leech_seeded;
	}

	constexpr auto level() const -> Level {
		return m_pokemon.level();
	}

	constexpr auto is_loafing() const -> bool {
		return ability() == Ability::Truant and m_flags.is_loafing_turn;
	}

	constexpr auto magnet_rise() const {
		return m_flags.magnet_rise;
	}

	constexpr auto me_first_is_active() const -> bool {
		return m_flags.me_first_is_active;
	}

	constexpr auto minimized() const -> bool {
		return m_flags.minimized;
	}

	constexpr auto power_trick_is_active() const -> bool {
		return m_flags.power_trick_is_active;
	}

	constexpr auto slow_start_is_active() const -> bool {
		return m_flags.slow_start.is_active();
	}

	constexpr auto species() const -> Species {
		return m_pokemon.species();
	}

	constexpr auto sport_is_active(Type const move_type) const -> bool {
		switch (move_type) {
			case Type::Electric: return m_flags.mud_sport;
			case Type::Fire: return m_flags.water_sport;
			default: return false;
		}
	}

	constexpr auto stages() const -> Stages const & {
		return m_flags.stages;
	}

	constexpr auto spit_up_power() const {
		return m_flags.stockpile.spit_up_power();
	}

	constexpr auto status() const -> Status {
		return m_pokemon.status();
	}

	constexpr auto fully_trapped() const -> bool {
		return m_flags.fully_trapped;
	}

	constexpr auto trapped() const -> bool {
		return m_flags.fully_trapped or ingrained() or m_flags.partial_trap.is_active();
	}

	constexpr auto is_tormented() const -> bool {
		return m_flags.is_tormented;
	}

	constexpr auto is_taunted() const -> bool {
		return m_flags.taunt.is_active();
	}

	constexpr auto types() const {
		return m_flags.types;
	}

	constexpr auto is_unburdened() const -> bool {
		return m_flags.unburdened;
	}

protected:
	PokemonType & m_pokemon;
	FlagsRef m_flags;
};

constexpr auto apply_own_mental_herb(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
	if (pokemon.item(environment) == Item::Mental_Herb) {
		pokemon.apply_mental_herb();
		pokemon.remove_item();
	}
}

// A reference to the currently active Pokemon
export template<typename PokemonType>
struct AnyActivePokemon : ActivePokemonImpl<PokemonType const> {
	constexpr AnyActivePokemon(PokemonType const & pokemon, ActivePokemonFlags<generation_from<PokemonType>> const & flags):
		ActivePokemonImpl<PokemonType const>(pokemon, flags)
	{
	}
};


constexpr auto activate_berserk_gene(any_mutable_active_pokemon auto pokemon, Environment const environment) -> void {
	pokemon.stages()[BoostableStat::atk] += 2_bi;
	// TODO: Berserk Gene causes 256-turn confusion, unless the Pokemon
	// switching out was confused.
	pokemon.confuse(environment);
	pokemon.remove_item();
}

constexpr auto yawn_can_apply(any_active_pokemon auto const target, Environment const environment, bool const either_is_uproaring, bool const sleep_clause_activates) {
	return
		!sleep_clause_activates and
		!either_is_uproaring and
		indirect_status_can_apply(StatusName::sleep, target, environment);
}

// A mutable reference to the currently active Pokemon
export template<typename PokemonType>
struct AnyMutableActivePokemon : ActivePokemonImpl<PokemonType> {
private:
	static constexpr auto generation = generation_from<PokemonType>;
public:
	constexpr AnyMutableActivePokemon(PokemonType & pokemon, ActivePokemonFlags<generation_from<PokemonType>> & flags):
		ActivePokemonImpl<PokemonType>(pokemon, flags)
	{
	}
	
	constexpr auto as_const() const {
		return AnyActivePokemon<PokemonType>(this->m_pokemon, this->m_flags);
	}

	constexpr auto add_move(Move const move) const -> void requires any_seen_pokemon<PokemonType> or any_known_pokemon<PokemonType> {
		return this->m_pokemon.add_move(move);
	}
	constexpr auto reduce_pp(MoveName const move_name, Environment const environment, bounded::bounded_integer auto const amount) const -> void {
		this->m_pokemon.reduce_pp(move_name, this->m_flags.embargo.is_active(), environment.magic_room(), amount);
	}

	constexpr auto stages() const -> Stages & {
		return this->m_flags.stages;
	}

	constexpr auto clear_field() const {
		this->m_flags.leech_seeded = false;
		this->m_flags.partial_trap = {};
	}

	constexpr auto set_ability(Ability const ability) const -> void {
		this->m_flags.ability = ability;
	}
	constexpr auto set_base_ability(Ability const ability) const -> void requires requires(PokemonType & pokemon) { pokemon.set_initial_ability(ability); } {
		this->m_pokemon.set_initial_ability(ability);
		set_ability(ability);
	}
	constexpr auto set_ability_to_base_ability() const -> void {
		set_ability(this->m_pokemon.initial_ability());
	}

	constexpr auto activate_aqua_ring() const {
		this->m_flags.aqua_ring = true;
	}
	constexpr auto attract(AnyMutableActivePokemon<OtherPokemon<PokemonType>> other, Environment const environment) const -> void {
		auto handle_item = [&] {
			switch (this->item(environment)) {
				case Item::Mental_Herb:
					apply_own_mental_herb(*this, environment);
					break;
				case Item::Destiny_Knot:
					remove_item();
					other.attract(*this, environment);
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

	constexpr auto charge() const {
		this->m_flags.charged = true;
	}
	constexpr auto use_charge_up_move() const -> void {
		this->m_flags.last_used_move.use_charge_up_move();
	}
	constexpr auto confuse(Environment const environment) const -> void {
		if (blocks_confusion(this->ability())) {
			return;
		}
		if (clears_confusion(this->item(environment))) {
			remove_item();
		} else {
			this->m_flags.confusion.activate();
		}
	}
	constexpr auto advance_confusion() const {
		this->m_flags.confusion.do_turn();
	}
	constexpr auto curse() const {
		this->m_flags.is_cursed = true;
	}
	constexpr auto defense_curl() const {
		this->m_flags.defense_curled = true;
	}
	constexpr auto disable(MoveName const move, Environment const environment) const {
		if (this->m_flags.disable.activate(move)) {
			apply_own_mental_herb(*this, environment);
		}
	}
	constexpr auto advance_disable() const {
		this->m_flags.disable.advance_one_turn();
	}
	constexpr auto activate_embargo() const {
		this->m_flags.embargo.activate();
	}
	constexpr auto advance_embargo() const {
		this->m_flags.embargo.advance_one_turn();
	}
	constexpr auto activate_encore(Environment const environment) const {
		this->m_flags.encore.activate();
		apply_own_mental_herb(*this, environment);
	}
	constexpr auto advance_encore() const {
		this->m_flags.encore.advance_one_turn();
	}
	constexpr auto activate_flash_fire() const {
		this->m_flags.flash_fire = true;
	}
	constexpr auto flinch() const {
		this->m_flags.flinched = true;
	}
	constexpr auto focus_energy() const {
		this->m_flags.has_focused_energy = true;
	}
	constexpr auto fully_trap() const {
		this->m_flags.fully_trapped = true;
	}
	constexpr auto activate_heal_block() const {
		this->m_flags.heal_block.activate();
	}
	constexpr auto advance_heal_block() const {
		this->m_flags.heal_block.advance_one_turn();
	}
	constexpr auto identify() const {
		this->m_flags.identified = true;
	}
	constexpr auto use_imprison() const {
		this->m_flags.used_imprison = true;
	}
	constexpr auto ingrain() const {
		this->m_flags.ingrained = true;
	}

	constexpr auto remove_item() const -> tv::optional<Item> {
		auto result = this->m_pokemon.remove_item();
		if (result) {
			this->m_flags.unburdened = true;
		}
		return result;
	}
	constexpr auto destroy_item() const -> bool {
		auto result = this->m_pokemon.destroy_item();
		if (result) {
			this->m_flags.unburdened = true;
		}
		return result;
	}
	constexpr auto recycle_item() const -> void {
		this->m_pokemon.recycle_item();
	}
	constexpr auto steal_item(AnyMutableActivePokemon<OtherPokemon<PokemonType>> other) const -> void {
		if (this->unrestricted_item() != Item::None) {
			return;
		}
		if (auto const other_item = other.remove_item()) {
			this->m_pokemon.set_item(*other_item);
		}
	}
	friend constexpr auto swap_items(AnyMutableActivePokemon<OtherPokemon<PokemonType>> user, AnyMutableActivePokemon<PokemonType> other) -> void;
	friend constexpr auto swap_items(AnyMutableActivePokemon<PokemonType> user, AnyMutableActivePokemon<OtherPokemon<PokemonType>> other) -> void {
		auto const user_item = user.unrestricted_item();
		auto const other_item = other.unrestricted_item();
		if (!cannot_be_lost(user_item) and !cannot_be_lost(other_item)) {
			user.m_pokemon.set_item(other_item);
			other.m_pokemon.set_item(user_item);
		}
	}

	constexpr auto hit_with_leech_seed() const {
		this->m_flags.leech_seeded = true;
	}
	constexpr auto advance_lock_in(bool const ending, Environment const environment) const {
		auto const confused = this->m_flags.last_used_move.advance_lock_in(ending);
		if (confused) {
			confuse(environment);
		}
	}
	constexpr auto activate_magnet_rise() const {
		this->m_flags.magnet_rise.activate();
	}
	constexpr auto advance_magnet_rise() const {
		this->m_flags.magnet_rise.advance_one_turn();
	}
	constexpr auto apply_mental_herb() const {
		this->m_flags.attracted = false;
		if (generation >= Generation::five) {
			this->m_flags.disable = {};
			this->m_flags.encore = {};
			this->m_flags.taunt = {};
			this->m_flags.is_tormented = false;
		}
	}
	constexpr auto minimize() const {
		this->m_flags.minimized = true;
		stages()[BoostableStat::eva] += BOUNDED_CONDITIONAL(generation <= Generation::four, 1_bi, 2_bi);
	}
	constexpr auto activate_mud_sport() const {
		this->m_flags.mud_sport = true;
	}
	constexpr auto try_to_give_nightmares() const {
		if (is_sleeping(this->status())) {
			this->m_flags.status.give_nightmares();
		}
	}
	constexpr auto partially_trap() const {
		this->m_flags.partial_trap.activate();
	}
	constexpr auto partial_trap_damage(Environment const environment) const {
		this->m_flags.partial_trap.damage(*this, environment);
	}
	constexpr auto activate_perish_song() const {
		this->m_flags.perish_song.activate();
	}
	constexpr auto perish_song_turn() const -> void {
		auto is_active = [&] {
			return this->m_flags.perish_song.is_active();
		};

		auto const was_active = is_active();
		this->m_flags.perish_song.advance_one_turn();
		if (was_active and !is_active()) {
			faint(*this);
		}
	}
	constexpr auto activate_power_trick() const {
		this->m_flags.power_trick_is_active = !this->m_flags.power_trick_is_active;
	}
	constexpr auto break_protect() const {
		this->m_flags.last_used_move.break_protect();
	}
	constexpr auto recharge() const -> bool {
		return this->m_flags.last_used_move.recharge();
	}
	constexpr auto use_recharge_move() const {
		this->m_flags.last_used_move.use_recharge_move();
	}

	constexpr auto set_status(StatusName const status_name, Environment const environment) const -> void {
		BOUNDED_ASSERT_OR_ASSUME(status_name != StatusName::clear);
		BOUNDED_ASSERT_OR_ASSUME(status_name != StatusName::rest);
		set_status_impl(status_name, environment);
	}

	constexpr auto rest(Environment const environment, bool const other_is_uproaring) const -> void {
		if (other_is_uproaring) {
			return;
		}
		if (generation >= Generation::three and is_sleeping(this->m_pokemon.status())) {
			return;
		}
		if (blocks_status(this->ability(), this->ability(), StatusName::rest, environment)) {
			return;
		}
		auto const active_hp = this->hp();
		if (active_hp.current() == active_hp.max() or healing_move_fails_in_generation_1(active_hp)) {
			return;
		}
		this->m_pokemon.set_hp(active_hp.max());
		set_status_impl(StatusName::rest, environment);
	}

	constexpr auto status_and_leech_seed_effects(AnyMutableActivePokemon<OtherPokemon<PokemonType>> const other, Environment const environment, bool const uproar = false) const {
		this->m_flags.status.status_and_leech_seed_effects(*this, other, environment, uproar);
	}
	constexpr auto clear_status() const -> void {
		this->m_pokemon.set_status(StatusName::clear);
		this->m_flags.status.set(StatusName::clear);
	}
	constexpr auto advance_status_from_move(bool const clear_status) & {
		this->m_pokemon.advance_status_from_move(this->ability(), clear_status);
	}


	constexpr auto increment_stockpile() const -> void {
		bool const increased = this->m_flags.stockpile.increment();
		if (increased) {
			boost_defensive(stages(), 1_bi);
		}
	}
	constexpr auto release_stockpile() const {
		auto const stockpiled_stages = this->m_flags.stockpile.release();
		boost_defensive(stages(), -stockpiled_stages);
		return stockpiled_stages;
	}

	constexpr auto use_substitute(Environment const environment) const -> void {
		auto const can_use_substitute = this->hp().current() > this->hp().max() / 4_bi;
		if (!can_use_substitute) {
			return;
		}
		indirect_damage(environment, this->m_flags.substitute.create(this->hp().max()));
	}

	constexpr auto switch_in(Environment const environment, bool const replacing_fainted_or_initial_switch) const {
		this->m_pokemon.mark_as_seen();
		this->m_flags.ability = this->m_pokemon.initial_ability();

		this->m_flags.types = PokemonTypes(generation, this->m_pokemon.species());
		if (toxic_resets_on_switch(generation) and this->m_pokemon.status().name() == StatusName::toxic) {
			this->m_pokemon.set_status(StatusName::poison);
		}
		this->m_flags.status.set(this->m_pokemon.status().name());
		if (!replacing_fainted_or_initial_switch) {
			this->m_flags.last_used_move.use_switch();
		}
		if (this->item(environment) == Item::Berserk_Gene) {
			activate_berserk_gene(*this, environment);
		}
	}

	constexpr auto switch_out() const -> void {
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


	constexpr auto taunt(Environment const environment) const {
		this->m_flags.taunt.activate();
		apply_own_mental_herb(*this, environment);
	}
	constexpr auto advance_taunt() const {
		this->m_flags.taunt.advance_one_turn();
	}
	constexpr auto torment(Environment const environment) const {
		this->m_flags.is_tormented = true;
		apply_own_mental_herb(*this, environment);
	}
	constexpr auto set_type(Type const type) const {
		this->m_flags.types = PokemonTypes(type);
	}
	constexpr auto activate_water_sport() const {
		this->m_flags.water_sport = true;
	}
	constexpr auto hit_with_yawn() const {
		this->m_flags.yawn.activate();
	}
	constexpr auto try_to_activate_yawn(Environment const environment, bool const either_is_uproaring, bool const sleep_clause_activates) const -> void {
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
		if (yawn_can_apply(as_const(), environment, either_is_uproaring, sleep_clause_activates)) {
			set_status(StatusName::sleep, environment);
		}
	}

	// Returns whether the move hits
	constexpr auto use_vanish_move(Environment const environment) const -> bool {
		auto result = this->m_flags.last_used_move.use_vanish_move(this->item(environment));
		switch (result) {
			case VanishOutcome::vanishes: return false;
			case VanishOutcome::attacks: return true;
			case VanishOutcome::consumes_item: remove_item(); return true;
		}
	}

	constexpr auto use_bide(AnyMutableActivePokemon<OtherPokemon<PokemonType>> target, Environment const environment) const -> void {
		if (auto const damage = this->m_flags.last_used_move.use_bide()) {
			change_hp(target, environment, -*damage * 2_bi);
		}
	}

	constexpr auto set_hp(Environment const environment, auto const new_hp) const -> void {
		this->m_pokemon.set_hp(new_hp);
		if (new_hp > 0_bi) {
			activate_pinch_item(environment);
		} else {
			this->m_flags.last_used_move.faint();
		}
	}
	constexpr auto indirect_damage(Environment const environment, auto const damage) const -> void {
		change_hp(*this, environment, -damage);
		this->m_flags.damaged = true;
	}

	constexpr auto direct_damage(MoveName const move, any_mutable_active_pokemon auto user, Environment const environment, damage_type const damage) const -> CurrentHP {
		auto const interaction = substitute_interaction(generation, move);
		BOUNDED_ASSERT(!this->m_flags.substitute or interaction != Substitute::causes_failure);
		if (this->m_flags.substitute and interaction == Substitute::absorbs) {
			return this->m_flags.substitute.damage(damage);
		}
		auto const applied = applied_damage(this->as_const(), move, damage, environment);
		indirect_damage(environment, applied.damage);
		this->m_flags.direct_damage_received = applied.damage;
		this->m_flags.last_used_move.direct_damage(applied.damage);
		if (applied.consume_item) {
			remove_item();
		}
		// TODO: Resolve ties properly
		if (this->last_used_move().is_destiny_bonded() and this->hp().current() == 0_bi) {
			faint(user);
		}
		return applied.damage;
	}

	constexpr auto successfully_use_move(MoveName const move) const {
		BOUNDED_ASSERT(move == MoveName::Struggle or containers::any_equal(this->regular_moves(), move));
		if (this->hp().current() != 0_bi) {
			this->m_flags.last_used_move.successful_move(move);
		}
	}
	constexpr auto unsuccessfully_use_move(MoveName const move) const {
		this->m_flags.last_used_move.unsuccessful_move(move);
	}
	constexpr auto hit_self() const {
		this->m_flags.last_used_move.hit_self();
	}

private:
	constexpr auto activate_pinch_item(Environment const environment) const -> void {
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
				confuse(environment);
			}
		};

		auto stat_boost_berry = [&](BoostableStat const stat) {
			if (current_hp > quarter_threshold()) {
				return;
			}
			consume();
			stages()[stat] += 1_bi;
		};

		switch (this->item(environment)) {
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

	constexpr auto set_status_impl(StatusName const status_name, Environment const environment) const -> void {
		auto unconditional_set_status = [&](StatusName const unconditional_status) {
			this->m_pokemon.set_status(unconditional_status);
			this->m_flags.status.set(unconditional_status);
		};
		if (clears_status(this->item(environment), status_name)) {
			unconditional_set_status(StatusName::clear);
			remove_item();
		} else {
			unconditional_set_status(status_name);
		}
	}
};

export constexpr auto apply_white_herb(any_mutable_active_pokemon auto const pokemon) {
	for (auto & stage : pokemon.stages()) {
		if (stage < 0_bi) {
			stage = Stage(0_bi);
		}
	}
}

constexpr auto apply_own_white_herb(any_mutable_active_pokemon auto const pokemon, Environment const environment) {
	if (pokemon.item(environment) == Item::White_Herb) {
		apply_white_herb(pokemon);
		pokemon.remove_item();
	}
}

} // namespace technicalmachine
