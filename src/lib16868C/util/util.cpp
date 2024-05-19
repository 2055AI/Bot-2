#include "lib16868C/util/util.hpp"

using namespace lib16868C;

double Util::degToRad(double deg) {
	return deg * M_PI / 180;
}
double Util::radToDeg(double rad) {
	return rad * 180 / M_PI;
}

double Util::mToIn(double m) {
	return m * 39.3701;
}
double Util::inToM(double in) {
	return in / 39.3701;
}

void Util::runAsBlocking(std::function<void()> fn, std::function<bool()> endCond, int timeout, int pollRate, int paddingDelay) {
	fn();
	Util::blocking(endCond, timeout, pollRate, paddingDelay);
}
void Util::blocking(std::function<bool()> endCond, int timeout, int pollRate, int paddingDelay) {
	pros::delay(paddingDelay);
	int st = pros::millis();
	uint32_t time = pros::millis();
	while (!endCond()) {
		if (timeout != -1 && pros::millis() - st > timeout) return;
		pros::Task::delay_until(&time, pollRate);
	}
	pros::delay(paddingDelay);
}

pros::Task Util::runAsync(std::function<void()> fn) {
	return pros::Task(fn, "Async Task");
}