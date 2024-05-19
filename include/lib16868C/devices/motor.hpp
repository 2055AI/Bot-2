#pragma once
#include "okapi/api.hpp"
#include "lib16868C/devices/abstractEncoder.hpp"
#include "lib16868C/util/math.hpp"

namespace lib16868C {
class Motor : public okapi::Motor, public AbstractEncoder {
	public:
	Motor(int port, okapi::AbstractMotor::gearset gearset);
	Motor(uint port, bool reverse, okapi::AbstractMotor::gearset gearset);

	double getPosition() override;
	double get() override;

	void resetZero() override;

	double getVelocity() override;

	void coast();
	void brake();
	void hold();
};
} // namespace lib16868C