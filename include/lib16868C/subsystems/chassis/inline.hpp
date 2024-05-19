#pragma once
#include "okapi/api.hpp"
#include "lib16868C/controllers/pidController.hpp"
#include "lib16868C/devices/inertial.hpp"
#include "lib16868C/devices/motorGroup.hpp"
#include "lib16868C/subsystems/chassis/odometry.hpp"
#include "lib16868C/util/math.hpp"
#include "lib16868C/util/pose.hpp"

namespace lib16868C {
enum class TurnWheel {
	LEFT,
	RIGHT,
	BOTH
};
enum class TurnDirection {
	CLOCKWISE = -1,
	COUNTER_CLOCKWISE = 1,
	SHORTEST = 0
};

struct MoveToPoseParams {
	bool forward = true;
	double horizontalDrift = 2;
	double lead = 0.6;
	double maxSpeed = 127;
	double minSpeed = 0;
	double earlyExitRange = 0;
	double lateralLargeExitRange = 0;
	double lateralSmallExitRange = 0;
	double angularLargeExitRange = 0;
	double angularSmallExitRange = 0;
	double lateralSlew = 0;
};

class Inline {
	public:
		Inline(MotorGroup& left, MotorGroup& right, Inertial& inertial, Odometry* odom, okapi::QLength wheelDiam, double gearRatio = 1.0);

		void moveTank(double left, double right, double slewRate = 0);
		void moveArcade(double forward, double turn, double slewRate = 0);

		void driveTank(double left, double right, double deadzone = 0);
		void driveArcade(double forward, double turn, double deadzone = 0);

		void moveDistance(okapi::QLength dist, okapi::QAngularSpeed maxRPM, PIDGains distGains, double accel, okapi::QAngle heading, okapi::QAngularSpeed turnRPM, PIDGains headingGains, int timeout = 0);
		void turnAbsolute(okapi::QAngle angle, okapi::QAngularSpeed maxRPM, PIDGains gains, double accelRate = 1.03, double errorMargin = 1, int numInMargin = 5, TurnWheel turnWheel = TurnWheel::BOTH, int timeout = 0);

		void turnAbsolute(okapi::QAngle angle, okapi::QAngularSpeed maxRPM, PIDGains gains, TurnDirection turnDir = TurnDirection::SHORTEST, double errorMargin = 3, int numInMargin = 5, TurnWheel turnWheel = TurnWheel::BOTH, int timeout = 0);
		void moveToPoint(Pose target, okapi::QAngularSpeed maxRPM, PIDGains distGains, PIDGains headingGains, okapi::QLength endRadius, bool backward = false, bool stopMtrs = false, int timeout = 0);

		void moveToPose(Pose target, int timeout, PIDGains lateralGains, PIDGains angularGains, MoveToPoseParams params, bool async);

		void setBrakeMode(okapi::AbstractMotor::brakeMode mode);
		void coast();
		void brake();
		void hold();

	private:
		MotorGroup& leftMtrs, rightMtrs;
		Inertial& inertial;
		Odometry* odom { nullptr };
		okapi::QLength wheelDiam;
		double gearRatio;
		double tpr;

		// Move Slew Rate Members
		double prevLeft = 0, prevRight = 0;

		const int MAX_VOLT = 12000;
};
} // namespace lib16868C