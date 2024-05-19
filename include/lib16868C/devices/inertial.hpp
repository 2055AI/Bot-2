#pragma once
#include "okapi/api.hpp"
#include "lib16868C/util/math.hpp"

namespace lib16868C {
enum class AngleUnit {
	DEG,
	RAD
};

class Inertial : public pros::Imu {
	public:
		/**
		 * @brief Construct a new Inertial
		 * 
		 * @param port The smart port of the inertial on the brain [1, 21]
		 */
		Inertial(uint port);

		/**
		 * @brief Custom reset method that detects failed resets and keeps trying, as well as inertial drift, which it will warn the user
		 */
		void calibrate();
		/**
		 * @brief Get the absolute heading of the robot (-inf, inf)
		 * 
		 * @param unit The unit type that the heading is in (degree, radian)
		 * @return double The heading of the robot in the specified units
		 */
		double get_rotation(AngleUnit unit) const;
		/**
		 * @brief Set the absolute heading of the robot
		 * 
		 * @param heading The heading of the robot
		 */
		void set_rotation(okapi::QAngle heading);
	
	private:
		double DRIFT_THRESHOLD = 1;
};
}