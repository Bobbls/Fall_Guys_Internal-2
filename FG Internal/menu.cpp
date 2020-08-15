#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "draw_manager.h"
#include "render.h"
#include "values.h"

#include <mutex>

namespace FGInternal {
	namespace ESP {
		bool correct_doors_enabled = false;
		bool correct_path_enabled = false;
		bool non_jinxed_players_enabled = false;
		bool correct_platforms_enabled = false;
	};

	namespace MOVEMENT {
		bool fly_enabled = false;
		int fly_speed = 15;

		bool speed_enabled = false;
		int speed_boost = 12;

		bool dive_enabled = false;
		int dive_speed = 25;

		bool gravity_enabled = false;
		int gravity_scale = 2;
	};

	namespace COLLISIONS {
		bool stun_collision = false;
		bool object_collision = false;
	};

	namespace CARRY {
		bool grabCooldown = false;
		int grabCooldownBoost = 0;

		bool carryPickupDuration = false;
		int carryPickupBoost = 0;

		bool carryDropForce = false;
		int carryDropBoost = 0;

		bool carryDiveDropForce = false;
		int carryDiveDropBoost = 0;
	};
};

namespace FGInternalHelper {
	bool disable_correct_doors = false;
	bool disable_correct_path = false;
	bool disable_non_jinxed_players = false;
	bool disable_correct_platforms = false;

	bool disable_fly = false;
	bool disable_speed = false;
	bool disable_dive = false;
	bool disable_gravity = false;

	bool disable_grabCooldown = false;
	bool disable_carryPickupDuration = false;
	bool disable_carryDropForce = false;
	bool disable_carryDiveDropForce = false;
}

namespace menu {
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
		if (ImGui::Button(name, { 244, 25 })) 
		{
			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
		}

