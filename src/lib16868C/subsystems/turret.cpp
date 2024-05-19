#include "turret.hpp"
#include "lib16868C/controllers/pidController.hpp"
#include "lib16868C/util/util.hpp"

using namespace lib16868C;

void lib16868C::turretManager(void* param) {
	Turret* turret = static_cast<Turret*>(param);
	turret->motor.tarePosition();

	PIDController pid({0.004, 0, 0}, 1, -1);

	int prevTarget = 0;
	uint32_t time = pros::millis();
	while (true) {
		if (turret->inertial.get_rotation() == INFINITY) continue;

		if (turret->targetMode == TargetMode::RELATIVE)
			turret->targetAngle = turret->inertial.get_rotation();
		else
			turret->targetAngle = -turret->inertial.get_rotation();

		// int reducedAngle = (targetAngle % 360 + 360) % 360;
		// if (reducedAngle > 170 && reducedAngle < 190) {
		// 	if (prevTarget + targetAngle > 0) {
		// 		if (targetAngle > 0) targetAngle -= reducedAngle + 170;
		// 		else targetAngle += reducedAngle - 170;
		// 	} else {
		// 		if (targetAngle > 0) targetAngle -= reducedAngle + 190;
		// 		else targetAngle += reducedAngle - 190;
		// 	}
		// }
		// prevTarget = -targetAngle;

		double targetTicks = turret->targetAngle / 360 * 300 / turret->gearRatio;
		
		double error = targetTicks - turret->motor.getPosition();
		double sgnl = pid.calculate(error);

		turret->spin(600 * sgnl);
		
		pros::Task::delay_until(&time, 100);
	}
}

Turret::Turret(okapi::Motor& motor, pros::Imu& inertial, double gearRatio)
	: motor(motor), inertial(inertial), gearRatio(gearRatio) {
	// turretTask = pros::c::task_create(turretManager, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Turret");
}

void Turret::spin(double vel) {
	motor.moveVoltage(vel / static_cast<double>(motor.getGearing()) * 12000);
}
void Turret::spinTo(double angle, double vel) {
	targetAngle = angle;
}

void Turret::setToRelativeTarget() {
	targetMode = TargetMode::RELATIVE;
}
void Turret::setToAbsoluteTarget() {
	targetMode = TargetMode::ABSOLUTE;
}