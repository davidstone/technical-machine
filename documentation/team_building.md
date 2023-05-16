# Team Building

## Predicting the Foe's Teams

Technical Machine's team building algorithm works in a few steps. It begins by selecting the most likely remaining Pokemon on the team given what it has already seen. It then adds items / moves to each Pokemon. Finally, it creates an optimized EV spread.

### Predicting the Pokemon and moves

Technical Machine predicts the foe's team using the usage stats. The first step is to simply look at usage stats overall, and that is the initial estimate. This estimate is then refined using various multipliers.

Note: Lead stats are currently not used for the latest version of the team builder, but it's expected to be added back in for versions without team preview. The first multiplier for the Pokemon is the non-lead multiplier. Whenever my AI has seen a team, It has always seen at least one Pokemon: the lead. We have lead stats for Pokemon, and so we can remove that from the general stats and get a non-lead multiplier. For instance, if Pidgey is used 100 times total, and is used 40 times as a lead, that means if I don't see it as a lead, it actually only has 60% of the usages that the overall stats suggest.

The next multiplier comes from the teammate stats. As an example, I will use a particular point in time in Generation 4. Machamp is 12.98% of all Pokemon used. If I have seen Celebi, however, then it is used 16.9% of the time, or about 1.3 times as much. If I then see a Gengar, Machamp is only used .92 times as much as you might expect Machamp to be used alone. Combining all of these gives 12.98% * 1.3 * .92 = 15.54% of all Pokemon used with Celebi and Gengar are Machamp. After taking all of these multipliers into account, I find which Pokemon is the most likely to be used, and assume that Pokemon is next on the team. I then see which Pokemon is made most likely by the inclusion of that Pokemon, and continue repeating this process until the entire team has been predicted.

In versions of Pokemon with team reveal, this is much less important, as all 6 Pokemon are known. However, there are variants with team reveal in which players select a subset of the visible 6, and therefore a strong player should be able to make use of this information to select the best subset. Even if all Pokemon are brought into battle (see 6, choose 6), there is still the issue of determining which Pokemon should lead.

Moves factor into this in the same way. If we have seen a Pikachu with Surf, that tells us the likelihood that there is a Tyranitar with Crunch (or any Tyranitar).

### Predicting the Foe's Item / Ability

Abilities and Items are currently used only as outputs of the predictor, not inputs. That is to say, after predicting the Pokemon and its moves, it selects the most likely item / ability for that moveset. Nothing else is predicted based on the item or ability chosen.

There are certain situations where Techincal Machine can use evidence in the battle to update its predictions. Technical Machine is currently limited to things that give an obvious message, such as Leftovers or Black Sludge. Technical Machine is not yet smart enough to see that the foe has changed moves without switching and thus cannot be using a choice item, nor is it smart enough to calculate damage and see that they must be using a Choice Band. I actually have written a "reverse damage calculator" that could do calculations like that, including finding exact EVs, but it takes far too long to calculate to be useful in a battle situation.

### Predicting the Foe's EVs / Nature

This is the one part of the team builder that Technical Machine relies more on mathematical analysis than usage stats. Technical Machine has two components for this:

1. Optimize current EVs / Nature to remove any waste
2. Pad out any remaining EVs

Technical Machine runs those two steps repeatedly until the optimizing step can no longer find any waste to remove.

#### Optimize current EVs / Nature

* Remove any offensive EVs in a stat that the Pokemon doesn't use. This has the slight problem of giving Attack EVs to Rapid Spin Pokemon and other similar issues. Technical Machine doesn't realize that even though the move does damage, it's not used for attacking.
* Move around HP, Defense, and Special Defense EVs to reduce waste. As an example, a Pokemon with 600 HP and 100 in each defense is better off having 400 HP and 200 in each defense. It will take all hits better. Technical Machine finds the solution that uses the fewest defensive EVs to keep the ability to take physical and special hits the same. This doesn't take into account passing Substitute or Wish, nor does it consider moves like Leech Seed and Pain Split.
* Find multiple ways to keep the Speed the same.

In each of these steps, Technical Machine performs the same process with every nature. It then combines the results from each step, added up the EVs required for each nature. It then picks the EV / nature combination that uses the fewest EVs.

#### Pad out remaining EVs

Technical Machine currently randomly divides all remaining EVs among each stat. However, my plans for the future are to weight this random padding by how often that particular EV is used by that Pokemon. In other words, a Pokemon that generally invests a lot in Speed is more likely to have its Speed EVs padded than a Pokemon that rarely puts a single EV into Speed. This will possibly also look into the overall speed distribution of the tier.

