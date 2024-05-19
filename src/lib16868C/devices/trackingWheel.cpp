#include "lib16868C/devices/trackingWheel.hpp"
#include "lib16868C/util/util.hpp"

using namespace lib16868C;

TrackingWheel::TrackingWheel() {}
TrackingWheel::TrackingWheel(Rotation* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio) : enc(enc) {
	this->wheelDiameter = wheelDiameter.convert(okapi::inch);
	this->offset = offset.convert(okapi::inch);
	this->gearRatio = gearRatio;
	
	type = TrackingWheelType::ROTATION;
}
TrackingWheel::TrackingWheel(OpticalEncoder* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio) : enc(enc) {
	this->wheelDiameter = wheelDiameter.convert(okapi::inch);
	this->offset = offset.convert(okapi::inch);
	this->gearRatio = gearRatio;

	type = TrackingWheelType::OPTICAL_ENCODER;
}
TrackingWheel::TrackingWheel(Motor* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio) : enc(enc) {
	this->wheelDiameter = wheelDiameter.convert(okapi::inch);
	this->offset = offset.convert(okapi::inch);
	this->gearRatio = gearRatio;

	type = TrackingWheelType::MOTOR;
}
TrackingWheel::TrackingWheel(MotorGroup* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio) : enc(enc) {
	this->wheelDiameter = wheelDiameter.convert(okapi::inch);
	this->offset = offset.convert(okapi::inch);
	this->gearRatio = gearRatio;

	type = TrackingWheelType::MOTOR_GROUP;
}

double TrackingWheel::getDist() const {
	if (!enc) return 0;
	return (getRaw() / enc->getTPR()) * (wheelDiameter * M_PI) * gearRatio;
}
double TrackingWheel::getRaw() const {
	return enc ? enc->get() : 0;
}
double TrackingWheel::getOffset() const {
	return offset;
}
void TrackingWheel::reset() {
	if (enc) enc->resetZero();
}

TrackingWheelType TrackingWheel::getType() const {
	return type;
}