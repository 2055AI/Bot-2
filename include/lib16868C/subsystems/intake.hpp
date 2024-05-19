#pragma once
#include "okapi/api.hpp"
#include "lib16868C/devices/pneumatic.hpp"
#include "lib16868C/util/util.hpp"

namespace lib16868C {
enum class IntakeState {
	INTAKE,
	OUTTAKE,
	SHOOT,
	MATCHLOAD,
	OFF
};

class Intake {
	public:
		Intake(okapi::Motor& front, okapi::Motor& rear, okapi::DistanceSensor& distSnsr, Pneumatic& mouth, double slewRate);

		void spin(double power);
		void spinFront(double power);
		void spinRear(double power);

		void stop();

		void intake(bool blocking = false);
		void outtake(bool openMouth = true, int delay = 0, bool blocking = false);
		void shoot();
		void matchload();

		bool hasBall() const;

		IntakeState getState() const;

	private:
		okapi::Motor& front, rear;
		okapi::DistanceSensor& distSnsr;
		Pneumatic& mouth;

		IntakeState state = IntakeState::OFF;

		double frontTarget, rearTarget;
		double slewRate;

		pros::task_t intakeTask;

		friend void intakeSlewRate(void* param);
};

void intakeSlewRate(void* param);
} // namespace lib16868C