		ImGui::PopStyleColor(2);
	}

	void draw_slider(const char* name, int* val, int min, int max) 
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.f, 92.f / 255.f, 196.f / 255.f, 1.f });

		ImGui::SliderInt(name, val, min, max);

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

		ImGui::Begin("esp_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("Visuals", esp_tab_active);
		if (esp_tab_active) {
			draw_button("Correct Doors (Doors Rush)", FGInternal::ESP::correct_doors_enabled, &FGInternalHelper::disable_correct_doors);
			draw_button("Correct Path (TipToe)", FGInternal::ESP::correct_path_enabled, &FGInternalHelper::disable_correct_path);
			draw_button("Non-Jinxed Players", FGInternal::ESP::non_jinxed_players_enabled, &FGInternalHelper::disable_non_jinxed_players);
			draw_button("Correct Platforms (Fruits)", FGInternal::ESP::correct_platforms_enabled, &FGInternalHelper::disable_correct_platforms);
		}
		ImGui::End();

		ImGui::Begin("misc_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("Collisions", collisions_tab_active);
		if (collisions_tab_active) {
			draw_button("Disable Stuns", FGInternal::COLLISIONS::stun_collision);
			draw_button("Disable Objects Collisions", FGInternal::COLLISIONS::object_collision);
		}
		ImGui::End();

		ImGui::Begin("boosts_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("Movement", boost_tab_active);
		if (boost_tab_active) {
			draw_button("Flying", FGInternal::MOVEMENT::fly_enabled, &FGInternalHelper::disable_fly);
			draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_min, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_max);
			draw_button("Movement Speed (Default: 9.5)", FGInternal::MOVEMENT::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Speed Boost", &FGInternal::MOVEMENT::speed_boost, VALUES::SAFE_VALUES::MOVEMENT::speed_boost_min, VALUES::SAFE_VALUES::MOVEMENT::speed_boost_max);
			draw_button("Dive Boost (Default: 16.5)", FGInternal::MOVEMENT::dive_enabled, &FGInternalHelper::disable_dive);
			draw_slider("Dive Speed", &FGInternal::MOVEMENT::dive_speed, VALUES::SAFE_VALUES::MOVEMENT::dive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::dive_speed_max);
			draw_button("Gravity Scale (Default: 1.5)", FGInternal::MOVEMENT::gravity_enabled, &FGInternalHelper::disable_gravity);
			draw_slider("Gravitation", &FGInternal::MOVEMENT::gravity_scale, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_min, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_max);
		}
		ImGui::End();

		ImGui::Begin("carrying_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("WIP", carrying_tab_active);
		if (carrying_tab_active) {
			draw_button("grabCooldown", FGInternal::CARRY::grabCooldown, &FGInternalHelper::disable_grabCooldown);
			draw_slider("grabCooldownBoost", &FGInternal::CARRY::grabCooldownBoost, VALUES::SAFE_VALUES::CARRY::grabCooldowwn_min, VALUES::SAFE_VALUES::CARRY::grabCooldowwn_max);
			draw_button("carryPickupDuration", FGInternal::CARRY::carryPickupDuration, &FGInternalHelper::disable_carryPickupDuration);
			draw_slider("carryPickupBoost", &FGInternal::CARRY::carryPickupBoost, VALUES::SAFE_VALUES::CARRY::carryPickupDuration_min, VALUES::SAFE_VALUES::CARRY::carryPickupDuration_max);
			draw_button("carryDropForce", FGInternal::CARRY::carryDropForce, &FGInternalHelper::disable_carryDropForce);
			draw_slider("carryDropBoost", &FGInternal::CARRY::carryDropBoost, VALUES::SAFE_VALUES::CARRY::carryDropForce_min, VALUES::SAFE_VALUES::CARRY::carryDropForce_max);
			draw_button("carryDiveDropForce", FGInternal::CARRY::carryDiveDropForce, &FGInternalHelper::disable_carryDiveDropForce);
			draw_slider("carryDiveDropBoost", &FGInternal::CARRY::carryDiveDropBoost, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_min, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_max);
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
			FGInternal::ESP::correct_platforms_enabled = !FGInternal::ESP::correct_platforms_enabled;
			FGInternalHelper::disable_correct_platforms = !FGInternal::ESP::correct_platforms_enabled;
		}

		if (io.KeysDown[VK_F9] && !OldKeysDown[VK_F9])
			FGInternal::COLLISIONS::stun_collision = !FGInternal::COLLISIONS::stun_collision;

		if (io.KeysDown[VK_F10] && !OldKeysDown[VK_F10])
			FGInternal::COLLISIONS::object_collision = !FGInternal::COLLISIONS::object_collision;

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

		draw_manager::add_text_on_screen({ 5, y }, 0xFF4045DB, 14, "ESP Menu - INSERT");
		y += text_size.y + 4.f;

		//if (FGInternal::MOVEMENT::fly_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F1 > Flying Enabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F1 > Flying Disabled");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::MOVEMENT::speed_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F2 > Movement Speed Enabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F2 > Movement Speed Disabled");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::MOVEMENT::dive_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F3 > Dive Speed Enabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F3 > Dive Speed Disabled");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::MOVEMENT::gravity_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F4 > Gravity Scale Enabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F4 > Gravity Scale Disabled");
		//	y += text_size.y + 3.f;
		//}
		//
		///////////////////
		//
		//if (FGInternal::ESP::correct_doors_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F5 > Correct Doors Shown");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F5 > Correct Doors Hidden");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::ESP::correct_path_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F6 > Correct Path Shown");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F6 > Correct Path Hidden");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::ESP::non_jinxed_players_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F7 > Non-Jinxed Players Shown");
		//	y += text_size.y + 3.f;
		//}
		//else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F7 > Non-Jinxed Players Hidden");
		//	y += text_size.y + 3.f;
		//}

		//if (FGInternal::ESP::correct_platforms_enabled) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F8 > Correct Platforms Shown");
		//	y += text_size.y + 3.f;
		//}
		//else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F8 > Correct Platforms Hidden");
		//	y += text_size.y + 3.f;
		//}

		////////////////////////

		//if (FGInternal::COLLISIONS::stun_collision) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F9 > Stun Collisions Disabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F9 > Stun Collisions Enabled");
		//	y += text_size.y + 3.f;
		//}
		//
		//if (FGInternal::COLLISIONS::object_collision) {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F10 > Object Collisions Disabled");
		//	y += text_size.y + 3.f;
		//} else {
		//	draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F10 > Object Collisions Enabled");
		//	y += text_size.y + 3.f;
		//}
	}
};
