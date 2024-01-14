#include "../plugin_sdk/plugin_sdk.hpp"
#include "DebugUtil.h"
#include "Interrupt.h"

namespace util {
	TreeTab* mainMenuTab = nullptr;
	
	
	void on_draw() {

		for (const auto& ally : entitylist->get_ally_heroes()) {
			auto data = InterruptDB::getInterruptable(ally);
			draw_manager->add_text(ally->get_position(), MAKE_COLOR(255, 0, 0, 255), 20, "Danger: %i", data.dangerLevel);
			draw_manager->add_text(ally->get_position() - vector(0,50,0), MAKE_COLOR(255, 255, 0, 255), 20, "ExpectedTime: %.2f", data.expectedRemainingTime);
			draw_manager->add_text(ally->get_position() - vector(0, 100, 0), MAKE_COLOR(255, 255, 0, 255), 20, "MinTime: %.2f", data.minRemainingTime);
			draw_manager->add_text(ally->get_position() - vector(0, 150, 0), MAKE_COLOR(255, 255, 0, 255), 20, "MaxTime: %.2f", data.maxRemainingTime);
		}


	}
	void load() {
		mainMenuTab = menu->create_tab("FlofianDebugUtil", "Interrupt Debug Util");
		InterruptDB::InitializeCancelMenu(mainMenuTab, false, -1, true);
		console->print(myhero->get_model_cstr());

		event_handler<events::on_draw>::add_callback(on_draw);
	}
	void unload() {
		menu->delete_tab(mainMenuTab);
		event_handler<events::on_draw>::remove_handler(on_draw);
	}
}