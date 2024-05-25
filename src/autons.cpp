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

void pid_turn_test() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, 0, 90, 3000);
    lemlib::Pose pose = chassis.getPose();
    pros::lcd::set_text(3, std::to_string(pose.theta));
    pros::delay(2000);
    
    chassis.moveToPose(0, 0, -90, 3000);
    lemlib::Pose pose = chassis.getPose();
    pros::lcd::set_text(3, std::to_string(pose.theta));

}
