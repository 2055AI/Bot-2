#include "lib16868C/devices/inertial.hpp"
#include "lib16868C/util/logger.hpp"
#include "lib16868C/util/util.hpp"

using namespace lib16868C;

Inertial::Inertial(uint port) : pros::Imu(port) {}

void Inertial::calibrate() {
	uint st = pros::millis();
	bool resetSuccess = true;
	do {
		if (!resetSuccess) {
			// Inertial failed to reset
			printError("[Inertial] Inertial Reset Failed\n");
			pros::lcd::print(0, "Inertial Reset Failed");
		}
		resetSuccess = reset(true) - 1;
		pros::delay(10);
	} while (!resetSuccess);

	double h1 = get_rotation(AngleUnit::DEG);
	pros::delay(500);
	double h2 = get_rotation(AngleUnit::DEG);
	if (std::abs(h1 - h2) > DRIFT_THRESHOLD) {
		// Inertial reading has changed too much in 500 ms
		printError("[Inertial] Inertial Drift Detected: %f deg difference in 500 ms\n", std::abs(h1 - h2));
		pros::lcd::print(0, "Inertial Drift Detected: %f deg difference in 500ms", std::abs(h1 - h2));
	}
	printDebug("[Inertial] Calibration time: %d ms\n", pros::millis() - st);
}

double Inertial::get_rotation(AngleUnit unit) const {
double a = pros::Imu::get_rotation();
	if (std::isinf(a)) {
		printError("[Inertial] Reading of Infinity\n");
		while (std::isinf(a)) {
      a = pros::Imu::get_rotation();
      pros::delay(20);
    }
	}

	if (unit == AngleUnit::RAD) a = Util::degToRad(a);
	return a;
}
void Inertial::set_rotation(okapi::QAngle heading) {
	pros::Imu::set_rotation(heading.convert(okapi::degree));
}