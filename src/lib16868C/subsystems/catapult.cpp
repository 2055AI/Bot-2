#include "okapi/api.hpp"
#include "lib16868C/subsystems/catapult.hpp"
#include "lib16868C/util/util.hpp"

using namespace lib16868C;

void lib16868C::CataMain(void* param) {
	Catapult* cata = (Catapult*) param;

	bool fireDebounce = false;
	bool fired = false;
	bool hasReset = false;

	uint32_t time = pros::millis();
	while (true) {
		if (cata->cataState == CataState::FIRING) {
			if (cata->enc.get() > 55 && !fireDebounce) {
				cata->cataState = CataState::SETTLED;
				fireDebounce = true;
			} else if (cata->enc.get() < 50 && fireDebounce) fireDebounce = false;
		} else if (cata->cataState == CataState::INTAKE) {
			if (cata->enc.get() > 35 && cata->enc.get() < 45 && hasReset) { cata->cataState = CataState::SETTLED; hasReset = false; }
		}
		if (cata->enc.get() < 10 && !fired) { fired = true; cata->numFired++; hasReset = true; }
		else if (cata->enc.get() > 30 && fired) fired = false;

		if (cata->cataState == CataState::MATCHLOAD) cata->mtrs.moveVoltage(11000);
		else if (cata->cataState != CataState::SETTLED) cata->mtrs.moveVoltage(11000);
		else cata->stop();

		pros::Task::delay_until(&time, 20);
	}
}

Catapult::Catapult(okapi::MotorGroup& mtrs, Rotation& enc) : mtrs(mtrs), enc(enc) {
	mtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	pros::c::task_create(CataMain, this, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Catapult Control");
}
Catapult::~Catapult() {
	stop();
}

void Catapult::fire() {
	cataState = CataState::FIRING;
}
void Catapult::intake() {
	if (enc.get() > 35 && enc.get() < 45) return;
	cataState = CataState::INTAKE;
}
void Catapult::matchload() {
	cataState = CataState::MATCHLOAD;
}
void Catapult::stop() {
	mtrs.moveVoltage(0);
	cataState = CataState::SETTLED;
}

void Catapult::moveVelocity(double vel) {	
	mtrs.moveVelocity(vel);
}

int Catapult::getNumFired() {
	return numFired;
}

bool Catapult::isSettled() {
	return cataState == CataState::SETTLED;
}

void Catapult::waitForSettled() {
	do pros::delay(10);
	while (!isSettled());
}