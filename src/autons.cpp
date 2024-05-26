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


void test_auton() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 24, 5000, {},false);
 //   chassis.waitUntilDone();
//     pros::delay(1000);
//     chassis.moveToPose(0, 0, 0, 5000, {.forwards = false});
//     chassis.waitUntilDone();
//     pros::delay(1000);
//     chassis.moveToPose(0, 48, 0, 5000);
//     chassis.waitUntilDone();
}

void optical_test() {
    intakeReset();
    while (true) {
        if (optical_sensor.get_hue() >= 90 && optical_sensor.get_hue() <= 120) {
            pros::lcd::set_text(4, "");
            pros::lcd::set_text(3, "triball detected!");
            pros::delay(500);
            intakeShoot();
            pros::lcd::set_text(3, "shot!");
            intakeReset();
            pros::lcd::set_text(3, "reset!");
        }
        else {
            pros::lcd::set_text(4, "waiting for triball!");
            pros::lcd::set_text(3, "");
        }
        pros::delay(20);
    }
}

// void pid_turn_test() {
//     chassis.setPose(0, 0, 0);
//     chassis.moveToPose(0, 0, 90, 3000);
//     lemlib::Pose pose = chassis.getPose();
//     pros::lcd::set_text(3, std::to_string(pose.theta));
//     pros::delay(2000);
    
//     chassis.moveToPose(0, 0, -90, 3000);
//     lemlib::Pose pose = chassis.getPose();
//     pros::lcd::set_text(3, std::to_string(pose.theta));

// }

void auton() {
    chassis.setPose(-32, -55, 0);
    chassis.moveToPose(-35, -11, 0, 5000, {}, false);
    pros::delay(500);
    chassis.turnToHeading(-90, 500, {}, false);
    pros::delay(500);
    wingBR.set_value(true);
    pros::delay(500);
    chassis.moveToPose(72, -11, -90, 1000, {.forwards=false, .maxSpeed = 80}, false);
    chassis.setPose(-7.75, chassis.getPose().y, chassis.getPose().theta);
    pros::delay(500);
    chassis.moveToPose(-10.75, chassis.getPose().y, -90, 1000, {.forwards=false}, false); // move back 3 inches
    wingBR.set_value(false);
    pros::delay(500);
    chassis.turnToHeading(-20, 1000, {}, false);
    pros::delay(500);
    setDrive(-400, -400);
    pros::delay(2000);
    setDrive(0, 0);
    chassis.setPose(-8.25, -39.5, 0);
}