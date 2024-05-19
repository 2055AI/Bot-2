#pragma once
#include "lib16868C/devices/inertial.hpp"
#include "lib16868C/devices/trackingWheel.hpp"
#include "lib16868C/util/math.hpp"
#include "lib16868C/util/pose.hpp"
#include <array>

using namespace okapi::literals;

namespace lib16868C {
struct DistanceSensor {
	okapi::DistanceSensor* snsr { nullptr };
	double offset = 0;

	DistanceSensor();
	DistanceSensor(okapi::DistanceSensor* snsr, okapi::QLength offset);

	double getDist() const;
	double getConfidence() const;
};

class Odometry {
	public:
		Odometry();
		Odometry(std::array<TrackingWheel, 3> trackingWheels, std::array<DistanceSensor, 4> distanceSensors, Inertial* inertial);
		Odometry(TrackingWheel left, TrackingWheel right, TrackingWheel middle);
		Odometry(TrackingWheel left, TrackingWheel right, TrackingWheel middle, Inertial* inertial);
		Odometry(DistanceSensor front, DistanceSensor right, DistanceSensor rear, DistanceSensor left, Inertial* inertial);
		Odometry(Odometry& odom);

		void init();
		void init(Pose pose);

		Pose getPose();
		Pose getState();

		void update(bool front, bool right, bool rear, bool left);
		void update(Pose pose);

		std::array<TrackingWheel*, 3> getEncoders() const;
		std::array<DistanceSensor*, 4> getDistanceSensors() const;
		Inertial* getInertial() const;
		
		void resetSensors();

	private:
		Pose pose { 0_in, 0_in, 0_deg, 0 };
		pros::Mutex poseMutex;
		Pose prevPose { 0_in, 0_in, 0_deg, 0 };

		TrackingWheel leftEnc;
		TrackingWheel rightEnc;
		TrackingWheel middleEnc;
		std::array<TrackingWheel*, 3> trackingWheels = { &leftEnc, &rightEnc, &middleEnc };

		DistanceSensor frontDist;
		DistanceSensor rightDist;
		DistanceSensor rearDist;
		DistanceSensor leftDist;
		std::array<DistanceSensor*, 4> distanceSensors = { &frontDist, &leftDist, &rearDist, &rightDist };

		Inertial* inertial { nullptr };

		pros::task_t odomTask;
		static void odomManager(void* param);

		const double MAX_DELTA = (10_in).convert(okapi::inch);
		const double FIELD_WIDTH = (12_ft).convert(okapi::inch);
		const int MIN_CONFIDENCE = 9;

		Point p_00 { 0, 0 };
		Point p_10 { FIELD_WIDTH, 0 };
		Point p_01 { 0, FIELD_WIDTH };
		Point p_11 { FIELD_WIDTH, FIELD_WIDTH };
		LineSegment north {p_10, p_11};
		LineSegment south {p_00, p_01};
		LineSegment east {p_00, p_10};
		LineSegment west {p_01, p_11};
		std::map<double, LineSegment> walls {{0.0, east}, {M_PI_2, north}, {M_PI, west}, {M_PI_2 * 3, south}};

		void step(std::array<double, 4> deltas);
};
} // namespace lib16868C