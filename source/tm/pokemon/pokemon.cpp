// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/initial_stat.hpp>

namespace technicalmachine {

template<Generation generation>
Pokemon<generation>::Pokemon(Species const species, Level const level, Gender const gender, Item const item, Ability const ability, CombinedStats<IVAndEV> stats, RegularMoves regular_moves_, Happiness const happiness):
    m_regular_moves(regular_moves_),
    m_stats(BaseStats(generation, species), stats, level),

    m_species(species),
    m_item(item),
    m_ability(ability),
    m_gender(gender),
    m_nature(stats.nature),

    m_level(level),

    m_happiness(happiness),

    // TODO: Make this none if there is no way to call Hidden Power
    // TODO: Use the IVs provided
    m_hidden_power([=]{
        constexpr auto dv = DV(15_bi);
        constexpr auto iv = IV(31_bi);
        return generation <= Generation::two ?
            HiddenPower(generation, DVs{dv, dv, dv, dv}) :
            HiddenPower(generation, IVs{iv, iv, iv, iv, iv, iv});
    }()),
    
    m_has_been_seen(false),

    m_ability_is_known(true),
    m_item_is_known(true),
    m_nature_is_known(true)
{
}

template<Generation generation>
Pokemon<generation>::Pokemon(Species const species, Level const level, Gender const gender) : 
    Pokemon::Pokemon(
        species,
        level,
        gender,
        Item::None,
        Ability::Honey_Gather,
        CombinedStats<IVAndEV>{
            Nature::Hardy,
            {default_iv(generation), EV(0_bi)},
            {default_iv(generation), EV(0_bi)},
            {default_iv(generation), EV(0_bi)},
            {default_iv(generation), EV(0_bi)},
            {default_iv(generation), EV(0_bi)},
            {default_iv(generation), EV(0_bi)},
        },
        RegularMoves(),
        Happiness()
    )
{
    m_ability_is_known = false;
    m_item_is_known = false;
    m_nature_is_known = false;
}

template<Generation generation>
auto Pokemon<generation>::set_ev(PermanentStat const stat_name, IV const iv, EV const ev) -> void {
    auto const base_stats = BaseStats(generation, species());
    if (stat_name == PermanentStat::hp) {
        m_stats.hp() = HP(base_stats, level(), iv, ev);
    } else {
        auto const regular = RegularStat(stat_name);
        auto & stat = m_stats[regular];
        stat = initial_stat(regular, base_stats[regular], nature(), iv, ev, level());
    }
}

template struct Pokemon<Generation::one>;
template struct Pokemon<Generation::two>;
template struct Pokemon<Generation::three>;
template struct Pokemon<Generation::four>;
template struct Pokemon<Generation::five>;
template struct Pokemon<Generation::six>;
template struct Pokemon<Generation::seven>;
template struct Pokemon<Generation::eight>;

} // namespace technicalmachine