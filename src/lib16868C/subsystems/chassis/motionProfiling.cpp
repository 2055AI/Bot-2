#include "lib16868C/subsystems/chassis/motionProfiling.hpp"

using namespace lib16868C;

/** MotionLimit **/
MotionLimit MotionLimit::operator*(double factor) {
	return {maxVel * factor, maxAccel * factor};
}
MotionLimit MotionLimit::operator/(double divisor) {
	return {maxVel / divisor, maxAccel / divisor};
}

/** MotionProfile **/
MotionData MotionProfile::operator[](int i) {
	return profile[i];
}
MotionProfile MotionProfile::operator+=(const MotionProfile& other) {
	profile.insert(profile.end(), other.profile.begin(), other.profile.end());
	return *this;
}

/** MotionProfiling **/
MotionProfile MotionProfiling::generateAccel(double dist, MotionLimit motionLimit) {
	MotionProfile profile;

	double accelTime = motionLimit.maxVel / motionLimit.maxAccel;
	profile.accelTime = accelTime;

	double time = 0;
	while (time <= accelTime) {
		MotionData data;
		data.time = time;
		data.distance = 0.5 * motionLimit.maxAccel * time * time;
		data.velocity = motionLimit.maxAccel * time;
		data.acceleration = motionLimit.maxAccel;

		profile.profile.push_back(data);
		time += 0.02;
	}

	return profile;
}

MotionProfile MotionProfiling::generateTrapezoidal(double dist, MotionLimit motionLimit) {
	MotionProfile profile;

	double accelTime = motionLimit.maxVel / motionLimit.maxAccel;
	double accelDist = 0.5 * motionLimit.maxAccel * accelTime * accelTime;

	double trapezoidalDist = std::abs(dist);
	double maxDist = trapezoidalDist - 2 * accelDist;

	if (maxDist < 0) {
		accelTime = sqrt(fabs(dist) / motionLimit.maxAccel);
		accelDist = 0.5 * motionLimit.maxAccel * accelTime * accelTime;
		trapezoidalDist = 2 * accelDist;
		maxDist = 0;
	}
	profile.accelTime = accelTime;

	double endAccel = accelTime;
	double endMax = endAccel + std::abs(maxDist / motionLimit.maxVel);
	double endDecel = endMax + accelTime;

	int dir = Util::sgn(dist);
	motionLimit = motionLimit * dir;

	double time = 0;
	while (time <= endDecel) {
		MotionData data;
		data.time = time;

		if (time <= endAccel) {
			data.distance = 0.5 * motionLimit.maxAccel * time * time;
			data.velocity = motionLimit.maxAccel * time;
			data.acceleration = motionLimit.maxAccel;
		} else if (time <= endMax) {
			data.distance = accelDist + motionLimit.maxVel * (time - accelTime);
			data.velocity = motionLimit.maxVel;
			data.acceleration = 0;
		} else if (time <= endDecel) {
			double timeLeft = endDecel - time;
			data.distance = trapezoidalDist - 0.5 * motionLimit.maxAccel * timeLeft * timeLeft;
			data.velocity = motionLimit.maxAccel * timeLeft;
			data.acceleration = -motionLimit.maxAccel;
		} else {
			data.distance = trapezoidalDist;
			data.velocity = 0;
			data.acceleration = 0;
		}

		profile.profile.push_back(data);
		time += 0.02;
	}

	return profile;
}