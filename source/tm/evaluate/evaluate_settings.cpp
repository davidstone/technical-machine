export module tm.evaluate.evaluate_settings;

import bounded;

namespace technicalmachine {

export struct EvaluateSettings {
	// Arbitrary values
	using value_type = bounded::integer<-4096, 4096>;

	value_type hp;
	value_type hidden;
	value_type spikes;
	value_type stealth_rock;
	value_type toxic_spikes;
};

} // namespace technicalmachine
