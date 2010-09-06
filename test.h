#ifndef TEST_H_
#define TEST_H_

#include "team.h"
#include "pokemon.h"
#include "move.h"

void test (const teams &team) {
	if (team.me)
		std::cout << "---------AI---------\n";
	else
		std::cout << "---------Foe---------\n";
	std::cout << "Light Screen: " << static_cast<int> (team.light_screen) << '\n';
	std::cout << "Lucky Chant: " << static_cast<int> (team.lucky_chant) << '\n';
	std::cout << "Mist: " << static_cast<int> (team.mist) << '\n';
	std::cout << "Reflect: " << static_cast<int> (team.reflect) << '\n';
	std::cout << "Safeguard: " << static_cast<int> (team.safeguard) << '\n';
	std::cout << "Tailwind: " << static_cast<int> (team.tailwind) << '\n';
	std::cout << "Stealth Rock: " << team.stealth_rock << '\n';
	std::cout << "Spikes: " << static_cast<int> (team.spikes) << '\n';
	std::cout << "Toxic Spikes: " << static_cast<int> (team.toxic_spikes) << '\n';
	std::cout << "Wish: " << static_cast<int> (team.wish) << '\n';
	std::cout << "Doom Desire / Future Sight: " << static_cast<int> (team.counter) << '\n';
	std::cout << "Me: " << team.me << '\n';
//	for (std::vector<pokemon>::iterator team.active = team.member.begin(); team.active != team.member.end(); ++team.active) {
		std::cout << "Pokemon: " << team.active->pokemon << '\n';
		std::cout << "Ability: " << team.active->ability << '\n';
		std::cout << "Aqua Ring: " << team.active->aqua_ring << '\n';
		std::cout << "Charge: " << team.active->charge << '\n';
		std::cout << "Curse: " << team.active->curse << '\n';
		std::cout << "Damaged: " << team.active->damaged << '\n';
		std::cout << "Defense Curl: " << team.active->defense_curl << '\n';
		std::cout << "Flash Fire: " << team.active->ff << '\n';
		std::cout << "Flinch: " << team.active->flinch << '\n';
		std::cout << "Imprison: " << team.active->imprison << '\n';
		std::cout << "Ingrain: " << team.active->ingrain << '\n';
		std::cout << "Item Recycle: " << team.active->item_recycle << '\n';
		std::cout << "Item Unburden: " << team.active->item_unburden << '\n';
		std::cout << "Leech Seed: " << team.active->leech_seed << '\n';
		std::cout << "Loaf: " << team.active->loaf << '\n';
		std::cout << "Me First: " << team.active->mf << '\n';
		std::cout << "Minimize: " << team.active->minimize << '\n';
		std::cout << "Moved: " << team.active->moved << '\n';
		std::cout << "Mud Sport: " << team.active->mud_sport << '\n';
		std::cout << "Nightmare: " << team.active->nightmare << '\n';
		std::cout << "Torment: " << team.active->torment << '\n';
		std::cout << "Trapped: " << team.active->trapped << '\n';
		std::cout << "Water Sport: " << team.active->water_sport << '\n';
		std::cout << "Gender: " << team.active->gender << '\n';
		std::cout << "Confused: " << static_cast<int> (team.active->confused) << '\n';
		std::cout << "Embargo: " << static_cast<int> (team.active->embargo) << '\n';
		std::cout << "Encore: " << static_cast<int> (team.active->encore) << '\n';
		std::cout << "Happiness: " << static_cast<int> (team.active->happiness) << '\n';
		std::cout << "Heal Block: " << static_cast<int> (team.active->heal_block) << '\n';
		std::cout << "Level: " << static_cast<int> (team.active->level) << '\n';
		std::cout << "Magnet Rise: " << static_cast<int> (team.active->magnet_rise) << '\n';
		std::cout << "Mass: " << static_cast<int> (team.active->mass) << '\n';
		std::cout << "Partial Trap: " << static_cast<int> (team.active->partial_trap) << '\n';
		std::cout << "Perish Song: " << static_cast<int> (team.active->perish_song) << '\n';
		std::cout << "Rampage: " << static_cast<int> (team.active->rampage) << '\n';
		std::cout << "Sleep: " << static_cast<int> (team.active->sleep) << '\n';
		std::cout << "Slow Start: " << static_cast<int> (team.active->slow_start) << '\n';
		std::cout << "Stockpile: " << static_cast<int> (team.active->stockpile) << '\n';
		std::cout << "Taunt: " << static_cast<int> (team.active->taunt) << '\n';
		std::cout << "Toxic: " << static_cast<int> (team.active->toxic) << '\n';
		std::cout << "Yawn: " << static_cast<int> (team.active->yawn) << '\n';
		std::cout << "Item: " << team.active->item << '\n';
		std::cout << "Nature: " << team.active->nature << '\n';
		std::cout << "HP base: " << static_cast<int> (team.active->hp.base) << '\n';
		std::cout << "HP IV: " << static_cast<int> (team.active->hp.iv) << '\n';
		std::cout << "HP EV: " << static_cast<int> (team.active->hp.ev) << '\n';
		std::cout << "HP stat: " << static_cast<int> (team.active->hp.stat) << '\n';
		std::cout << "HP max: " << static_cast<int> (team.active->hp.max) << '\n';
		std::cout << "Atk base: " << static_cast<int> (team.active->atk.base) << '\n';
		std::cout << "Atk IV: " << static_cast<int> (team.active->atk.iv) << '\n';
		std::cout << "Atk EV: " << static_cast<int> (team.active->atk.ev) << '\n';
		std::cout << "Atk stage: " << static_cast<int> (team.active->atk.stage) << '\n';
		std::cout << "Atk stat: " << static_cast<int> (team.active->atk.stat) << '\n';
		std::cout << "Def base: " << static_cast<int> (team.active->def.base) << '\n';
		std::cout << "Def IV: " << static_cast<int> (team.active->def.iv) << '\n';
		std::cout << "Def EV: " << static_cast<int> (team.active->def.ev) << '\n';
		std::cout << "Def stage: " << static_cast<int> (team.active->def.stage) << '\n';
		std::cout << "Def stat: " << static_cast<int> (team.active->def.stat) << '\n';
		std::cout << "SpA base: " << static_cast<int> (team.active->spa.base) << '\n';
		std::cout << "SpA IV: " << static_cast<int> (team.active->spa.iv) << '\n';
		std::cout << "SpA EV: " << static_cast<int> (team.active->spa.ev) << '\n';
		std::cout << "SpA stage: " << static_cast<int> (team.active->spa.stage) << '\n';
		std::cout << "SpA stat: " << static_cast<int> (team.active->spa.stat) << '\n';
		std::cout << "SpD base: " << static_cast<int> (team.active->spd.base) << '\n';
		std::cout << "SpD IV: " << static_cast<int> (team.active->spd.iv) << '\n';
		std::cout << "SpD EV: " << static_cast<int> (team.active->spd.ev) << '\n';
		std::cout << "SpD stage: " << static_cast<int> (team.active->spd.stage) << '\n';
		std::cout << "SpD stat: " << static_cast<int> (team.active->spd.stat) << '\n';
		std::cout << "Spe base: " << static_cast<int> (team.active->spe.base) << '\n';
		std::cout << "Spe IV: " << static_cast<int> (team.active->spe.iv) << '\n';
		std::cout << "Spe EV: " << static_cast<int> (team.active->spe.ev) << '\n';
		std::cout << "Spe stage: " << static_cast<int> (team.active->spe.stage) << '\n';
		std::cout << "Spe stat: " << static_cast<int> (team.active->spe.stat) << '\n';
		std::cout << "Status: " << static_cast<int> (team.active->status) << '\n';
		std::cout << "Type1: " << static_cast<int> (team.active->type1) << '\n';
		std::cout << "Type2: " << static_cast<int> (team.active->type2) << '\n';
		std::cout << "Vanish: " << static_cast<int> (team.active->vanish) << '\n';
//		for (std::vector<moves>::iterator team.active->move = team.active->moveset.begin(); team.active->move != team.active->moveset.end(); ++team.active->move) {
			std::cout << "Name: " << team.active->move->name << '\n';
			std::cout << "CH: " << team.active->move->ch << '\n';
			std::cout << "Execute: " << team.active->move->execute << '\n';
			std::cout << "Physical: " << team.active->move->physical << '\n';
			std::cout << "Selectable: " << team.active->move->selectable << '\n';
			std::cout << "Accuracy: " << static_cast<int> (team.active->move->accuracy) << '\n';
			std::cout << "Base Power: " << team.active->move->basepower << '\n';
			std::cout << "Disable: " << static_cast<int> (team.active->move->disable) << '\n';
			std::cout << "Power: " << team.active->move->power << '\n';
			std::cout << "PP: " << static_cast<int> (team.active->move->pp) << '\n';
			std::cout << "PP max: " << static_cast<int> (team.active->move->pp_max) << '\n';
			std::cout << "Priority: " << static_cast<int> (team.active->move->priority) << '\n';
			std::cout << "r: " << static_cast<int> (team.active->move->r) << '\n';
			std::cout << "Times used: " << static_cast<int> (team.active->move->times_used) << '\n';
			std::cout << "Variable: " << static_cast<int> (team.active->move->variable) << '\n';
			std::cout << "Type: " << team.active->move->type << '\n';
//		}
//	}
	if (team.me)
		std::cout << "---------AI---------\n\n";
	else
		std::cout << "---------Foe---------\n\n";
	std::cin.get();
}
#endif
