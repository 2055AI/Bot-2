#pragma once
#include "api.h"
#include "okapi/api.hpp"
#include "lib16868C/controllers/pidController.hpp"
#include "lib16868C/devices/rotation.hpp"

namespace lib16868C {
void CataMain(void*);

enum class CataState { SETTLED, FIRING, INTAKE, MATCHLOAD };

class Catapult {
public:
	/**
	 * @brief Construct a new Catapult Subsystem object
	 * 
	 * @param mtr The motors on the catapult
	 * @param limit The limit switch used to determine where the reset position of the catapult is
	 */
	Catapult(okapi::MotorGroup& mtrs, Rotation& enc);
	/**
	 * @brief Destroy the Catapult Subsystem object
	 * 
	 */
	~Catapult();

	/**
	 * @brief Add 1 to the target.
	 * Also creates a new task if there was not one running already
	 */
	void fire();
	void intake();
	void matchload();
	/**
	 * @brief Stops the catapult from moving, as well as deleting the task.
	 */
	void stop();

	/**
	 * @brief Moves the catapult at a given velocity
	 * 
	 * @param vel The velocity that the motor spins at
	 */
	void moveVelocity(double vel);

	int getNumFired();

	/**
	 * @brief Whether the catapult is moving or not
	 * 
	 * @return true The task is open and it is moving
	 * @return false The task has been deleted and it is not moving
	 */
	bool isSettled();

	/**
	 * @brief Waits until the catapult has reset
	 */
	void waitForSettled();

private:
	okapi::MotorGroup& mtrs;
	// pros::ADIDigitalIn& limitSwitch;
	// okapi::DistanceSensor& distance;
	Rotation& enc;

	CataState cataState { CataState::SETTLED };

	int numFired = 0;

	pros::task_t ctrlTask;
	friend void CataMain(void*);
};
} // namespace lib16868C