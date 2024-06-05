#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	chassis.calibrate(true);
	pros::lcd::set_text(1, "Hello PROS User!");

	driveLB.set_brake_mode(MOTOR_BRAKE_COAST);
	driveLM.set_brake_mode(MOTOR_BRAKE_COAST);
    driveLF.set_brake_mode(MOTOR_BRAKE_COAST);

    driveRB.set_brake_mode(MOTOR_BRAKE_COAST);
	driveRM.set_brake_mode(MOTOR_BRAKE_COAST);
    driveRF.set_brake_mode(MOTOR_BRAKE_COAST);

	driveLB.tare_position();
	driveLM.tare_position();
    driveLF.tare_position();

    driveRB.tare_position();
	driveRM.tare_position();
    driveRF.tare_position();
	
	// pros::Task log_data_task(log_data);
	// pros::Task screen_task(screen);

	cata1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	cata2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	shooter1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	shooter2.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	// test_auton();
	// optical_test(); // SLOT 2
	// pid_turn_test();
	auton(); // SLOT 1
	// skills(); // SLOT 3
	// test_auton(); // SLOT 3
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::delay(2000);

	my_opcontrol();
	// chassis.turnToHeading(90, 9999999);
	// setDrive(6000, 6000);
	// pros::delay(1000);
	// setDrive(0, 0);
}
