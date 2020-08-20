#pragma once
#include <Windows.h>

namespace FGInternal {
	namespace GENERAL {
		extern bool hide_corner_text;
		extern bool disable_safe_values;
	};

	namespace ESP {
		extern bool correct_doors_enabled;
		extern bool correct_path_enabled;
		extern bool non_jinxed_players_enabled;
		extern bool show_all_platforms_enabled;
		extern bool show_player_with_tail;
	};

	namespace MOVEMENT {
		extern bool fly_enabled;
		extern float fly_speed;

		extern bool speed_enabled;
		extern float ms_normal_boost;
		extern float ms_carry_boost;
		extern float ms_grabbing_boost;

		extern bool dive_enabled;
		extern float normalDive_speed;
		extern float airDive_speed;

		extern bool gravity_enabled;
		extern float gravity_scale;
	};

	namespace COLLISIONS {
		extern bool stun_collision;
		extern bool object_collision;
		extern bool player_collision;
		extern bool falling_down;
	};

	namespace CARRY {
		extern bool carryDropForce;
		extern float carryNormalDropBoost;
		extern float carryDiveDropBoost;
		extern bool carryTussleChances;
	};

	namespace GRAB {
		extern bool supergrabfeature_enabled;

		extern bool grabDetectRadius;
		extern float grabDetectRadiusBoost;

		extern bool grabCheckDistance;
		extern float grabCheckDistanceBoost;

		extern bool grabMaxForce;
		extern float grabMaxForceBoost;

		extern bool grabBreakTime;
		extern float grabBreakTimeBoost;
		extern float grabBreakTimeJumpBoost;

		extern bool armLength;
		extern float armLengthBoost;

		extern bool grabCheckPredictionBase;
		extern float grabCheckPredictionBaseBoost;

		extern bool grabImmediateVelocityReduction;
		extern float grabImmediateVelocityReductionBoost;

		extern bool grabDragDirectionContribution;
		extern float grabDragDirectionContributionBoost;

		extern bool grabCooldown;
		extern float grabCooldownBoost;

		extern bool grabRegrabDelay;
		extern float grabRegrabDelayBoost;

		extern bool grabReleaseRegrabCooldown;
		extern float grabReleaseRegrabCooldownBoost;

		extern bool grabBreakAngle;
		extern float grabBreakAngleBoost;

		extern bool grabberVelocity;
		extern float grabberVelocityBoost;

		extern bool grabSeparationForce;
		extern float grabSeparationForceBoost;
	};
};

namespace FGInternalHelper {
	extern bool disable_correct_doors;
	extern bool disable_correct_path;
	extern bool disable_non_jinxed_players;
	extern bool disable_show_all_platforms;
	extern bool disable_show_player_with_tail;

	extern bool disable_fly;
	extern bool disable_speed;
	extern bool disable_dive;
	extern bool disable_gravity;

	extern bool disable_carryDropForce;
	extern bool disable_carryDiveDropForce;
	extern bool disable_carryTussleChances;

	extern bool disable_supergrabfeature;
};

namespace menu { 
	void draw();
	void update_keys();
	void update_indicators();
};