#pragma once
#include "lib16868C/util/util.hpp"
#include <vector>

namespace lib16868C {
struct MotionLimit {
	double maxVel;
	double maxAccel;

	MotionLimit operator*(double factor);
	MotionLimit operator/(double divisor);
};

struct MotionData {
	double time;
	double distance;
	double velocity;
	double acceleration;
};

struct MotionProfile {
	std::vector<MotionData> profile;
	double accelTime;

	MotionData operator[](int i);

	MotionProfile operator+=(const MotionProfile& other);
};

class MotionProfiling {
	public:
		static MotionProfile generateAccel(double dist, MotionLimit motionLimit);
		static MotionProfile generateTrapezoidal(double dist, MotionLimit motionLimit);
};
} // namespace lib16868C