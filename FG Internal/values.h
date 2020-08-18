#include <Windows.h>
#include <iostream>

namespace VALUES {
	namespace SAFE_VALUES {
		namespace MOVEMENT {
			const float fly_speed_min = 10;
			const float fly_speed_max = 25;

			const float ms_normal_min = 5;
			const float ms_normal_max = 15;

			const float ms_carry_min = 5;
			const float ms_carry_max = 15;

			const float ms_grabbing_min = 5;
			const float ms_grabbing_max = 15;

			const float normalDive_speed_min = 15;
			const float normalDive_speed_max = 50;
			const float airDive_speed_min = 5;
			const float airDive_speed_max = 25;

			const float gravity_scale_min = 0;
			const float gravity_scale_max = 5;
		};

		namespace CARRY {
			const float carryNormalDropForce_min = 25;
			const float carryNormalDropForce_max = 75;

			const float carryDiveDropForce_min = 75;
			const float carryDiveDropForce_max = 125;
		};
	};

	namespace DEFAULT_VALUES {
		// MOVEMENT
		const float default_max_speed = 9.500000f;
		const float default_carryMaxSpeed = 8.000000f;
		const float default_grabbingMaxSpeed = 5.000000f;
		const float default_diveForce = 16.500000f;
		const float default_airDiveForce = 7.000000f;
		const float default_gravityScale = 1.500000f;

		// COLLISIONS
		const float default_anyCollisionStunForce = 28.000000f;
		const float default_dynamicCollisionStunForce = 10.000000f;
		const float default_CollisionThreshold = 14.000000f;

		// CARRY
		const float default_carryNormalDropForce = 50.000000f;
		const float default_carryDiveDropForce = 90.000000f;
	};
};