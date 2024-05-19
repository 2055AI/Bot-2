#pragma once
#include <functional>

namespace lib16868C {
struct PIDGains {
	double kP, kI, kD, kF = 0;
};

class PIDController {
	public:
		/**
		 * @brief Construct a new PID Controller
		 * 
		 * @param gains The kP, kI, kD, and kF terms
		 * @param outputMax The maximum output
		 * @param outputMin The minimum output
		 * @param maxIntegral The maximum integral will sum up to
		 * @param integralRange The range within the target when integral starts accumulating
		 * @param resetIntegralOnCross Whether the integral resets when crossing the target
		 * @param settleCond The settle condition for when the PID Controller will consider itself settled
		 */
		PIDController(PIDGains gains, double outputMax = 1, double outputMin = -1, double maxIntegral = 1e5, double integralRange = 15, bool resetIntegralOnCross = true, std::function<bool()> settleCond = {[]() { return false; }});
		/**
		 * @brief Copy constructor of a PID Controller
		 * 
		 * @param pidController 
		 */
		PIDController(const PIDController& pidController);

		/**
		 * @brief Updates the PID gains
		 * 
		 * @param gains The kP, kI, kD, and kF terms
		 */
		void setGains(PIDGains gains);
		/**
		 * @brief Set the maximum of the output and the minimum of the output
		 * 
		 * @param max The maximum output
		 * @param min The minimum output
		 */
		void setOutputMaxMin(double max, double min);
		/**
		 * @brief Set the maximum integral
		 * 
		 * @param max The maximum integral value
		 */
		void setMaxIntegral(double max);
		/**
		 * @brief Sets whether it resets when it crosses the target
		 * 
		 * @param resetIntegralOnCross True or false
		 */
		void setResetIntegralOnCross(bool resetIntegralOnCross);
		/**
		 * @brief Set the settle condition
		 * 
		 * @param settleCond A function that returns a boolean to determine whether the controller has settled or not
		 */
		void setSettleCondition(std::function<bool()> settleCond);

		/**
		 * @brief Calculate the output of the PID controller
		 * 
		 * @param error The error between the current sensor reading and the target
		 * @return double The output of the controller
		 */
		double calculate(double error);
		/**
		 * @brief Calculate the output of the PID controller
		 * 
		 * @param target The target sensor reading
		 * @param curent The current sensor reading
		 * @return double The output of the controller
		 */
		double calculate(double target, double curent);

		/**
		 * @brief Checks whether the settle condition is met
		 * 
		 * @return true The settle condition is met
		 * @return false The settle condition has not yet been met
		 */
		bool isSettled();

	private:
		PIDGains gains;
		double outputMax, outputMin;
		double maxIntegral;
		double integralRange;
		bool resetIntegralOnCross;
		double output;
		double prevError = 0;
		double integral = 0;
		double prevTime = 0;

		std::function<bool()> settleCond;
};
} // namespace lib16868C