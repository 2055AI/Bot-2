#pragma once
#include "okapi/api.hpp"
#include "lib16868C/devices/abstractEncoder.hpp"
#include "lib16868C/devices/motor.hpp"
#include "lib16868C/util/math.hpp"

namespace lib16868C {
class MotorGroup : public AbstractEncoder {
	public:
	MotorGroup(std::initializer_list<Motor> mtrs);
	MotorGroup(std::vector<Motor> mtrs);

	void moveVoltage(double volts);
	void moveVelocity(double vel);

	double getPosition();
	double get() override;

	void tarePosition();
	void resetZero() override;

	double getActualVelocity();
	double getVelocity() override;
	double getTemperature();

	okapi::AbstractMotor::gearset getGearing();

	void setBrakeMode(okapi::AbstractMotor::brakeMode mode);

	int getSize();

	private:
	std::vector<Motor> mtrs;
};
} // namespace lib16868C