## Generating a New Team

Technical Machine has two methods of making a team. The first is essentially a team thief. If the AI loses against someone, it stores their team. It uses whatever it has seen as the base of the team, and then it runs their team through the team predictor / EV optimizer to fill in the gaps / improve it.

The other method is purely algorithmic. It basically predicts a "foe" team starting from no information, and uses that as its team. Unlike with the foe prediction, however, it selects each option randomly weighted by its probability, rather than always selecting the most likely outcome.

Technical Machine currently supports battling with one specific team, every team in a given directory (weighted equally), or randomly generated teams.

### Examples

The following are a few randomly selected predicted teams using stats from early 2020. These stats do not have access to users' ratings, and thus are more likely to include bad teams than if they could be weighted by ratings. It also does not properly account for Hidden Power type. These should show off both the strengths and current limitations.

#### Generation 1

<pre>
Starmie (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 0 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Thunder Wave
	- Psychic
	- Ice Beam
	- Surf
Dugtrio (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Sand Attack
	- Earthquake
	- Substitute
	- Body Slam
Articuno (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Agility
	- Ice Beam
	- Blizzard
	- Hyper Beam
Exeggutor (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Rest
	- Psychic
	- Sleep Powder
	- Double-Edge
Zapdos (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Rest
	- Thunderbolt
	- Drill Peck
	- Thunder Wave
Golem (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Rock Slide
	- Explosion
	- Earthquake
	- Body Slam
</pre>

<pre>
Exeggutor (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Stun Spore
	- Sleep Powder
	- Mega Drain
	- Explosion
Starmie (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 0 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Surf
	- Thunderbolt
	- Recover
	- Thunder Wave
Chansey (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Seismic Toss
	- Reflect
	- Thunder Wave
	- Soft-Boiled
Rhydon (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Body Slam
	- Rock Slide
	- Earthquake
	- Substitute
Snorlax (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Reflect
	- Rest
	- Self-Destruct
	- Hyper Beam
Jynx (100.0% HP) @ None
	Ability: Honey Gather
	DVs: 0 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Lovely Kiss
	- Rest
	- Blizzard
	- Psychic
</pre>

#### Generation 2

<pre>
Snorlax (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Lovely Kiss
	- Self-Destruct
	- Fire Blast
	- Body Slam
Exeggutor (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 11 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Stun Spore
	- Hidden Power
	- Psychic
	- Giga Drain
Charizard (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Rock Slide
	- Belly Drum
	- Earthquake
	- Fire Blast
Cloyster (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Hidden Power
	- Spikes
	- Explosion
	- Ice Beam
Raikou (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 11 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Sleep Talk
	- Thunder
	- Rest
	- Hidden Power
Steelix (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Iron Tail
	- Roar
	- Explosion
	- Curse
</pre>

<pre>
Marowak (100.0% HP) @ Thick Club
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Fire Blast
	- Swords Dance
	- Earthquake
	- Rock Slide
Snorlax (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Body Slam
	- Curse
	- Rest
	- Earthquake
Starmie (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Recover
	- Surf
	- Rapid Spin
	- Psychic
Dragonite (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Thunder Wave
	- Dynamic Punch
	- Ice Beam
	- Thunderbolt
Tyranitar (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 15 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 252 Atk / 252 Def / 252 Spe / 252 Spc
	- Dynamic Punch
	- Rock Slide
	- Roar
	- Pursuit
Umbreon (100.0% HP) @ Leftovers
	Ability: Honey Gather
	DVs: 0 Atk / 15 Def / 15 Spe / 15 Spc
	EVs: 252 HP / 0 Atk / 252 Def / 252 Spe / 252 Spc
	- Pursuit
	- Rest
	- Charm
	- Sleep Talk
</pre>

#### Generation 8

<pre>
Dragapult (100.0% HP) @ Power Herb
	Ability: Clear Body
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 44 Atk / 48 Def / 0 SpA / 68 SpD / 96 Spe
	- Sucker Punch
	- Dragon Dance
	- Dragon Darts
	- Phantom Force
Hitmontop (100.0% HP) @ Life Orb
	Ability: Intimidate
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 56 Atk / 52 Def / 0 SpA / 44 SpD / 104 Spe
	- Close Combat
	- Mach Punch
	- Fake Out
	- Rapid Spin
Bisharp (100.0% HP) @ Black Glasses
	Ability: Defiant
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 228 HP / 48 Atk / 0 Def / 0 SpA / 80 SpD / 152 Spe
	- Throat Chop
	- Swords Dance
	- Iron Head
	- Psycho Cut
Charizard (100.0% HP) @ Choice Scarf
	Ability: Solar Power
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 48 Def / 36 SpA / 44 SpD / 128 Spe
	- Dragon Pulse
	- Air Slash
	- Solar Beam
	- Fire Blast
Araquanid (100.0% HP) @ Choice Band
	Ability: Water Bubble
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 80 Atk / 32 Def / 0 SpA / 0 SpD / 144 Spe
	- Sticky Web
	- Lunge
	- Liquidation
	- Leech Life
Obstagoon (100.0% HP) @ Leftovers
	Ability: Guts
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 36 Atk / 84 Def / 0 SpA / 44 SpD / 92 Spe
	- Knock Off
	- Facade
	- Bulk Up
	- Close Combat
</pre>

<pre>
Clefable (100.0% HP) @ Life Orb
	Ability: Magic Guard
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 68 Def / 32 SpA / 28 SpD / 128 Spe
	- Flamethrower
	- Thunderbolt
	- Moonblast
	- Moonlight
Dragapult (100.0% HP) @ Choice Scarf
	Ability: Infiltrator
	Nature: Mild
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 240 HP / 76 Atk / 92 Def / 0 SpA / 16 SpD / 84 Spe
	- U-turn
	- Draco Meteor
	- Sucker Punch
	- Fire Blast
Corsola-Galar (100.0% HP) @ Eviolite
	Ability: Weak Armor
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 20 Def / 44 SpA / 44 SpD / 148 Spe
	- Strength Sap
	- Night Shade
	- Stealth Rock
	- Will-O-Wisp
Togekiss (100.0% HP) @ Leftovers
	Ability: Serene Grace
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 36 Def / 44 SpA / 16 SpD / 160 Spe
	- Nasty Plot
	- Thunder Wave
	- Psychic
	- Air Slash
Darmanitan-Galar (100.0% HP) @ Choice Band
	Ability: Gorilla Tactics
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 68 HP / 16 Atk / 152 Def / 0 SpA / 148 SpD / 124 Spe
	- U-turn
	- Icicle Crash
	- Flare Blitz
	- Earthquake
Corviknight (100.0% HP) @ Leftovers
	Ability: Pressure
	Nature: Adamant
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 56 Atk / 44 Def / 0 SpA / 44 SpD / 112 Spe
	- Defog
	- Taunt
	- Bulk Up
	- Brave Bird
</pre>

<pre>
Shuckle (100.0% HP) @ Rocky Helmet
	Ability: Sturdy
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 216 HP / 0 Atk / 0 Def / 128 SpA / 0 SpD / 164 Spe
	- Final Gambit
	- Encore
	- Stealth Rock
	- Sticky Web
Kyurem (100.0% HP) @ Choice Specs
	Ability: Pressure
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 56 Def / 16 SpA / 36 SpD / 148 Spe
	- Draco Meteor
	- Flash Cannon
	- Ice Beam
	- Earth Power
Aegislash (100.0% HP) @ Leftovers
	Ability: Stance Change
	Nature: Timid
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 40 Def / 148 SpA / 8 SpD / 60 Spe
	- Toxic
	- King's Shield
	- Substitute
	- Shadow Ball
Zeraora (100.0% HP) @ Life Orb
	Ability: Volt Absorb
	Nature: Jolly
	IVs: 31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 120 Atk / 60 Def / 0 SpA / 40 SpD / 36 Spe
	- Plasma Fists
	- Bulk Up
	- Play Rough
	- Close Combat
Togekiss (100.0% HP) @ Leftovers
	Ability: Serene Grace
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 252 HP / 0 Atk / 84 Def / 24 SpA / 32 SpD / 116 Spe
	- Substitute
	- Air Slash
	- Nasty Plot
	- Flamethrower
Necrozma (100.0% HP) @ Lum Berry
	Ability: Prism Armor
	Nature: Modest
	IVs: 31 HP / 0 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe
	EVs: 248 HP / 0 Atk / 28 Def / 32 SpA / 104 SpD / 96 Spe
	- Heat Wave
	- Photon Geyser
	- Autotomize
	- Calm Mind
</pre>
