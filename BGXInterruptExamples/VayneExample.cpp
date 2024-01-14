#include "../plugin_sdk/plugin_sdk.hpp"
#include "VayneExample.h"

namespace vayne {

	TreeTab* mainMenuTab = nullptr;

	namespace interruptSettings {
		TreeTab* db = nullptr;
		TreeEntry* useE = nullptr;
	}

	script_spell* e = nullptr;

	void on_update() {
		// If i cant even cast a spell right now, skip this game tick
		if (!myhero->can_cast()) return;

		// If E is ready and i want to use it for interrupts ...
		if (e->is_ready() && interruptSettings::useE->get_bool()) {
		// ... I go through all the enemies ...
			for (const auto& enemy : entitylist->get_enemy_heroes()) {

				// ... and if they are "interruptable" ...
				if (!enemy || enemy->is_dead() || enemy->get_is_cc_immune() || enemy->is_zombie()) return;
				auto data = interrupt::getInterruptable(enemy);

				float estimatedTravelTime = myhero->get_distance(enemy) / 2200.f + 0.25f;	// Vayne E Missile Speed + Cast Time

				// and if the spell is dangerous and its pretty likely they will cast for long enough...
				if (data.dangerLevel && data.expectedRemainingTime >= estimatedTravelTime) {

					// then I cast E and return, since i cant cast 2 spells at once
					e->cast(enemy);
					myhero->print_chat(0, "Casted E to interrupt %s with %f time left",
						interrupt::getDisplayName(enemy).c_str(), data.expectedRemainingTime);
					return;
				}
				
			}
		}


	}
	void load() {
		e = plugin_sdk->register_spell(spellslot::e, 550);
		mainMenuTab = menu->create_tab("FlofianVayne", "Interrupt Example");
		auto interruptMenu = mainMenuTab->add_tab("interrupt", "Interrupt Settings");
		interruptSettings::useE = interruptMenu->add_checkbox("useE", "Use E to interrupt", true);

		/*Either this
		interruptSettings::db = interruptMenu->add_tab("db", "Interruptable Spells");

		// This is the important line, this initializes the menu, and the menu needs to exit to compare the importance values
		interrupt::InitializeCancelMenu(interruptSettings::db, true, 2);

		*/
		// Or this
		interruptMenu->add_separator("sep1", "");
		interrupt::InitializeCancelMenu(interruptMenu, true, 2);

		event_handler<events::on_update>::add_callback(on_update);
	}
	void unload() {
		menu->delete_tab(mainMenuTab);
		event_handler<events::on_update>::remove_handler(on_update);
	}
}