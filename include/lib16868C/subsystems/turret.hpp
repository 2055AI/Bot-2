#pragma once
#include "okapi/api.hpp"

namespace lib16868C {
enum class TargetMode {
	ABSOLUTE,
	RELATIVE
};

class Turret {
	public:
		Turret(okapi::Motor& motor, pros::Imu& inertial, double gearRatio);

		void spin(double vel);
		void spinTo(double angle, double vel);
		void stop();

		double getAngle();

		void setToRelativeTarget();
		void setToAbsoluteTarget();

	private:
		okapi::Motor& motor;
		pros::Imu& inertial;
		double gearRatio;

		pros::task_t turretTask;

		double targetAngle = 0;
		TargetMode targetMode = TargetMode::ABSOLUTE;

		friend void turretManager(void* param);
};

void turretManager(void* param);
} // namespace lib16868C