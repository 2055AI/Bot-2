#include "motorGroup.hpp"

using namespace lib16868C;

MotorGroup::MotorGroup(std::initializer_list<Motor> mtrs) : mtrs(mtrs) {
	if (mtrs.size() == 0) std::cerr << "[MotorGroup] No motors in group" << std::endl;

	tpr = this->mtrs[0].getTPR();
}
MotorGroup::MotorGroup(std::vector<Motor> mtrs) : mtrs(mtrs) {
	if (mtrs.size() == 0) std::cerr << "[MotorGroup] No motors in group" << std::endl;

	tpr = this->mtrs[0].getTPR();
}

void MotorGroup::moveVoltage(double volts) {
	for (Motor& mtr : mtrs) mtr.moveVoltage(volts);
}
void MotorGroup::moveVelocity(double vel) {
	for (Motor& mtr : mtrs) mtr.moveVelocity(vel);
}

double MotorGroup::getPosition() {
	return mtrs[0].getPosition();
}
double MotorGroup::get() {
	return getPosition();
}

void MotorGroup::tarePosition() {
	for (Motor& mtr : mtrs) mtr.tarePosition();
}
void MotorGroup::resetZero() {
	tarePosition();
}

okapi::AbstractMotor::gearset MotorGroup::getGearing() {
	return mtrs[0].getGearing();
}

double MotorGroup::getActualVelocity() {
	return mtrs[0].getActualVelocity();
}
double MotorGroup::getVelocity() {
	return getActualVelocity();
}
double MotorGroup::getTemperature() {
	double mx = 0;
	for (auto& m : mtrs) mx = std::max(mx, m.getTemperature());
	return mx;
}

void MotorGroup::setBrakeMode(okapi::AbstractMotor::brakeMode mode) {
	for (Motor& mtr : mtrs) mtr.setBrakeMode(mode);
}

int MotorGroup::getSize() {
	return mtrs.size();
}