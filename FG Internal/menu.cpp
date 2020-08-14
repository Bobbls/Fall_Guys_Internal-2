#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "draw_manager.h"
#include "render.h"

#include <mutex>

namespace FGInternal
{
	namespace ESP
	{
		bool correct_doors_enabled = false;
		bool correct_path_enabled = false;
		bool non_jinxed_players_enabled = false;
	};

	namespace OTHERS
	{
		bool fly_enabled = false;
		int fly_speed = 15;

		bool speed_enabled = false;
		int speed_boost = 12;

		bool dive_enabled = false;
		int dive_speed = 25;

		bool gravity_enabled = false;
		int gravity_scale = 2;

		bool stun_collision = false;
		bool object_collision = false;
	};
};

namespace FGInternalHelper
{
	bool disable_correct_doors = false;
	bool disable_correct_path = false;
	bool disable_non_jinxed_players = false;

	bool disable_fly = false;
	bool disable_speed = false;
	bool disable_dive = false;
	bool disable_gravity = false;
}

namespace menu {
	bool esp_tab_active = true;
	bool boost_tab_active = true;
	bool collisions_tab_active = true;

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
			draw_button("Correct Doors [ Doors Rush ]", FGInternal::ESP::correct_doors_enabled, &FGInternalHelper::disable_correct_doors);
			draw_button("Correct Path [ TipToe ]", FGInternal::ESP::correct_path_enabled, &FGInternalHelper::disable_correct_path);
			draw_button("Non-Jinxed Players", FGInternal::ESP::non_jinxed_players_enabled, &FGInternalHelper::disable_non_jinxed_players);
		}
		ImGui::End();

		ImGui::Begin("misc_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("Collisions", collisions_tab_active);
		if (collisions_tab_active) {
			draw_button("Disable Stuns", FGInternal::OTHERS::stun_collision);
			draw_button("Disable Objects Collisions", FGInternal::OTHERS::object_collision);
		}
		ImGui::End();

		ImGui::Begin("boosts_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab("Movement", boost_tab_active);
		if (boost_tab_active) {
			draw_button("Flying", FGInternal::OTHERS::fly_enabled, &FGInternalHelper::disable_fly);
			draw_slider("Flying Speed", &FGInternal::OTHERS::fly_speed, 10, 25);
			draw_button("Movement Speed (Default: 9.5)", FGInternal::OTHERS::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Speed Boost", &FGInternal::OTHERS::speed_boost, 5, 15);
			draw_button("Dive Boost (Default: 16.5)", FGInternal::OTHERS::dive_enabled, &FGInternalHelper::disable_dive);
			draw_slider("Dive Speed", &FGInternal::OTHERS::dive_speed, 15, 50);
			draw_button("Gravity Scale (Default: 1.5)", FGInternal::OTHERS::gravity_enabled, &FGInternalHelper::disable_gravity);
			draw_slider("Gravitation", &FGInternal::OTHERS::gravity_scale, 0, 5);
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

		if ((io.KeysDown[VK_F1] && !OldKeysDown[VK_F1]) || (io.NavInputs[ImGuiNavInput_Input] && !OldNavInputs[ImGuiNavInput_Input]))
		{
			FGInternal::OTHERS::fly_enabled = !FGInternal::OTHERS::fly_enabled;
			FGInternalHelper::disable_fly = !FGInternal::OTHERS::fly_enabled;
		}

		if (io.KeysDown[VK_F2] && !OldKeysDown[VK_F2])
		{
			FGInternal::OTHERS::speed_enabled = !FGInternal::OTHERS::speed_enabled;
			FGInternalHelper::disable_speed = !FGInternal::OTHERS::speed_enabled;
		}

		if (io.KeysDown[VK_F3] && !OldKeysDown[VK_F3])
		{
			FGInternal::OTHERS::dive_enabled = !FGInternal::OTHERS::dive_enabled;
			FGInternalHelper::disable_dive = !FGInternal::OTHERS::dive_enabled;
		}

		if (io.KeysDown[VK_F4] && !OldKeysDown[VK_F4])
		{
			FGInternal::OTHERS::gravity_enabled = !FGInternal::OTHERS::gravity_enabled;
			FGInternalHelper::disable_gravity = !FGInternal::OTHERS::gravity_enabled;
		}

		if (io.KeysDown[VK_F5] && !OldKeysDown[VK_F5])
		{
			FGInternal::ESP::correct_doors_enabled = !FGInternal::ESP::correct_doors_enabled;
			FGInternalHelper::disable_correct_doors = !FGInternal::ESP::correct_doors_enabled;
		}

		if (io.KeysDown[VK_F6] && !OldKeysDown[VK_F6])
		{
			FGInternal::ESP::correct_path_enabled = !FGInternal::ESP::correct_path_enabled;
			FGInternalHelper::disable_correct_path = !FGInternal::ESP::correct_path_enabled;
		}

		if (io.KeysDown[VK_F7] && !OldKeysDown[VK_F7])
		{
			FGInternal::ESP::non_jinxed_players_enabled = !FGInternal::ESP::non_jinxed_players_enabled;
			FGInternalHelper::disable_non_jinxed_players = !FGInternal::ESP::non_jinxed_players_enabled;
		}

		if (io.KeysDown[VK_F9] && !OldKeysDown[VK_F9])
		{
			FGInternal::OTHERS::stun_collision = !FGInternal::OTHERS::stun_collision;
		}

		if (io.KeysDown[VK_F10] && !OldKeysDown[VK_F10])
		{
			FGInternal::OTHERS::object_collision = !FGInternal::OTHERS::object_collision;
		}

		if (io.NavInputs[ImGuiNavInput_FocusPrev] > 0.f) 
		{
			FGInternal::OTHERS::speed_enabled = true;
			FGInternalHelper::disable_speed = false;
		}
		else if (io.NavInputs[ImGuiNavInput_FocusPrev] == 0.f && OldNavInputs[ImGuiNavInput_FocusPrev] > 0.f) 
		{
			FGInternal::OTHERS::speed_enabled = false;
			FGInternalHelper::disable_speed = true;
		}

		memcpy(OldKeysDown, io.KeysDown, 512 * sizeof(bool));
		memcpy(OldNavInputs, io.NavInputs, ImGuiNavInput_COUNT * sizeof(float));
	}

	void update_indicators() {
		auto text_size = draw_manager::calc_text_size(12, "TAKE");
		auto y = float(15);

		draw_manager::add_text_on_screen({ 5, y }, 0xFF374DCC, 14, "ESP Menu - INSERT");
		y += text_size.y + 4.f;

		if (FGInternal::OTHERS::fly_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F1 > Flying Enabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F1 > Flying Disabled");
			y += text_size.y + 3.f;
		}

		if (FGInternal::OTHERS::speed_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F2 > Movement Speed Enabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F2 > Movement Speed Disabled");
			y += text_size.y + 3.f;
		}

		if (FGInternal::OTHERS::dive_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F3 > Dive Speed Enabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F3 > Dive Speed Disabled");
			y += text_size.y + 3.f;
		}

		if (FGInternal::OTHERS::gravity_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F4 > Gravity Scale Enabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F4 > Gravity Scale Disabled");
			y += text_size.y + 3.f;
		}

/////////////////

		if (FGInternal::ESP::correct_doors_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F5 > Hide Correct Doors");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F5 > Show Correct Doors");
			y += text_size.y + 3.f;
		}

		if (FGInternal::ESP::correct_path_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F6 > Hide Correct Path");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F6 > Show Correct Path");
			y += text_size.y + 3.f;
		}

		if (FGInternal::ESP::non_jinxed_players_enabled)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F7 > Hide Non-Jinxed Players");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F7 > Show Non-Jinxed Players");
			y += text_size.y + 3.f;
		}

////////////////////////

		if (FGInternal::OTHERS::stun_collision)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F9 > Stun Collisions Disabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F9 > Stun Collisions Enabled");
			y += text_size.y + 3.f;
		}

		if (FGInternal::OTHERS::object_collision)
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF37CC5A, 12, "> F10 > Object Collisions Disabled");
			y += text_size.y + 3.f;
		}
		else
		{
			draw_manager::add_text_on_screen({ 5, y }, 0xFF7E3EDE, 12, "> F10 > Object Collisions Enabled");
			y += text_size.y + 3.f;
		}
	}
};
