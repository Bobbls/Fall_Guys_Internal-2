#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "draw_manager.h"
#include "render.h"
#include "values.h"

#include <mutex>

namespace FGInternal {
	namespace GENERAL {
		bool hide_corner_text = false;
		bool disable_safe_values = false;
	};

	namespace ESP {
		bool correct_doors_enabled = false;
		bool correct_path_enabled = false;
		bool non_jinxed_players_enabled = false;
		bool show_all_platforms_enabled = false;
		bool show_player_with_tail = false;
	};

	namespace MOVEMENT {
		bool fly_enabled = false;
		float fly_speed = 15;

		bool speed_enabled = false;
		float speed_boost = 12;

		bool dive_enabled = false;
		float normalDive_speed = 25;
		float airDive_speed = 15;

		bool gravity_enabled = false;
		float gravity_scale = 2;
	};

	namespace COLLISIONS {
		bool stun_collision = false;
		bool object_collision = false;
	};

	namespace CARRY {
		bool carryDropForce = false;
		float carryNormalDropBoost = 0;
		float carryDiveDropBoost = 0;
	};
};

namespace FGInternalHelper {
	bool disable_correct_doors = false;
	bool disable_correct_path = false;
	bool disable_non_jinxed_players = false;
	bool disable_show_all_platforms = false;
	bool disable_show_player_with_tail = false;

	bool disable_fly = false;
	bool disable_speed = false;
	bool disable_dive = false;
	bool disable_gravity = false;

	bool disable_carryDropForce = false;
	bool disable_carryDiveDropForce = false;
}

namespace menu {
	bool general_tab_active = true;
	bool esp_tab_active = true;
	bool boost_tab_active = true;
	bool collisions_tab_active = true;
	bool carrying_tab_active = true;

