#include "lib16868C/subsystems/chassis/inline.hpp"
#include "lemlib/util.hpp"
#include "lib16868C/devices/inertial.hpp"
#include "lib16868C/subsystems/chassis/motionProfiling.hpp"
#include "lib16868C/util/logger.hpp"
#include "lib16868C/util/math.hpp"
#include "lib16868C/util/util.hpp"
#include "okapi/api/util/mathUtil.hpp"

using namespace lib16868C;

Inline::Inline(MotorGroup& leftMtrs, MotorGroup& rightMtrs, Inertial& inertial, Odometry* odom, okapi::QLength wheelDiam, double gearRatio)
	: leftMtrs(leftMtrs), rightMtrs(rightMtrs), inertial(inertial), odom(odom), wheelDiam(wheelDiam), gearRatio(gearRatio) {

	tpr = leftMtrs.getGearing() == okapi::AbstractMotor::gearset::red ? 1800 : 
			leftMtrs.getGearing() == okapi::AbstractMotor::gearset::green ? 900 :
																			300;
	
	leftMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	rightMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
}

void Inline::moveTank(double left, double right, double slewRate) {
	if (slewRate > 0 && std::abs(left) - std::abs(prevLeft) > slewRate) left = prevLeft + slewRate * Util::sgn(left);
	if (slewRate > 0 && std::abs(right) - std::abs(prevRight) > slewRate) right = prevRight + slewRate * Util::sgn(right);
	prevLeft = left, prevRight = right;

	double max = std::max(std::abs(left), std::abs(right));
	if (max > MAX_VOLT) {
		left *= MAX_VOLT / max;
		right *= MAX_VOLT / max;
	}

	leftMtrs.moveVoltage(left);
	rightMtrs.moveVoltage(right);
}

void Inline::moveArcade(double forward, double turn, double slewRate) {
	double left = forward + turn;
	double right = forward - turn;

	moveTank(left, right, slewRate);
}

void Inline::driveTank(double left, double right, double deadzone) {
	if (std::abs(left) < deadzone) left = 0;
	if (std::abs(right) < deadzone) right = 0;
	moveTank(left * 12000, right * 12000);
}

void Inline::driveArcade(double forward, double turn, double deadzone) {
	if (std::abs(forward) < deadzone) forward = 0;
	if (std::abs(turn) < deadzone) turn = 0;
	moveArcade(forward * 12000, turn * 12000);
}

void Inline::moveDistance(okapi::QLength dist, okapi::QAngularSpeed maxRPM, PIDGains distGains, double maxAccelRPM, okapi::QAngle heading, okapi::QAngularSpeed turnRPM, PIDGains headingGains, int timeout) {
leftMtrs.tarePosition();
	rightMtrs.tarePosition();

	double maxVel = maxRPM.convert(okapi::rpm) * (wheelDiam * okapi::pi).convert(okapi::inch) / 60.0 / gearRatio;
	double maxAccel = maxAccelRPM * (wheelDiam * okapi::pi).convert(okapi::inch) / 60.0 / gearRatio;
	MotionProfile accelProfile = MotionProfiling::generateAccel(dist.convert(okapi::inch), {maxVel, maxAccel});

	PIDController distPID(distGains, 0, 0);
	PIDController headingPID(headingGains, 1, -1);

	double currDist = 0;

	int dir = Util::sgn(dist.convert(okapi::inch));

	int i = 0;
	uint32_t time = pros::millis();
	uint st = pros::millis(), t = pros::millis();
	while (dist.abs().convert(okapi::inch) - currDist > 0) {
		t = pros::millis();
		if (t - st > timeout && timeout > 0) {
			printDebug("[Inline Move Distance] Timeout: %d ms\n", t - st);
			break;
		}

		double avgTicks = std::abs((leftMtrs.getPosition() + rightMtrs.getPosition()) / 2.0);
		currDist = avgTicks / tpr * (wheelDiam * okapi::pi).convert(okapi::inch) * gearRatio;

		double distCtrl = distPID.calculate(dist.abs().convert(okapi::inch), std::abs(currDist));
		double headingCtrl = headingPID.calculate(heading.convert(okapi::degree), inertial.get_rotation(AngleUnit::DEG));

		double accelRPM = t - st < accelProfile.accelTime * 1000 ? accelProfile.profile[i++].velocity * 60.0 / (wheelDiam * okapi::pi).convert(okapi::inch) : maxRPM.convert(okapi::rpm);
		double decelRPM = maxRPM.convert(okapi::rpm) * distCtrl;
		double vel = std::min(maxRPM.convert(okapi::rpm), std::min(accelRPM, decelRPM));
		double volts = vel / static_cast<int>(leftMtrs.getGearing()) * 12000;
		double turnVolts = turnRPM.convert(okapi::rpm) / static_cast<int>(leftMtrs.getGearing()) * 12000;
		moveArcade(volts * dir, turnVolts * headingCtrl * (i / accelProfile.profile.size() + 0.5));

		pros::Task::delay_until(&time, 20);
	}

	moveTank(0, 0);
	double avgTicks = std::abs((leftMtrs.getPosition() + rightMtrs.getPosition()) / 2.0);
	currDist = avgTicks / tpr * (wheelDiam * okapi::pi).convert(okapi::inch) * gearRatio;
	printDebug("[Inline Move Distance] Finished with distance of %f\" with a heading of %f deg, taking %d ms\n", currDist, inertial.get_rotation(AngleUnit::DEG), pros::millis() - st);
}

