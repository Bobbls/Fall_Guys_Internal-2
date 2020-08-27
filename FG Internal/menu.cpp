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
		float fly_speed = 15.f;

		bool speed_enabled = false;
		float ms_normal_boost = 12.f;
		float ms_carry_boost = 10.f;
		float ms_grabbing_boost = 8.f;

		bool dive_enabled = false;
		float normalDive_speed = 25.f;
		float airDive_speed = 15.f;

		bool gravity_enabled = false;
		float gravity_scale = 2.f;
	};

	namespace COLLISIONS {
		bool stun_collision = false;
		bool object_collision = false;
		bool player_collision = false;
		bool falling_down = false;
	};

	namespace CARRY {
		bool carryDropForce = false;
		float carryNormalDropBoost = 0.f;
		float carryDiveDropBoost = 0.f;
		bool carryTussleChances = false;
	};

	namespace GRAB {
		bool supergrabfeature_enabled = false;

		bool grabDetectRadius = false;
		float grabDetectRadiusBoost = 10.f;
		
		bool grabCheckDistance = false;
		float grabCheckDistanceBoost = 5.f;

		bool grabMaxForce = false;
		float grabMaxForceBoost = 2.f;

		bool grabBreakTime = false;
		float grabBreakTimeBoost = 5.f;
		float grabBreakTimeJumpBoost = 0.01f;

		bool armLength = false;
		float armLengthBoost = 3.f;

		bool grabCheckPredictionBase = false;
		float grabCheckPredictionBaseBoost = 0.5f;

		bool grabImmediateVelocityReduction = false;
		float grabImmediateVelocityReductionBoost = 1.25f;

		bool grabDragDirectionContribution = false;
		float grabDragDirectionContributionBoost = 1.25f;

		bool grabCooldown = false;
		float grabCooldownBoost = 0.01f;

		bool grabRegrabDelay = false;
		float grabRegrabDelayBoost = 0.01f;

		bool grabReleaseRegrabCooldown = false;
		float grabReleaseRegrabCooldownBoost = 0.01f;

		bool grabBreakAngle = false;
		float grabBreakAngleBoost = 100;

		bool grabberVelocity = false;
		float grabberVelocityBoost = 1.f;

		bool grabSeparationForce = false;
		float grabSeparationForceBoost = 5.f;
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
	bool disable_carryTussleChances = false;

	bool disable_supergrabfeature = false;
};

namespace menu {
	bool general_tab_active = true;
	bool esp_tab_active = true;
	bool boost_tab_active = true;
	bool collisions_tab_active = true;
	bool carrying_tab_active = true;
	bool grab_tab_active = true;

