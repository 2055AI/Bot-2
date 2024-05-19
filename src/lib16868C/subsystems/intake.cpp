#include "lib16868C/subsystems/intake.hpp"
#include "lib16868C/util/logger.hpp"

using namespace lib16868C;

void lib16868C::intakeSlewRate(void* param) {
	Intake* intake = static_cast<Intake*>(param);
	
	uint32_t time = pros::millis();
	double frontVel = 0, rearVel = 0;
	while (true) {
		double frontVolts = intake->front.getActualVelocity() / static_cast<double>(intake->front.getGearing()) * 12000;
		double rearVolts = intake->rear.getActualVelocity() / static_cast<double>(intake->rear.getGearing()) * 12000;
		if (std::abs(intake->frontTarget - frontVolts) > intake->slewRate && (std::abs(frontVolts) < std::abs(intake->frontTarget) || Util::sgn(frontVolts) != Util::sgn(intake->frontTarget)))
			frontVel += intake->slewRate * Util::sgn(intake->frontTarget - frontVolts);
		else {
			frontVel = intake->frontTarget;
		}
		if (std::abs(intake->rearTarget - rearVolts) > intake->slewRate && (std::abs(rearVolts) < std::abs(intake->rearTarget) || Util::sgn(rearVolts) != Util::sgn(intake->rearTarget)))
			rearVel += intake->slewRate * Util::sgn(intake->rearTarget - rearVolts);
		else {
			rearVel = intake->rearTarget;
		}

		frontVel = std::clamp(frontVel, -12000.0, 12000.0);
		rearVel = std::clamp(rearVel, -12000.0, 12000.0);

		intake->front.moveVoltage(frontVel);
		intake->rear.moveVoltage(rearVel);

		pros::Task::delay_until(&time, 50);
	}
}

Intake::Intake(okapi::Motor& front, okapi::Motor& rear, okapi::DistanceSensor& distSnsr, Pneumatic& mouth, double slewRate)
	: front(front), rear(rear), distSnsr(distSnsr), mouth(mouth), slewRate(slewRate) {
	intakeTask = pros::c::task_create(intakeSlewRate, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Intake Manager");
}

void Intake::spin(double volts) {
	frontTarget = rearTarget = volts;
}
void Intake::spinFront(double volts) {
	frontTarget = volts;
}
void Intake::spinRear(double volts) {
	rearTarget = volts;
}

void Intake::stop() {
	frontTarget = rearTarget = 0;
	front.moveVoltage(0);
	rear.moveVoltage(0);
	state = IntakeState::OFF;
}

void Intake::intake(bool blocking) {
	if (state == IntakeState::INTAKE && !blocking) return;
	state = IntakeState::INTAKE;

	if (blocking) {
		mouth.retract();
		frontTarget = 5000;
		rearTarget = -12000;
		
		while (!hasBall()) {
			if (state != IntakeState::INTAKE) return;
			pros::delay(50);
		}
		stop();
		printDebug("[Intake Intake] Triball intaked at a distance of %f mm\n", distSnsr.get());
	} else pros::Task intake([&] {
		this->intake(true);
	});
}
void Intake::outtake(bool openMouth, int delay, bool blocking) {
	if (state == IntakeState::OUTTAKE && !blocking) return;
	state = IntakeState::OUTTAKE;

	frontTarget = rearTarget = -12000;

	if (openMouth) {
		if (blocking) {
			while (hasBall()) {
				if (state != IntakeState::OUTTAKE) return;
				pros::delay(50);
			}

			pros::delay(delay);
			mouth.extend();
			stop();
			printDebug("[Intake Outtake] Triball outtaked at a distance of %f mm\n", distSnsr.get());
		} else pros::Task outtake([&] {
			this->outtake(true, delay, true);
		});
	}
}
void Intake::shoot() {
	state = IntakeState::SHOOT;
	frontTarget = rearTarget = 12000;
}
void Intake::matchload() {
	state = IntakeState::MATCHLOAD;
	frontTarget = 12000;
	rearTarget = -12000;
}

bool Intake::hasBall() const  {
	return distSnsr.get() < 150;
}
IntakeState Intake::getState() const {
	return state;
}