void Inline::turnAbsolute(okapi::QAngle angle, okapi::QAngularSpeed maxRPM, lib16868C::PIDGains gains, double accelRate, double errorMargin, int numInMargin, TurnWheel turnWheel, int timeout) {
	PIDController turnPID(gains, 1, -1);

	double currAngle = inertial.get_rotation(AngleUnit::DEG);
	double accelVolts = 3000;
	uint st = pros::millis(), t = pros::millis();
	int inMargin = 0;
	while (inMargin < numInMargin) {
		t = pros::millis();
		if (t - st > timeout && timeout > 0) {
			printDebug("[Inline Turn Absolute] Timeout: %d\n", t - st);
			break;
		}

		currAngle = inertial.get_rotation(AngleUnit::DEG);

		if (std::abs(angle.convert(okapi::degree) - currAngle) < errorMargin) inMargin++;
		else inMargin = 0;

		double turnCtrl = turnPID.calculate(angle.convert(okapi::degree), currAngle);
		double maxVolts = maxRPM.convert(okapi::rpm) / static_cast<int>(leftMtrs.getGearing()) * 12000;
		accelVolts *= accelRate;
		double volts = std::min(maxVolts * std::abs(turnCtrl), accelVolts) * Util::sgn(turnCtrl);

		switch (turnWheel) {
			case TurnWheel::LEFT:
				rightMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
				moveTank(volts, 0);
				break;
			case TurnWheel::RIGHT:
				leftMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
				moveTank(0, -volts);
				break;
			case TurnWheel::BOTH:
				moveTank(volts, -volts);
				break;
		}

		pros::delay(20);
	}

	leftMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	rightMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	moveTank(0, 0);
	printDebug("[Inline Turn Absolute] Finished with heading of %f deg, taking %d ms\n", inertial.get_rotation(AngleUnit::DEG), pros::millis() - st);
}

void Inline::turnAbsolute(okapi::QAngle angle, okapi::QAngularSpeed maxRPM, PIDGains gains, TurnDirection turnDir, double errorMargin, int numInMargin, TurnWheel turnWheel, int timeout) {
	PIDController turnPID(gains);

	if (turnDir == TurnDirection::SHORTEST) {
		if (std::abs(angle.convert(okapi::degree) - inertial.get_rotation(AngleUnit::DEG)) < 180) turnDir = TurnDirection::COUNTER_CLOCKWISE;
		else turnDir = TurnDirection::CLOCKWISE;
	}

	double heading = inertial.get_rotation(AngleUnit::DEG);
	double target = angle.convert(okapi::degree);
	if (turnDir == TurnDirection::CLOCKWISE) target = ReduceAngle::reduce(target, heading + 360.0, heading);
	if (turnDir == TurnDirection::COUNTER_CLOCKWISE) target = ReduceAngle::reduce(target, heading, heading - 360.0);

	uint st = pros::millis(), t = pros::millis();
	int inMargin = 0;
	while (inMargin < numInMargin) {
		t = pros::millis();
		if (t - st > timeout && timeout > 0) {
			std::cout << "[Inline Turn Absolute] Timeout: " << t - st << std::endl;
			break;
		}

		heading = inertial.get_rotation(AngleUnit::DEG);

		if (std::abs(target - heading) < errorMargin) inMargin++;
		else inMargin = 0;

		double turnCtrl = turnPID.calculate(target, heading);
		double volts = maxRPM.convert(okapi::rpm) / static_cast<int>(leftMtrs.getGearing()) * 12000 * turnCtrl;

		switch (turnWheel) {
			case TurnWheel::LEFT:
				rightMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
				moveTank(-volts, 0, 6000);
				break;
			case TurnWheel::RIGHT:
				leftMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
				moveTank(0, volts, 6000);
				break;
			case TurnWheel::BOTH:
				moveTank(-volts, volts, 6000);
				break;
		}

		pros::delay(20);
	}

	leftMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	rightMtrs.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
	moveTank(0, 0);
	std::cout << "[Inline Turn Absolute] Finished with heading of " << inertial.get_rotation(AngleUnit::DEG) << " deg, taking " << pros::millis() - st << "ms" << std::endl;
}