	void push_color_for_button(bool active) {
		if (active) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 255.f, 194.f / 255.f, 64.f / 255.f, 1.f });
		} else {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 255.f, 194.f / 255.f, 64.f / 255.f, 1.f });
		}
	}

	void draw_button(const char* name, bool& config_key, bool* change_opositive = nullptr) {
		push_color_for_button(config_key);
		if (ImGui::Button(name, {244, 25}))
		{
			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
		}

		ImGui::PopStyleColor(3);
	}

	void draw_buttonDisabled(const char* name, bool config_key = false, bool* change_opositive = nullptr) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });

		if (!ImGui::Button(name, { 244, 25 }))
			return;

		ImGui::PopStyleColor(3);
	}

	void draw_slider(const char* name, float* val, float min, float max, const char* display_format = "%.2f") {
		if (!display_format)
			display_format = "%.2f";

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });

		ImGui::SetNextItemWidth(244.0f);
		ImGui::SliderFloat(name, val, min, max, display_format);

		ImGui::PopStyleColor(2);
	}

	void draw_tab(const char* name, bool& active) {
		ImGui::Text(name);
		ImGui::SameLine(237);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
		if (ImGui::Button("", { 10, 10 }))
			active = !active;

		ImGui::PopStyleColor(2);
	}

	std::once_flag init_style;
	void draw() {
		std::call_once(init_style, [&]() {
			auto& style = ImGui::GetStyle();
			style.WindowRounding = 0.f;
			style.FrameRounding = 0.f;
			style.ItemSpacing = ImVec2(8, 2);
			style.WindowPadding = ImVec2(3.f, 3.f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4{ 35.f / 255.f, 35.f / 255.f, 35.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4{ 46.f / 255.f, 46.f / 255.f, 46.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 46.f / 255.f, 46.f / 255.f, 46.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrab] = ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f };
			});

		ImGui::Begin("general_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» GENERAL TAB «", general_tab_active);
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
			ImGui::Text(u8"Unsafe Values » 0 - 2137");
			draw_button("Disable Safe Values", FGInternal::GENERAL::disable_safe_values);
		}
		ImGui::End();

		ImGui::Begin("visuals_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» VISUALS TAB «", esp_tab_active);
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
		draw_tab(u8"» COLLISIONS TAB «", collisions_tab_active);
		if (collisions_tab_active) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"Stuns/Knockdowns");
			draw_button("Disable Stuns/Knockdowns", FGInternal::COLLISIONS::stun_collision);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"Objects Collisions");
			draw_button("Disable Objects Collisions", FGInternal::COLLISIONS::object_collision);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"Player to Player Collisions");
			draw_button("Disable P2P Collisions", FGInternal::COLLISIONS::player_collision);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"Never Fall Down");
			draw_button("Disable Falling Down", FGInternal::COLLISIONS::falling_down);
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
			draw_button("Movement Speed Boost", FGInternal::MOVEMENT::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_min, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_max);
			draw_slider("Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_min, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_max);
			draw_slider("Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_min, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_max);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F3 » [CTRL/RMB] » Dive");
			draw_button("Dive Boost", FGInternal::MOVEMENT::dive_enabled, &FGInternalHelper::disable_dive);
			draw_slider("Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_max);
			draw_slider("Air Dive", &FGInternal::MOVEMENT::airDive_speed, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_max);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F4 » [SPACE] » Gravitation");
			ImGui::Text(u8"Lower Value - Lower Gravitation");
			draw_button("Gravity Scale", FGInternal::MOVEMENT::gravity_enabled, &FGInternalHelper::disable_gravity);
			draw_slider("Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_min, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_max);
		} else if (boost_tab_active && FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F1 » [SPACE/SHIFT] » Flying Mode");
			draw_button("Enable Flying", FGInternal::MOVEMENT::fly_enabled, &FGInternalHelper::disable_fly);
			draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, 0, 2137);
			ImGui::Dummy(ImVec2(0.0f, 5.0f));
			ImGui::Text(u8"F2 » [W] » Speed Hack");
			draw_button("Movement Speed Boost", FGInternal::MOVEMENT::speed_enabled, &FGInternalHelper::disable_speed);
			draw_slider("Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, 0, 2137);
			draw_slider("Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, 0, 2137);
			draw_slider("Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, 0, 2137);
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
		draw_tab(u8"» CARRYING ITEMS TAB «", carrying_tab_active);
		if (carrying_tab_active && !FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F10 » [SHIFT] » Drop Boost");
			draw_button("Item Drop", FGInternal::CARRY::carryDropForce, &FGInternalHelper::disable_carryDropForce);
			draw_slider("Normal Drop", &FGInternal::CARRY::carryNormalDropBoost, VALUES::SAFE_VALUES::CARRY::carryNormalDropForce_min, VALUES::SAFE_VALUES::CARRY::carryNormalDropForce_max);
			draw_slider("Dive Drop", &FGInternal::CARRY::carryDiveDropBoost, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_min, VALUES::SAFE_VALUES::CARRY::carryDiveDropForce_max);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F11 » Tussle When Grabbed");
			draw_button("Enable Equal Chances", FGInternal::CARRY::carryTussleChances, &FGInternalHelper::disable_carryTussleChances);
		}
		else if (carrying_tab_active && FGInternal::GENERAL::disable_safe_values) {
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F10 » [SHIFT] » Drop Boost");
			draw_button("Item Drop", FGInternal::CARRY::carryDropForce, &FGInternalHelper::disable_carryDropForce);
			draw_slider("Normal Drop", &FGInternal::CARRY::carryNormalDropBoost, 0, 2137);
			draw_slider("Dive Drop", &FGInternal::CARRY::carryDiveDropBoost, 0, 2137);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			ImGui::Text(u8"F11 » Tussle When Grabbed");
			draw_button("Enable Equal Chances", FGInternal::CARRY::carryTussleChances, &FGInternalHelper::disable_carryTussleChances);
		}
		ImGui::End();

		ImGui::Begin("grab_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 250, 0 }, ImGuiCond_Always);
		draw_tab(u8"» GRAB FEATURES TAB «", grab_tab_active);
		if (grab_tab_active && !FGInternal::GENERAL::disable_safe_values) {
			if (!FGInternal::GRAB::grabDetectRadius && !FGInternal::GRAB::grabCheckDistance && !FGInternal::GRAB::grabMaxForce
				&& !FGInternal::GRAB::grabBreakTime && !FGInternal::GRAB::armLength && !FGInternal::GRAB::grabCheckPredictionBase
				&& !FGInternal::GRAB::grabImmediateVelocityReduction && !FGInternal::GRAB::grabDragDirectionContribution
				&& !FGInternal::GRAB::grabCooldown && !FGInternal::GRAB::grabRegrabDelay
				&& !FGInternal::GRAB::grabReleaseRegrabCooldown && !FGInternal::GRAB::grabBreakAngle) {
				ImGui::Dummy(ImVec2(0.0f, 7.75f));
				draw_button(u8"F12 » Super Grab Feature", FGInternal::GRAB::supergrabfeature_enabled, &FGInternalHelper::disable_supergrabfeature);
				ImGui::Dummy(ImVec2(0.0f, 7.75f));
			}
			else {
				ImGui::Text(u8"Disable all minor functions to");
				ImGui::Text(u8"use Super Grab, which will remain");
				ImGui::Text(u8"hidden to prevent bugs.");
			}

			if (!FGInternal::GRAB::supergrabfeature_enabled) {
				draw_button("Grab Detect Radius", FGInternal::GRAB::grabDetectRadius);
				draw_slider("Radius", &FGInternal::GRAB::grabDetectRadiusBoost, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_min, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Check Distance", FGInternal::GRAB::grabCheckDistance);
				draw_slider("Distance", &FGInternal::GRAB::grabCheckDistanceBoost, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_min, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Max Force", FGInternal::GRAB::grabMaxForce);
				draw_slider("Force", &FGInternal::GRAB::grabMaxForceBoost, VALUES::SAFE_VALUES::GRAB::grabMaxForce_min, VALUES::SAFE_VALUES::GRAB::grabMaxForce_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Break Time", FGInternal::GRAB::grabBreakTime);
				draw_slider("Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTime_min, VALUES::SAFE_VALUES::GRAB::grabBreakTime_max);
				draw_slider("Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_min, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Arm Length", FGInternal::GRAB::armLength);
				draw_slider("Boost", &FGInternal::GRAB::armLengthBoost, VALUES::SAFE_VALUES::GRAB::armLength_min, VALUES::SAFE_VALUES::GRAB::armLength_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Check Prediction", FGInternal::GRAB::grabCheckPredictionBase);
				draw_slider("Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_min, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Velocity Reduction", FGInternal::GRAB::grabImmediateVelocityReduction);
				draw_slider("Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_min, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Direction Contribution", FGInternal::GRAB::grabDragDirectionContribution);
				draw_slider("Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_min, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Cooldown", FGInternal::GRAB::grabCooldown);
				draw_slider("Cooldown", &FGInternal::GRAB::grabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabCooldown_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Regrab Delay", FGInternal::GRAB::grabRegrabDelay);
				draw_slider("Delay", &FGInternal::GRAB::grabRegrabDelayBoost, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_min, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Release Regrab Cooldown", FGInternal::GRAB::grabReleaseRegrabCooldown);
				draw_slider("Regrab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Break Grab Angle", FGInternal::GRAB::grabBreakAngle);
				draw_slider("Angle", &FGInternal::GRAB::grabBreakAngleBoost, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_min, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_max);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Separation Force", FGInternal::GRAB::grabSeparationForce);
				draw_slider("Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_min, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_max);
			} else {
				ImGui::Text(u8"Super Grab uses all of the");
				ImGui::Text(u8"following features that will");
				ImGui::Text(u8"be hidden until you disable");
				ImGui::Text(u8"this feature to avoid bugs.");
			}
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			draw_button("Increase Grabber Velocity", FGInternal::GRAB::grabberVelocity);
			draw_slider("Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, VALUES::SAFE_VALUES::GRAB::grabberVelocity_min, VALUES::SAFE_VALUES::GRAB::grabberVelocity_max);
		} else if (grab_tab_active && FGInternal::GENERAL::disable_safe_values) {
			if (!FGInternal::GRAB::grabDetectRadius && !FGInternal::GRAB::grabCheckDistance && !FGInternal::GRAB::grabMaxForce
				&& !FGInternal::GRAB::grabBreakTime && !FGInternal::GRAB::armLength && !FGInternal::GRAB::grabCheckPredictionBase
				&& !FGInternal::GRAB::grabImmediateVelocityReduction && !FGInternal::GRAB::grabDragDirectionContribution
				&& !FGInternal::GRAB::grabCooldown && !FGInternal::GRAB::grabRegrabDelay
				&& !FGInternal::GRAB::grabReleaseRegrabCooldown && !FGInternal::GRAB::grabBreakAngle) {
				ImGui::Dummy(ImVec2(0.0f, 7.75f));
				draw_button(u8"F12 » Super Grab Feature", FGInternal::GRAB::supergrabfeature_enabled, &FGInternalHelper::disable_supergrabfeature);
				ImGui::Dummy(ImVec2(0.0f, 7.75f));
			} else {
				ImGui::Text(u8"Disable all minor functions to");
				ImGui::Text(u8"use Super Grab, which will remain");
				ImGui::Text(u8"hidden to prevent bugs.");
			}

			if (!FGInternal::GRAB::supergrabfeature_enabled) {
				draw_button("Grab Detect Radius", FGInternal::GRAB::grabDetectRadius);
				draw_slider("Radius", &FGInternal::GRAB::grabDetectRadiusBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Check Distance", FGInternal::GRAB::grabCheckDistance);
				draw_slider("Distance", &FGInternal::GRAB::grabCheckDistanceBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Max Force", FGInternal::GRAB::grabMaxForce);
				draw_slider("Force", &FGInternal::GRAB::grabMaxForceBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Break Time", FGInternal::GRAB::grabBreakTime);
				draw_slider("Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, 0, 2137);
				draw_slider("Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Arm Length", FGInternal::GRAB::armLength);
				draw_slider("Boost", &FGInternal::GRAB::armLengthBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Check Prediction", FGInternal::GRAB::grabCheckPredictionBase);
				draw_slider("Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Velocity Reduction", FGInternal::GRAB::grabImmediateVelocityReduction);
				draw_slider("Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Direction Contribution", FGInternal::GRAB::grabDragDirectionContribution);
				draw_slider("Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Cooldown", FGInternal::GRAB::grabCooldown);
				draw_slider("Cooldown", &FGInternal::GRAB::grabCooldownBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Regrab Delay", FGInternal::GRAB::grabRegrabDelay);
				draw_slider("Delay", &FGInternal::GRAB::grabRegrabDelayBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Release Regrab Cooldown", FGInternal::GRAB::grabReleaseRegrabCooldown);
				draw_slider("Regrab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, 0, 2137);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Break Grab Angle", FGInternal::GRAB::grabBreakAngle);
				draw_slider("Angle", &FGInternal::GRAB::grabBreakAngleBoost, 0, 360);
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				draw_button("Grab Separation Force", FGInternal::GRAB::grabSeparationForce);
				draw_slider("Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, 0, 2137);
			} else {
				ImGui::Text(u8"Super Grab uses all of the");
				ImGui::Text(u8"following features that will");
				ImGui::Text(u8"be hidden until you disable");
				ImGui::Text(u8"this feature to avoid bugs.");
			}
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
			draw_button("Increase Grabber Velocity", FGInternal::GRAB::grabberVelocity);
			draw_slider("Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, 0, 2137);
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

		if (io.KeysDown[VK_F10] && !OldKeysDown[VK_F10]) {
			FGInternal::CARRY::carryDropForce = !FGInternal::CARRY::carryDropForce;
			FGInternalHelper::disable_carryDropForce = !FGInternal::CARRY::carryDropForce;
		}

		if (io.KeysDown[VK_F11] && !OldKeysDown[VK_F11]) {
			FGInternal::CARRY::carryTussleChances = !FGInternal::CARRY::carryTussleChances;
			FGInternalHelper::disable_carryTussleChances = !FGInternal::CARRY::carryTussleChances;
		}

		if (io.KeysDown[VK_F12] && !OldKeysDown[VK_F12])
			FGInternal::GRAB::supergrabfeature_enabled = !FGInternal::GRAB::supergrabfeature_enabled;
			FGInternalHelper::disable_supergrabfeature = !FGInternal::GRAB::supergrabfeature_enabled;

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
		auto text_size = draw_manager::calc_text_size(12, "");
		auto y = float(15);

		if (!FGInternal::GENERAL::hide_corner_text) {
			draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 14, "FG Internal");
			y += text_size.y + 4.f;
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF363636, 12, u8"» INSERT » Open Cheat Menu");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF363636, 12, u8"» HOME » Hide Corner Text");
			y += text_size.y + 3.f;
		}
	}
};
