#include "motor.hpp"

using namespace lib16868C;

Motor::Motor(int port, okapi::AbstractMotor::gearset gearset)
	: okapi::Motor(std::abs(port), port < 0, gearset, okapi::AbstractMotor::encoderUnits::counts) {
	tpr = gearset == okapi::AbstractMotor::gearset::red ? 1800 :
			gearset == okapi::AbstractMotor::gearset::green ? 900 :
																300;
}
Motor::Motor(uint port, bool reversed, okapi::AbstractMotor::gearset gearset)
	: okapi::Motor(port, reversed, gearset, okapi::AbstractMotor::encoderUnits::counts) {
	tpr = gearset == okapi::AbstractMotor::gearset::red ? 1800 :
			gearset == okapi::AbstractMotor::gearset::green ? 900 :
																300;
}

double Motor::getPosition() {
	double ticks = okapi::Motor::getPosition();
	
	if (std::isinf(ticks)) {
		std::cerr << "[Motor] Position of Infinity" << std::endl;
		while (std::isinf(ticks)) { ticks = okapi::Motor::getPosition(); pros::delay(20); }
	}

	return ticks;
}
double Motor::get() {
	return getPosition();
}

void Motor::resetZero() {
	okapi::Motor::tarePosition();
}

double Motor::getVelocity() {
	return okapi::Motor::getActualVelocity();
}

void Motor::coast() {
	okapi::Motor::setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
}
void Motor::brake() {
	okapi::Motor::setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
}
void Motor::hold() {
	okapi::Motor::setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
}