void Inline::moveToPoint(Pose target, okapi::QAngularSpeed maxRPM, PIDGains distGains, PIDGains headingGains, okapi::QLength endRadius, bool backward, bool stopMtrs, int timeout) {
	if (!odom) {
		printError("[Inline Move to Point] No Odometry class was provided, unable to call moveToPoint\n");
		return;
	}

	PIDController distPID(distGains, 0, 0);
	PIDController headingPID(headingGains, 0, 0);

	Pose pose = odom->getPose();

	int dir = backward ? -1 : 1;

	uint st = pros::millis(), t = 0;
	int i = 0;
	while (pose.distTo(target) >= endRadius) {
		t = pros::millis();
		if (t - st > timeout && timeout > 0) {
			std::cout << "[Inline Move to Point] Timeout: " << t - st << std::endl;
			break;
		}

		pose = odom->getPose();
		double distLeft = pose.distTo(target).convert(okapi::inch);
		double heading = pose.angleTo(target).convert(okapi::radian);
		if(backward) heading = std::min(heading - 180, heading + 180);
		// printDebug("%f %f\n", *pose.x(), *pose.y());

		double distCtrl = distPID.calculate(distLeft);
		double headingErr = heading - inertial.get_rotation(AngleUnit::RAD);
		double headingCtrl = headingPID.calculate(headingErr);

		double headingDeadzone = M_PI_2 - std::atan2(0.5 * distLeft, endRadius.convert(okapi::inch));
		if (std::abs(heading) < std::abs(headingDeadzone)) headingCtrl = 0;

		double volts = maxRPM.convert(okapi::rpm) / static_cast<int>(leftMtrs.getGearing()) * 12000;
		printDebug("%f, %f, %f, %f, %f, %f, %f\n", *pose.x(), *pose.y(),  inertial.get_rotation(AngleUnit::DEG), heading * okapi::radianToDegree, headingCtrl, volts * distCtrl * dir * std::abs(std::cos(headingErr)), volts * headingCtrl);
		moveArcade(volts * distCtrl * dir * std::abs(std::cos(headingErr)), volts * headingCtrl, 6000);

		pros::delay(50);
	}

	if (stopMtrs) moveTank(0, 0);
	std::cout << "[Inline Move to Point] Finished with pose of " << odom->getPose().toStr() << ", taking " << pros::millis() - st << "ms" << std::endl;
}

