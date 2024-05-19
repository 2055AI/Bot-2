#pragma once
#include "api.h"
#include "lib16868C/devices/abstractEncoder.hpp"
#include "lib16868C/util/math.hpp"

namespace lib16868C {
class Rotation : private pros::Rotation, public AbstractEncoder {
	public:
		Rotation(int port);
		Rotation(uint port, bool reversed);

		/**
		 * @brief Returns the current position of the encoder in degrees.
		 * 
		 * @return double 
		 */
		double get() override;
		/**
		 * @brief Manually sets the zero of the encoder.
		 * * Does not actually reset the encoder.
		 */
		void resetZero() override;

		/**
		 * @brief Gets the velocity of the encoder.
		 * ? I do not know how accurate this is
		 * 
		 * @return double 
		 */
		double getVelocity();
	
	private:
		double prevTicks = 0;
		uint prevTime = 0;
};
} // namespace lib16868C