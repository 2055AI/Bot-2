#include "lib16868C/devices/opticalEncoder.hpp"

using namespace lib16868C;

OpticalEncoder::OpticalEncoder(const OpticalEncoder& enc) : okapi::ADIEncoder(enc) {
	tpr = enc.tpr;
}
OpticalEncoder::OpticalEncoder(int portTop, int portBottom, bool reversed)
							: okapi::ADIEncoder(portTop, portBottom, reversed) {
	tpr = 360;
}
OpticalEncoder::OpticalEncoder(int smartPort, int portTop, int portBottom, bool reversed)
							: okapi::ADIEncoder({smartPort, portTop, portBottom}, reversed) {
	tpr = 360;
}

double OpticalEncoder::get() {
	return okapi::ADIEncoder::get();
}
void OpticalEncoder::resetZero() {
	okapi::ADIEncoder::reset();
}
double OpticalEncoder::getVelocity() {
	double dTicks = get() - prevTicks;
	double dT = pros::millis() - prevTime;
	prevTicks = get();
	prevTime = pros::millis();
	return dTicks / dT * 1000; // Multiply by 1000 to convert from ticks/ms to ticks/s
}