double angleError(double target, double position) {
	target = std::fmod(std::fmod(target, 2 * M_PI) + 2 * M_PI, 2 * M_PI);
	target = std::fmod(std::fmod(target, 2 * M_PI) + 2 * M_PI, 2 * M_PI);
	return std::remainder(target - position, 2 * M_PI);
}
double getCurvature(Pose pose, Pose other) {
	double side = Util::sgn(sin(*pose.theta()) * (*other.x() - *pose.x()) - cos(*pose.theta()) * (*other.y() - *pose.y()));
	double a = -std::tan(*pose.theta());
	double c = std::tan(*other.theta()) * *pose.x() - *pose.y();
	double x = std::abs(a * *other.x() + *other.y() + c) / std::sqrt((a * a) + 1);
	double d = std::hypot(*other.x() - *pose.x(), *other.y() - *pose.y());
	return side * ((2 * x) / (d * d));
}
void Inline::moveToPose(Pose target, int timeout, PIDGains lateralGains, PIDGains angularGains, MoveToPoseParams params, bool async) {
	if (async) {
		pros::Task asyncTask([&]() {
			moveToPose(target, timeout, lateralGains, angularGains, params, false);
			pros::delay(10);
			return;
		});
	}

	if (!params.forward) target.setTheta(std::fmod(*target.theta() + M_PI, 2 * M_PI));

	PIDController lateralPID(lateralGains);
	PIDController angularPID(angularGains);

	Pose prevPose = odom->getPose();
	double distTravelled = 0;
	uint st = pros::millis();

	bool close = false;
	bool lateralSettled = false;
	bool prevSameSide = false;
	double prevLateralPower = 0;
	double prevAngularPower = 0;

	double lateralError = 0;
	double angularError = 0;
	
	while (pros::millis() - st < timeout && (!lateralSettled || std::abs(angularError) < params.angularSmallExitRange || !close)) {
		Pose pose = odom->getPose();

		distTravelled += pose.distTo(prevPose).convert(okapi::inch);
		prevPose = pose;

		const double distTarget = pose.distTo(target).convert(okapi::inch);
		if (distTarget < 7.5 && !close) {
			close = true;
			params.maxSpeed = std::max(std::abs(prevLateralPower), 60.0);
		}

		if (std::abs(lateralError) < params.angularSmallExitRange) lateralSettled = true;

		Pose carrot = target - Pose(cos(*target.theta()) * okapi::inch, sin(*target.theta()) * okapi::inch) * params.lead * distTarget;
		if (close) carrot = target;

		const bool robotSide = (*pose.y() - *target.y()) * -sin(*target.theta()) <= (*pose.x() - *target.x()) * cos(*target.theta()) + params.earlyExitRange;
		const bool carrotSide = (*carrot.y() - *target.y()) * -sin(*target.theta()) <= (*carrot.x() - *target.x()) * cos(*target.theta()) + params.earlyExitRange;
		const bool sameSide = robotSide == carrotSide;
		if (!sameSide && prevSameSide && close && params.minSpeed != 0) break;
		prevSameSide = sameSide;

		const double adjustedRobotTheta = params.forward ? *pose.theta() : *pose.theta() + M_PI;
		angularError = close ? angleError(adjustedRobotTheta, *target.theta()) : angleError(adjustedRobotTheta, pose.angleTo(carrot).convert(okapi::radian));
		lateralError = pose.distTo(carrot).convert(okapi::inch);
		if (close) lateralError *= cos(angleError(*pose.theta(), pose.angleTo(carrot).convert(okapi::radian)));
		else lateralError *= Util::sgn(cos(angleError(*pose.theta(), pose.angleTo(carrot).convert(okapi::radian))));

		double lateralPower = lateralPID.calculate(lateralError);
		double angularPower = angularPID.calculate(angularError);
		lateralPower = std::clamp(lateralPower, -params.maxSpeed, params.maxSpeed);
		angularPower = std::clamp(angularPower, -params.maxSpeed, params.maxSpeed);

		if (!close) lateralPower = lemlib::slew(lateralPower, prevLateralPower, params.lateralSlew);

		const double radius = 1 / std::abs(getCurvature(pose, carrot));
		const double maxSlipSpeed = std::sqrt(params.horizontalDrift * radius * 9.8);
		lateralPower = std::clamp(lateralPower, -maxSlipSpeed, maxSlipSpeed);

		const double overturn = std::abs(angularPower) + std::abs(lateralPower) - params.maxSpeed;
		if (overturn > 0) lateralPower -= lateralPower > 0 ? overturn : -overturn;

		if (params.forward && !close) lateralPower = std::max(lateralPower, 0.0);
		else if (!params.forward && !close) lateralPower = std::min(lateralPower, 0.0);

		if (params.forward && lateralPower < std::abs(params.minSpeed) && lateralPower > 0) lateralPower = std::abs(params.minSpeed);
		if (!params.forward && -lateralPower < std::abs(params.minSpeed) && lateralPower < 0) lateralPower = -std::abs(params.minSpeed);

		prevLateralPower = lateralPower;
		prevAngularPower = angularPower;

		moveTank(lateralPower + angularPower, lateralPower - angularPower);

		pros::delay(10);
	}

	moveTank(0, 0);
}

void Inline::setBrakeMode(okapi::AbstractMotor::brakeMode mode) {
	leftMtrs.setBrakeMode(mode);
	rightMtrs.setBrakeMode(mode);
}
void Inline::coast() {
	setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
}
void Inline::brake() {
	setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
}
void Inline::hold() {
	setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
}