	void push_color_for_button(bool active) 
	{
		if (active) 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
		}
		else 
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 61.f / 255.f, 61.f / 255.f, 61.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 128.f / 255.f, 199.f / 255.f, 1.f });
		}
	}

	void draw_button(const char* name, bool& config_key, bool* change_opositive = nullptr)
	{
		push_color_for_button(config_key);
		if (ImGui::Button(name, {244, 25}))
		{
			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
		}

		ImGui::PopStyleColor(2);
	}

	void draw_buttonDisabled(const char* name, bool config_key = false, bool* change_opositive = nullptr)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 61.f / 255.f, 61.f / 255.f, 61.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 61.f / 255.f, 61.f / 255.f, 61.f / 255.f, 1.f });
		if (!ImGui::Button(name, { 244, 25 }))
			return;

		ImGui::PopStyleColor(2);
	}

	void draw_slider(const char* name, float* val, float min, float max, const char* display_format = "%.2f")
	{
		if (!display_format)
			display_format = "%.2f";

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });

		ImGui::SetNextItemWidth(244.0f);
		ImGui::SliderFloat(name, val, min, max, display_format);

		ImGui::PopStyleColor(2);
	}

	void draw_tab(const char* name, bool& active)
	{
		ImGui::Text(name);
		ImGui::SameLine(237);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
		if (ImGui::Button("", { 10, 10 }))
			active = !active;
		ImGui::PopStyleColor(2);
	}

	std::once_flag init_style;
	void draw() 
	{
		std::call_once(init_style, [&]() {
			auto& style = ImGui::GetStyle();
			style.WindowRounding = 0.f;
			style.FrameRounding = 0.f;
			style.ItemSpacing = ImVec2(8, 2);
			style.WindowPadding = ImVec2(3.f, 3.f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4{ 56.f / 255.f, 59.f / 255.f, 58.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4{ 56.f / 255.f, 59.f / 255.f, 58.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 56.f / 255.f, 59.f / 255.f, 58.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrab] = ImVec4{ 0.f, 128.f / 255.f, 199.f / 255.f, 1.f };
			});

		ImGui::Begin("general_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» GENERAL «", general_tab_active);
		if (general_tab_active) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"HOME » Hide Corner Menu");
			draw_button("Left Sided Menu", FGInternal::GENERAL::hide_corner_text);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"END » Unhook Cheat");
			draw_buttonDisabled("Kill Cheat Process");
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"DEL » Panic Key");
			draw_buttonDisabled("Kill Game Process");
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"Unsafe Values » 0-2137");
			draw_button("Disable Safe Values", FGInternal::GENERAL::disable_safe_values);
		}
		ImGui::End();

		ImGui::Begin("visuals_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» VISUALS «", esp_tab_active);
		if (esp_tab_active) {
			ImGui::Text(u8"F5 » Doors Rush");
			draw_button("Correct Doors", FGInternal::ESP::correct_doors_enabled, &FGInternalHelper::disable_correct_doors);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F6 » Tip Toe");
			draw_button("Correct Path", FGInternal::ESP::correct_path_enabled, &FGInternalHelper::disable_correct_path);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F7 » Jinxed");
			draw_button("Non-Jinxed Players", FGInternal::ESP::non_jinxed_players_enabled, &FGInternalHelper::disable_non_jinxed_players);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F8 » Fall Match (Fruits)");
			draw_button("All Platforms Visible", FGInternal::ESP::show_all_platforms_enabled, &FGInternalHelper::disable_show_all_platforms);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F9 » Grab a Tail (Final)");
			draw_button("Player with Tail", FGInternal::ESP::show_player_with_tail, &FGInternalHelper::disable_show_player_with_tail);
		}
		ImGui::End();

		ImGui::Begin("collisions_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» COLLISIONS «", collisions_tab_active);
		if (collisions_tab_active) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F10 » Stuns/Knockdowns");
			draw_button("Disable Stuns/Knockdowns", FGInternal::COLLISIONS::stun_collision);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F11 » Objects Collisions");
			draw_button("Disable Objects Collisions", FGInternal::COLLISIONS::object_collision);
		}
		ImGui::End();

		ImGui::Begin("movement_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» MOVEMENT TAB «", boost_tab_active);
		if (boost_tab_active && !FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F1 » [SPACE/SHIFT] » Flying Mode");
			draw_button("Enable Flying", FGInternal::MOVEMENT::fly_enabled, &FGInternalHelper::disable_fly);
			draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_min, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_max);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F2 » [W] » Speed Hack");
			draw_button("Movement Speed", FGInternal::MOVEMENT::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Speed Boost", &FGInternal::MOVEMENT::speed_boost, VALUES::SAFE_VALUES::MOVEMENT::speed_boost_min, VALUES::SAFE_VALUES::MOVEMENT::speed_boost_max);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F3 » [CTRL/RMB] » Dive");
			draw_button("Dive Boost", FGInternal::MOVEMENT::dive_enabled, &FGInternalHelper::disable_dive);
			draw_slider("Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_max);
			draw_slider("Air Dive", &FGInternal::MOVEMENT::airDive_speed, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_max);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F4 » [SPACE] » Gravitation");
			draw_button("Gravity Scale", FGInternal::MOVEMENT::gravity_enabled, &FGInternalHelper::disable_gravity);
			draw_slider("Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_min, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_max);
		} else if (boost_tab_active && FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F1 » [SPACE/SHIFT] » Flying Mode");
			draw_button("Enable Flying", FGInternal::MOVEMENT::fly_enabled, &FGInternalHelper::disable_fly);
			draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, 0, 2137);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F2 » [W] » Speed Hack");
			draw_button("Movement Speed", FGInternal::MOVEMENT::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Speed Boost", &FGInternal::MOVEMENT::speed_boost, 0, 2137);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F3 » [CTRL/RMB] » Dive");
			draw_button("Dive Boost", FGInternal::MOVEMENT::dive_enabled, &FGInternalHelper::disable_dive);
			draw_slider("Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, 0, 2137);
			draw_slider("Air Dive", &FGInternal::MOVEMENT::airDive_speed, 0, 2137);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F4 » [SPACE] » Gravitation");
			draw_button(" Gravity Scale", FGInternal::MOVEMENT::gravity_enabled, &FGInternalHelper::disable_gravity);
			draw_slider("Gravitation", &FGInternal::MOVEMENT::gravity_scale, 0, 10);
		}
		ImGui::End();

		ImGui::Begin("carrying_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» CARRYING ITEMS «", carrying_tab_active);
		if (carrying_tab_active && !FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F12 » [SHIFT] » Drop Boost");
			draw_button("Item Drop", FGInternal::CARRY::carryDropForce, &FGInternalHelper::disable_carryDropForce);
			draw_slider("Normal Drop", &FGInternal::CARRY::carryNormalDropBoost, VALUES::SAFE_VALUES::CARRY::carryNormalDropForce_min, VALUES::SAFE_VALUES::CARRY::carryNormalDropForce_max);
			draw_slider("Dive Drop", &FGInternal::CARRY::carryDiveDropBoost, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_min, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_max);
		} else if (carrying_tab_active && FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F12 » [SHIFT] » Drop Boost");
			draw_button("Item Drop", FGInternal::CARRY::carryDropForce, &FGInternalHelper::disable_carryDropForce);
			draw_slider("Normal Drop", &FGInternal::CARRY::carryNormalDropBoost, 0, 2137);
			draw_slider("Dive Drop", &FGInternal::CARRY::carryDiveDropBoost, 0, 2137);
		}
		ImGui::End();
	}

	bool OldKeysDown[512];
	float OldNavInputs[ImGuiNavInput_COUNT];
	void update_keys() {

		auto io = ImGui::GetIO();

		if ((io.KeysDown[VK_INSERT] && !OldKeysDown[VK_INSERT]) ||
			(io.NavInputs[ImGuiNavInput_RightThumb] > 0.f && OldNavInputs[ImGuiNavInput_RightThumb] == 0.f)) {
			render::menu_is_open = !render::menu_is_open;
		}

		if (render::menu_is_open) {
			auto context = ImGui::GetCurrentContext();
			if (context->NavWindow) {
				if (context->Windows.Size > 1) {
					int32_t move_window_direction = -1;
					if (io.NavInputs[ImGuiNavInput_FocusPrev] > 0.f && OldNavInputs[ImGuiNavInput_FocusPrev] == 0.f)
						move_window_direction = 0;
					else if (io.NavInputs[ImGuiNavInput_FocusNext] > 0.f && OldNavInputs[ImGuiNavInput_FocusNext] == 0.f)
						move_window_direction = 1;

					if (move_window_direction != -1) {
						ImGuiWindow* new_window = nullptr;
						for (auto window : context->Windows) {
							if (window == context->NavWindow || window->Hidden || window->IsFallbackWindow)
								continue;

							if (!strcmp(window->Name, "BackBuffer"))
								continue;

							if (move_window_direction == 0) {
								if (window->Pos.x <= context->NavWindow->Pos.x)
									if (!new_window || window->Pos.x > new_window->Pos.x)
										new_window = window;
							}
							else {
								if (window->Pos.x >= context->NavWindow->Pos.x)
									if (!new_window || window->Pos.x < new_window->Pos.x)
										new_window = window;
							}
						}
						if (new_window) {
							ImGui::FocusWindow(new_window);
						}
					}
				}
			}
		}

		if ((io.KeysDown[VK_F1] && !OldKeysDown[VK_F1]) || (io.NavInputs[ImGuiNavInput_Input] && !OldNavInputs[ImGuiNavInput_Input])) {
			FGInternal::MOVEMENT::fly_enabled = !FGInternal::MOVEMENT::fly_enabled;
			FGInternalHelper::disable_fly = !FGInternal::MOVEMENT::fly_enabled;
		}

		if (io.KeysDown[VK_F2] && !OldKeysDown[VK_F2]) {
			FGInternal::MOVEMENT::speed_enabled = !FGInternal::MOVEMENT::speed_enabled;
			FGInternalHelper::disable_speed = !FGInternal::MOVEMENT::speed_enabled;
		}

		if (io.KeysDown[VK_F3] && !OldKeysDown[VK_F3]) {
			FGInternal::MOVEMENT::dive_enabled = !FGInternal::MOVEMENT::dive_enabled;
			FGInternalHelper::disable_dive = !FGInternal::MOVEMENT::dive_enabled;
		}

		if (io.KeysDown[VK_F4] && !OldKeysDown[VK_F4]) {
			FGInternal::MOVEMENT::gravity_enabled = !FGInternal::MOVEMENT::gravity_enabled;
			FGInternalHelper::disable_gravity = !FGInternal::MOVEMENT::gravity_enabled;
		}

		if (io.KeysDown[VK_F5] && !OldKeysDown[VK_F5]) {
			FGInternal::ESP::correct_doors_enabled = !FGInternal::ESP::correct_doors_enabled;
			FGInternalHelper::disable_correct_doors = !FGInternal::ESP::correct_doors_enabled;
		}

		if (io.KeysDown[VK_F6] && !OldKeysDown[VK_F6]) {
			FGInternal::ESP::correct_path_enabled = !FGInternal::ESP::correct_path_enabled;
			FGInternalHelper::disable_correct_path = !FGInternal::ESP::correct_path_enabled;
		}

		if (io.KeysDown[VK_F7] && !OldKeysDown[VK_F7]) {
			FGInternal::ESP::non_jinxed_players_enabled = !FGInternal::ESP::non_jinxed_players_enabled;
			FGInternalHelper::disable_non_jinxed_players = !FGInternal::ESP::non_jinxed_players_enabled;
		}

		if (io.KeysDown[VK_F8] && !OldKeysDown[VK_F8]) {
			FGInternal::ESP::show_all_platforms_enabled = !FGInternal::ESP::show_all_platforms_enabled;
			FGInternalHelper::disable_show_all_platforms = !FGInternal::ESP::show_all_platforms_enabled;
		}

		if (io.KeysDown[VK_F9] && !OldKeysDown[VK_F9]) {
			FGInternal::ESP::show_player_with_tail = !FGInternal::ESP::show_player_with_tail;
			FGInternalHelper::disable_show_player_with_tail = !FGInternal::ESP::show_player_with_tail;
		}

		if (io.KeysDown[VK_F10] && !OldKeysDown[VK_F10])
			FGInternal::COLLISIONS::stun_collision = !FGInternal::COLLISIONS::stun_collision;

		if (io.KeysDown[VK_F11] && !OldKeysDown[VK_F11])
			FGInternal::COLLISIONS::object_collision = !FGInternal::COLLISIONS::object_collision;

		if (io.KeysDown[VK_F12] && !OldKeysDown[VK_F12]) {
			FGInternal::CARRY::carryDropForce = !FGInternal::CARRY::carryDropForce;
			FGInternalHelper::disable_carryDropForce = !FGInternal::CARRY::carryDropForce;
		}

		if (io.KeysDown[VK_HOME] && !OldKeysDown[VK_HOME]) {
			FGInternal::GENERAL::hide_corner_text = !FGInternal::GENERAL::hide_corner_text;
		}

		if (io.NavInputs[ImGuiNavInput_FocusPrev] > 0.f) {
			FGInternal::MOVEMENT::speed_enabled = true;
			FGInternalHelper::disable_speed = false;
		} else if (io.NavInputs[ImGuiNavInput_FocusPrev] == 0.f && OldNavInputs[ImGuiNavInput_FocusPrev] > 0.f) {
			FGInternal::MOVEMENT::speed_enabled = false;
			FGInternalHelper::disable_speed = true;
		}

		memcpy(OldKeysDown, io.KeysDown, 512 * sizeof(bool));
		memcpy(OldNavInputs, io.NavInputs, ImGuiNavInput_COUNT * sizeof(float));
	}

	void update_indicators() {
		auto text_size = draw_manager::calc_text_size(12, "TAKE");
		auto y = float(15);

		if (!FGInternal::GENERAL::hide_corner_text) {
			draw_manager::add_text_on_screen({ 5, y }, 0xFF4045DB, 14, "FG Internal");
			y += text_size.y + 4.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, u8"» github.com/xTeJk «");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, u8"» INSERT » Open Cheat Menu");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, u8"» HOME » Hide Corner Text");
			y += text_size.y + 3.f;
		}
	}
};
