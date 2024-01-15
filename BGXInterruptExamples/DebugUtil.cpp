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
			if (ally->get_spell(spellslot::w)->get_name_hash() == spell_hash("AurelionSolWToggle") && ally->is_playing_animation(buff_hash("Spell2")))
				draw_manager->add_text(ally->get_position() - vector(0, 200, 0), MAKE_COLOR(255, 0, 0, 255), 20, "Yorick Method");
			if (ally->get_spell(spellslot::w)->get_name_hash() == spell_hash("AurelionSolWToggle")) {
				
				auto b = ally->get_buff(buff_hash("AurelionSolW"));
				if (b) draw_manager->add_text(ally->get_position() - vector(0, 250, 0), MAKE_COLOR(255, 0, 0, 255), 20, "%f",gametime->get_time()- b->get_start());
			}
		}

	}
	void on_play_animation(game_object_script sender, const char* name, bool* process)
	{
		if (sender->is_ai_hero())
			console->print("Animation %s by %s", name, InterruptDB::getDisplayName(sender).c_str());
	}
	void on_network_packet(game_object_script sender, std::uint32_t network_id, pkttype_e type, void* args)
	{
		if (type != pkttype_e::PKT_S2C_PlayAnimation_s || !sender) return;

		// Get animation info, if there is none then ignore that animation
		const auto& data = (PKT_S2C_PlayAnimationArgs*)args;
		if (!data) return;

		console->print("Name : %s Owner : %s", data->animation_name, sender->get_model_cstr());
	}
	void load() {
		mainMenuTab = menu->create_tab("FlofianDebugUtil", "Interrupt Debug Util");
		InterruptDB::InitializeCancelMenu(mainMenuTab, false, -1, true);
		for (const auto& b : myhero->get_bufflist()) {
			if (b && b->is_valid() && b->is_alive()) {
				console->print("%s %i", b->get_name_cstr(), b->get_count());
			}
		}

		event_handler<events::on_draw>::add_callback(on_draw);
		event_handler<events::on_play_animation>::add_callback(on_play_animation);
		event_handler<events::on_network_packet>::add_callback(on_network_packet);
	}
	void unload() {
		menu->delete_tab(mainMenuTab);
		event_handler<events::on_draw>::remove_handler(on_draw);
		event_handler<events::on_play_animation>::remove_handler(on_play_animation);
		event_handler<events::on_network_packet>::remove_handler(on_network_packet);
	}
}