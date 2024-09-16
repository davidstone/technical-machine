# Action Prediction

Technical Machine uses usage stats to estimate how likely each move is to be used in each matchup. If 26% of all Tauros use Body Slam when up against a Starmie (conditional on Tauros knowing Body Slam), then Body Slam has a usage weight of 0.26. The probability of a Pokemon using a move in a matchup just becomes how much the weight for that move is out of the share of the weights of all moves.

Switching is handled specially because it is in some ways two decisions: switching the current Pokemon out and switching a new Pokemon in. Switching the current Pokemon out is handled like any move -- you end up with just a regular weight for how often the Pokemon is switched out of a matchup (a number between 0 and 1). When deciding what to switch in, each Pokemon has a multiplier for how likely this Pokemon is to be brought in vs the average. A value of 1 indicates that the Pokemon is no more or less likely to be brought in than expected by chance (on teams with 6 Pokemon, if it's brought out 1/5 times that a switch occurs, it would have a multiplier of 1, and if it's brought out 5/5 times it has a multplier of 5). In a matchup of Tauros vs Starmie, therefore, the weight for the selection "Switch to Pikachu" is the weight for Tauros switching out multiplied by Pikachu's multiplier for switching into Starmie.

This behavior ensures that switching is handled properly.

If the chance of switching into a particular Pokemon were just a relative probability and that were multiplied by the chance of switching out, that could only ever decrease the probability. Consider a Generation 3 example. Celebi might have a 40% chance of switching out vs Skarmory, but if the Celebi team has Magneton you might expect a greater than 40% chance of Celebi switching to Magneton. By combining the "how likely is Celebi to switch out of Skarmory" probability with the "How much more likely are you to switch to Magneton than expected by chance if the foe has out Skarmory", you appropriately weight a Magneton switch higher than Celebi switches in general.

We still care about the chance of the Pokemon switching out of a match-up, not just the chance of it switching in, or we could end up with a different pathological behvaior. Consider a Generation 2 example. Assume a team has Zapdos and Raikou -- Zapdos isn't going to switch out of Skarmory just because Raikou is a very likely Pokemon to switch into Skarmory. Here, the very low probability of Zapdos switching out of Skarmory prevents even a very high Raikou switch in multiplier from overriding the most likely outcome: Thunder.

## Evaluating the effectiveness of action prediction

Each selection is given a probability, and then this prediction is scored against historical battles by seeing what the player actually did. To rate the evaluation, we use a multi-category Brier score. This score ranges from 0 to 2, with 0 being a perfect prediction record (the actual move was predicted with 100% probability every time) and 2 being a perfectly wrong record (the actual move was predicted with 0% probability every time).

Another scoring rule is to use a log score (the Brier score is a quadratic score). A log scoring rule is appropriate when relative error is important, a quadratic scoring rule is appropriate when absolute error is important. In other words, if an event has a 1/1000 chance of occuring, is it better to call that 1% or 0%? A log scoring rule will give the overall prediction a score of -infinity if a 0% prediction ever occurs (it's infinitely surprising for an impossible event to occur), so under such a system 1% is a better prediction. A quadric scoring rule favors the 0% estimate.

Given the application is to feed into a Pokemon AI that wants to decide what to do based on this, it seems that 0% is a better estimate (because the resulting difference in play vs. the true value would most likely be smaller), and thus a Brier score seems a more appropriate scoring rule.

The statistical method outlined above has a Brier score of 0.753736.

## Other approaches considered

All Brier scores cited are as measured by running the prediction algorithm against a set of 17195 generation 1 OU battles, evaluating both sides of the battle.

### Random

Every possible action is equally likely to occur, including switches. This ends up predicting switches very often (at the start of the game, a switch is typically 5/9 of all possibilities). This gave a Brier score of 0.832831.

### Random(switch_probability)

Rather than treating switches as any other action, assume the player switches with a particular probability (except when switches are impossible or forced), and randomly select from available actions at those probabilities. If the switch probability is 0%, the Brier score is 0.808325. If the switch probability is 100%, the Brier score is 1.13662. The Brier score is minimized with a switch probability of 16.4%, which gives a Brier score of 0.795287.

### Expectiminimax + silly math

The original action prediction assumed the foe was playing an expectiminimax strategy, and it turned the scores into probabilities with some arbitrary math. This did a very poor job of predicting actual player behavior and it was quite slow. The best depth of search was 1,0 (look 1 turn ahead and do not consider 1v1 matchups before evaluating), and this gave a Brier score of 0.764263.

This is slightly better than random selection, even with ideal switch weighting. However, due to how slow it is, this prediction algorithm leaves less time for evaluation and ends up not improving overall play.

### Max damage

Assume the player will always select the move that does the most damage. They never switch, and they never use non-damaging moves (unless forced). This has a Brier score of 0.662987.

This is the best Brier score so far for a very simple algorithm, but it is likely this would actually lead to worse play than the other options. This is because it assumes the opponent uses a move with 100% probability, meaning Technical Machine will not even consider that they might do something else and thus makes Technical Machine predictable in a highly exploitable way.

### Rule based

This prediction strategy would look something like the in-game "good AI". It is essentially a refinement of the max damage player to also sometimes use non-damaging moves (Something like "If I have an entry hazard move and it isn't active, use it more often"). It would also switch to a Pokemon if that would let it avoid damage and do more damage. This could end up improving on max damage a fair amount, but has not yet been implemented. The downside is that it ends up being more complicated, requires human expertise to determine the rules and how important each one is, and thus does not generalize to other generations, tiers, and metagames.