#include "../plugin_sdk/plugin_sdk.hpp"
#include "NamiExample.h"

namespace nami {

	TreeTab* mainMenuTab = nullptr;

	namespace interruptSettings {
		TreeTab* db = nullptr;
		TreeEntry* useQ = nullptr;
		TreeEntry* useR = nullptr;
	}

	script_spell* q = nullptr;
	script_spell* r = nullptr;

	void on_update() {
		// If i cant even cast a spell right now, skip this game tick
		if (!myhero->can_cast()) return;

		// I go through all the enemies...
		for (const auto& enemy : entitylist->get_enemy_heroes()) {

			// ... and if they are "interruptable" and are casting an important Spell ...
			if (!enemy || enemy->is_dead() || enemy->get_is_cc_immune() || enemy->is_zombie()) return;
			auto data = interrupt::getInterruptable(enemy);
			if (data.dangerLevel <= 0) continue;

			// ... I first try to use Q to interrupt ...
			if (q->is_ready() && interruptSettings::useQ->get_bool()) {
				auto pred = q->get_prediction(enemy);

				// and if its possible to hit and the spell is actually dangerous enough and its pretty likely they will cast for long enough...
				if (pred.hitchance >= hit_chance::low && data.dangerLevel >= 2 && data.expectedRemainingTime >= q->delay) {

					// then I cast Q and return, since i cant cast 2 spells at once
					q->cast(pred.get_cast_position());
					myhero->print_chat(0, "Casted Q to interrupt %s with danger %i and %f time left", 
						interrupt::getDisplayName(enemy).c_str(), data.dangerLevel, data.expectedRemainingTime);
					return;
				}
			}
			// And then I try to use R to interrupt ...
			if (r->is_ready() && interruptSettings::useR->get_bool()) {
				auto pred = r->get_prediction(enemy);

				// this time i need to check how long the r takes to hit the enemy 
				// (doing the same would work out for q as well, since the speed is practically infinite, so the traveltime is 0 and it just takes the delay
				float expectedTravelTime = pred.get_unit_position().distance(myhero) / r->get_speed();

				// here i check if danger is at least 3, change to whatever you feel like for the spell you want to use
				// and here i check for 
				if (pred.hitchance >= hit_chance::low && data.dangerLevel >= 3 && data.expectedRemainingTime >= r->delay + expectedTravelTime) {
					q->cast(pred.get_cast_position());
					myhero->print_chat(0, "Casted R to interrupt %s with danger %i and %f time left",
						interrupt::getDisplayName(enemy).c_str(), data.dangerLevel, data.expectedRemainingTime);
					return;
				}
			}

		}


	}
	void load() {
		q = plugin_sdk->register_spell(spellslot::q, 850);
		q->set_skillshot(1, 200, FLT_MAX, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_circle);
		r = plugin_sdk->register_spell(spellslot::r, 2750);
		r->set_skillshot(0.5, 250, 850, { collisionable_objects::yasuo_wall }, skillshot_type::skillshot_line);
		mainMenuTab = menu->create_tab("FlofianNami", "Interrupt Example");
		auto interruptMenu = mainMenuTab->add_tab("interrupt", "Interrupt Settings");
		interruptSettings::useQ = interruptMenu->add_checkbox("useQ", "Use Q when Importance >= 2", true);
		interruptSettings::useR = interruptMenu->add_checkbox("useR", "Use R when Importance >= 3", true);


		interruptSettings::db = interruptMenu->add_tab("db", "Spell Importance");

		// This is the important line, this initializes the menu, and the menu needs to exit to compare the importance values
		interrupt::InitializeCancelMenu(interruptSettings::db);

		event_handler<events::on_update>::add_callback(on_update);
	}
	void unload() {
		menu->delete_tab(mainMenuTab);
		event_handler<events::on_update>::remove_handler(on_update);
	}
}