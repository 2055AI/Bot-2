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

void potential_reset(int x, int y, int target_angle) {
    if (chassis.getPose().theta >= target_angle - 10 && chassis.getPose().theta <= target_angle + 10) {
        chassis.setPose(x, y, chassis.getPose().theta);
    }
}

void optical_test() {
    intakeReset();
    while (true) {
        if (optical_sensor.get_hue() >= 90 && optical_sensor.get_hue() <= 120) {
            pros::delay(250);
            intakeShoot();
            intakeReset();
        }
        pros::delay(20);
    }
}

void skills_matchload(int num) {
    intakeReset();
    int counter = 0;
    while (counter < num) {
        if (optical_sensor.get_hue() >= 90 && optical_sensor.get_hue() <= 120) {
            if (counter == num - 1) {
                pros::delay(125);
                intakeShoot();
                setIntake(0);
                counter += 1;
            } else {
                pros::delay(125);
                intakeShoot();
                intakeReset();
                counter += 1;
            } 
        }
        pros::delay(15);
    }

}

// Matchloading on the right side (Skills)
void bowl_matchload(int num) {
    intakeReset();
    int counter = 0;
    int time = 0;
    while (counter < num) {
        if (optical_sensor.get_hue() >= 90 && optical_sensor.get_hue() <= 120) {
            if (counter == num - 1) {
                pros::delay(125);
                intakeShoot();
                setIntake(0);
                counter += 1;
            } else {
                pros::delay(125);
                intakeShoot();
                intakeReset();
                counter += 1;
            } 

        }
        if (time > 2000 && counter == 0) {
            intakeShoot();
            setIntake(0);
            wingBL.set_value(true);
            pros::delay(100);
            chassis.turnToHeading(150, 1200, {}, false);
            bowl(1, 300);
            wingBL.set_value(false);
            break;
        }
        pros::delay(15);
        time += 10;
    }

}

void last_ball() {
    int counter = 0;
    while (counter < 1) {
        if (optical_sensor.get_hue() >= 90 && optical_sensor.get_hue() <= 120) {
            pros::delay(150);
            intakeShoot();
            counter += 1;
        }
    pros::delay(15); 
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
    // chassis.setPose(-32, -55, 0);
    // chassis.moveToPose(-35, -11, 0, 5000, {}, false);
    // pros::delay(500);
    // chassis.turnToHeading(-90, 500, {}, false);
    // pros::delay(500);
    // wingBR.set_value(true);
    // pros::delay(500);
    // chassis.moveToPose(72, -11, -90, 1000, {.forwards=false, .maxSpeed = 80}, false);
    // chassis.setPose(-7.75, chassis.getPose().y, chassis.getPose().theta);
    // pros::delay(500);
    // chassis.moveToPose(-10.75, chassis.getPose().y, -90, 1000, {.forwards=false}, false); // move back 3 inches
    // wingBR.set_value(false);
    // pros::delay(500);
    // chassis.turnToHeading(-20, 1000, {}, false);
    // pros::delay(500);
    // setDrive(-250, -250);
    // pros::delay(2000);
    // setDrive(0, 0);

    // MOVE TO BOWL
    chassis.setPose(-8.5, -40, 0);
    chassis.moveToPose(-11, -25, -10, 2000, {}, false);
    pros::delay(500);
    chassis.turnToHeading(50, 1000, {}, false);
    pros::delay(500);
    chassis.moveToPose(-62, -54, 45, 3000, {.forwards=false}, false);
    pros::delay(500);
    chassis.moveToPose(chassis.getPose().x + 2, chassis.getPose().y + 2, chassis.getPose().theta, 1000, {.maxSpeed=60}, false);
    pros::delay(500);
    chassis.turnToHeading(-35, 1000, {}, false);
    pros::delay(500);
    wingBL.set_value(true);
    chassis.moveToPose(chassis.getPose().x + 3.441, chassis.getPose().y - 4.915, -35, 1000, {.forwards=true}, false);
    pros::delay(500);

    // chassis.moveToPose(-8.5, -20, 0, 5000, {}, false);
    // chassis.turnToHeading(55, 500, {},false);
    // chassis.moveToPose(-48,-58,41, 5000, {.forwards=false});
    // pros::delay(1000);
    // bowl(5,1500);

    // Score the balls
    // chassis.setPose(-51,-52,chassis.getPose().theta);
    // chassis.moveToPose(-38, -55, -90, 2000, {.forwards=false}, true);
    // chassis.moveToPose(32, -55, -90, 4000, {.forwards=false}, true);
    // pros::delay(1500);
}