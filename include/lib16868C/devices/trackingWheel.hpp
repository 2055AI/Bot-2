#pragma once
#include "okapi/api.hpp"
#include "lib16868C/devices/motor.hpp"
#include "lib16868C/devices/motorGroup.hpp"
#include "lib16868C/devices/opticalEncoder.hpp"
#include "lib16868C/devices/rotation.hpp"

namespace lib16868C {
enum class TrackingWheelType {
	ROTATION = 0,
	OPTICAL_ENCODER = 1,
	MOTOR = 2,
	MOTOR_GROUP = 3,
	INVALID = -1
};

class TrackingWheel {
public:
	TrackingWheel();
	TrackingWheel(lib16868C::Rotation* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio = 1);
	TrackingWheel(lib16868C::OpticalEncoder* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio = 1);
	TrackingWheel(lib16868C::Motor* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio = 1);
	TrackingWheel(lib16868C::MotorGroup* enc, okapi::QLength wheelDiameter, okapi::QLength offset, double gearRatio = 1);

	double getRaw() const;
	double getDist() const;
	double getOffset() const;
	void reset();

	TrackingWheelType getType() const;

private:
	AbstractEncoder* enc = nullptr;

	double wheelDiameter = 0;
	double offset = 0;
	double gearRatio = 1;

	TrackingWheelType type = TrackingWheelType::INVALID;
};
} // namespace lib16868C