# Team Building

## Predicting the Foe's Teams

Technical Machine's team building algorithm works in a few steps. It begins by selecting the most likely remaining Pokemon on the team given what it has already seen. It then adds items / moves to each Pokemon. Finally, it creates an optimized EV spread.

### Predicting the Pokemon and moves

Technical Machine predicts the foe's team using the usage stats. The first step is to simply look at usage stats overall, and that is the initial estimate. This estimate is then refined using various multipliers.

Note: Lead stats are currently not used for the latest version of the team builder, but it's expected to be added back in for versions without team preview. This would allow us to remove the lead usage from the general stats and get a non-lead multiplier. For instance, if Pidgey is used 100 times total, and is used 40 times as a lead, that means if I don't see it as a lead, it actually has only 60% of the usages that the overall stats suggest.

The next multiplier comes from the teammate stats. As an example, I will use a particular point in time in Generation 4. Machamp is 12.98% of all Pokemon used. If I have seen Celebi, however, then it is used 16.9% of the time, or about 1.3 times as much. If I then see a Gengar, Machamp is only used .92 times as much as you might expect Machamp to be used alone. Combining all of these gives 12.98% * 1.3 * .92 = 15.54% of all Pokemon used with Celebi and Gengar are Machamp. After taking all of these multipliers into account, I find which Pokemon is the most likely to be used, and assume that Pokemon is next on the team. I then see which Pokemon is made most likely by the inclusion of that Pokemon, and continue repeating this process until the entire team has been predicted.

In versions of Pokemon with team reveal, this is much less important, as all 6 Pokemon are known. However, there are variants with team reveal in which players select a subset of the visible 6, and therefore a strong player should be able to make use of this information to select the best subset. Even if all Pokemon are brought into battle (see 6, choose 6), there is still the issue of determining which Pokemon should lead.

Moves factor into this in the same way. If we have seen a Pikachu with Surf, that tells us the likelihood that there is a Tyranitar with Crunch (or any Tyranitar).

### Predicting the Foe's Item / Ability

Abilities and Items are currently used only as outputs of the predictor, not inputs. That is to say, after predicting the Pokemon and its moves, it selects the most likely item / ability for that moveset. Nothing else is predicted based on the item or ability chosen.

There are certain situations where Technical Machine can use evidence in the battle to update its predictions. Technical Machine is currently limited to things that give an obvious message, such as Leftovers or Black Sludge. Technical Machine is not yet smart enough to see that the foe has changed moves without switching and thus cannot be using a choice item, nor is it smart enough to calculate damage and see that they must be using a Choice Band. I actually have written a "reverse damage calculator" that could do calculations like that, including finding exact EVs, but it takes far too long to calculate to be useful in a battle situation.

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

Technical Machine randomly divides all remaining EVs among each stat, weighted by the amount of EVs already in this stat. In the future, Technical Machine will generate a distribution of foe Speeds actually seen by each Pokemon and weight its Speed by how often that will let it outspeed something. It will also determine initial EVs by the moveset it's using.

## Generating a New Team

If a team prediction algorithm stats with nothing, it can be used to generate a team. Unlike with the foe prediction, however, it selects each option randomly weighted by its probability, rather than always selecting the most likely outcome. This lets Technical Machine generate an unlimited number of unique teams, weighted by their usage by good players

Technical Machine supports battling with one specific team, every team in a given directory (weighted equally), or randomly generated teams.

### Examples

The following are a few randomly selected predicted teams using stats from early 2020. These stats do not have access to users' ratings, and thus are more likely to include bad teams than if they could be weighted by ratings.

#### Generation 1

<pre>
Chansey
- Soft-Boiled
- Ice Beam
- Thunder Wave
- Thunderbolt
Tauros
- Blizzard
- Body Slam
- Hyper Beam
- Earthquake
Rhydon
- Rock Slide
- Body Slam
- Earthquake
- Submission
Exeggutor
- Psychic
- Sleep Powder
- Stun Spore
- Explosion
Jolteon
- Double Kick
- Thunderbolt
- Pin Missile
- Thunder Wave
Snorlax
- Body Slam
- Counter
- Amnesia
- Rest
</pre>

<pre>
Alakazam
- Thunder Wave
- Seismic Toss
- Psychic
- Recover
Jynx
- Blizzard
- Psychic
- Counter
- Lovely Kiss
Tauros
- Body Slam
- Hyper Beam
- Blizzard
- Earthquake
Chansey
- Soft-Boiled
- Ice Beam
- Thunderbolt
- Thunder Wave
Starmie
- Thunderbolt
- Blizzard
- Recover
- Thunder Wave
Snorlax
- Self-Destruct
- Earthquake
- Body Slam
- Counter
</pre>
