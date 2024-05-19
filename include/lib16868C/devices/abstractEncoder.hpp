#pragma once

namespace lib16868C {
class AbstractEncoder {
	public:
		/**
		 * @brief Gets the encoder ticks
		 * 
		 * @return double The encoder ticks
		 */
		virtual double get() = 0;
		/**
		 * @brief Resets the encoder ticks
		 */
		virtual void resetZero() = 0;
		/**
		 * @brief Gets the velocity of the encoder
		 * 
		 * @return double The velocity
		 */
		virtual double getVelocity() = 0;

		/**
		 * @brief Gets the ticks per rotation
		 * 
		 * @return int The number of ticks per rotation
		 */
		int getTPR();
	
	protected:
		int tpr;
};
} // namespace lib16868C