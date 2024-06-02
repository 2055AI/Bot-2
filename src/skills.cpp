#include "main.h"
#include "lemlib/api.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/optical.h"
#include "pros/rtos.h"
#include "pros/rtos.hpp"
#include <cmath>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>


void skills() {
    // Set Initial Position
    chassis.setPose(-50, 58, -53.5);
    pros::delay(200);

    // Set Up 
    setCata(-127);
    pros::delay(65);
    setCata(0);
    intakeReset();
    cataShoot();

    // Shoot 22 Tri-balls
    skills_matchload(1);

    // Push Over Middle Tri-balls
    chassis.turnToHeading(-35, 800, {}, false);
    chassis.moveToPose(-20, 42, -90, 1200, {.forwards = false}, false);
    setDrive(-600, -600);
    pros::delay(500);
    setDrive(0, 0);

    //chassis.setPose(-7.5, chassis.getPose().y, -90);
    chassis.turnToHeading(-5, 800, {}, false);
    wingBL.set_value(true);
    // pros::delay(150);
    setDrive(-600, -600);
    pros::delay(400);
    setDrive(0, 0);
    chassis.moveToPose(-5, -45, 0, 1200, {.forwards = false, .minSpeed = 120}, false);
    wingBL.set_value(false);

    // Distance Sensor Resets
    // wallReset("right", "right");

    // Move to Other Matchload Bar
    chassis.setPose(chassis.getPose().x, -40, 0);
    pros::delay(250);
    chassis.moveToPose(chassis.getPose().x, -30, 0, 1000, {}, false);
    pros::delay(150);
    chassis.turnToHeading(240, 900, {}, false);
    chassis.moveToPose(-42, -48, 235, 2300, {}, false); //-56, -60
    // pros::delay(150);
    // chassis.moveToPose(-48, -50, 230, 1750, {.minSpeed = 100}, false); //-60, -56
    wingBR.set_value(true);
    pros::delay(150);
    chassis.turnToHeading(50, 1100, {.direction = lemlib::AngularDirection::CCW_COUNTERCLOCKWISE}, false);
    wingBR.set_value(false);
    pros::delay(150);
    chassis.turnToHeading(235, 1100, {}, false);
    chassis.moveToPose(-50, -54, 235, 2000, {}, false);

    // Shoot 12 Tri-balls OR Bowl 8
    bowl_matchload(3);
    pros::delay(150);

    // Bowl / Side Ram
    shooter1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	shooter2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    chassis.turnToHeading(-50, 900, {}, false);
    wingBR.set_value(true);
    // wingBL.set_value(true);
    pros::delay(150);
    chassis.moveToPose(25, -66, -90, 2500, {.forwards = false, .minSpeed = 100}, false);
    // wingBL.set_value(false);
    chassis.moveToPose(64, -10, 180, 2500, {.forwards = false, .minSpeed = 127}, false);
    // chassis.moveToPose(64, chassis.getPose().y - 15, 180, 1000, {.minSpeed = 80}, false);
    // chassis.moveToPose(64, chassis.getPose().y + 40, 180, 1000, {.forwards = false, .minSpeed = 127}, false);
    
    // Hang
    chassis.setPose(chassis.getPose().x, -30, chassis.getPose().theta);
    wingBR.set_value(false);
    pros::delay(150);
    chassis.moveToPose(10, -56, -90, 2000, {.minSpeed = 80}, false);
    chassis.moveToPose(25, -56, -90, 2000, {.forwards = false, .minSpeed = 100}, false);

    endgame();
    
}