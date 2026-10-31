export module tm.team_predictor.ev_optimizer.nature_effects;

import tm.stat.nature_effect;

export import containers;

namespace technicalmachine {

export struct NatureEffects {
	NatureEffect physical;
	NatureEffect special;
};
export constexpr auto nature_effects = containers::array({
	NatureEffects(NatureEffect::positive, NatureEffect::neutral),
	NatureEffects(NatureEffect::positive, NatureEffect::negative),
	NatureEffects(NatureEffect::neutral, NatureEffect::positive),
	NatureEffects(NatureEffect::neutral, NatureEffect::neutral),
	NatureEffects(NatureEffect::neutral, NatureEffect::negative),
	NatureEffects(NatureEffect::negative, NatureEffect::positive),
	NatureEffects(NatureEffect::negative, NatureEffect::neutral),
});

} // namespace technicalmachine
