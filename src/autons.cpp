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
    setIntake(60);
    pros::delay(300);
    setIntake(0);

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
            intakeShoot(127);
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
                intakeShoot(127);
                setIntake(0);
                counter += 1;
            } else {
                pros::delay(125);
                intakeShoot(127);
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
                intakeShoot(127);
                setIntake(0);
                counter += 1;
            } else {
                pros::delay(125);
                intakeShoot(127);
                intakeReset();
                counter += 1;
            } 

        }
        if (time > 2000 && counter <= 1) {
            intakeShoot(127);
            setIntake(0);
            wingBL.set_value(true);
            pros::delay(200);
            bowl_isolation(8, 500);
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
            intakeShoot(127);
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

void endgame() {
    setIntake(127);
    setCata(-127);
    pros::delay(600);
    setIntake(0);
    setCata(0);

    setDrive(400, 400);
    pros::delay(750);
    hang.set_value(true);
    cataShoot();    
    pros::delay(200);
    setIntake(-100);
    pros::delay(150);
    setIntake(0);
    setDrive(0, 0);
}

void auton() {
    shooter1.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
	shooter2.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    chassis.setPose(-32, -55, 0);
    chassis.moveToPose(-35, -12, 0, 3000, {}, false);
    pros::delay(500);
    chassis.turnToHeading(-90, 500, {}, false);
    pros::delay(500);
    wingBR.set_value(true);
    // pros::delay(500);
    chassis.moveToPose(72, -12, -90, 1200, {.forwards=false, .maxSpeed = 80}, false);
    chassis.setPose(-7.75, chassis.getPose().y, chassis.getPose().theta);
    pros::delay(500);
    chassis.moveToPose(-10.75, chassis.getPose().y, -90, 1000, {.forwards=false}, false); // move back 3 inches
    wingBR.set_value(false);
    pros::delay(500);
    chassis.turnToHeading(-20, 1000, {}, false);
    pros::delay(500);
    setDrive(-400, -400);
    pros::delay(1000);
    setDrive(0, 0);
    chassis.setPose(-8.5, -40, 0);
    
    // SET CATAPULT AND INTAKE IN PROPER POSITIONS
    setCata(-127);
    pros::delay(65);
    setCata(0);
    intakeReset();
    cataShoot();
    intakeShoot(60);
    

    // MOVE TO BOWL
    chassis.moveToPose(-11, -25, -10, 2000, {}, false);
    pros::delay(500);
    chassis.turnToHeading(50, 1000, {}, false);
    pros::delay(500);
    chassis.moveToPose(-60, -54, 45, 2000, {.forwards=false}, false);
    pros::delay(500);
    chassis.moveToPose(chassis.getPose().x + 1, chassis.getPose().y + 1, chassis.getPose().theta, 1000, {.maxSpeed=60}, false);
    pros::delay(500);
    chassis.turnToHeading(-35, 1000, {}, false);
    pros::delay(500);
    chassis.moveToPose(chassis.getPose().x + 3.441, chassis.getPose().y - 4.915, -35, 1000, {.forwards=true}, false);
    pros::delay(500);

    // MATCH LOAD
    bowl_isolation(6, 750); //6
    pros::delay(500);

    // BOWL & SIDE RAM
    chassis.turnToHeading(-50, 900, {}, false);
    // wingBL.set_value(true);
    wingBR.set_value(true);
    pros::delay(150);
    chassis.moveToPose(45, -70, -90, 3000, {.forwards = false, .maxSpeed = 98}, false);
    chassis.moveToPose(70, -30, 180, 2000, {.forwards = false, .maxSpeed = 400});
    chassis.waitUntil(30);
    wingBL.set_value(false);
    chassis.waitUntilDone();
    chassis.setPose(61, -29, 180);
    
    // SECOND SIDE RAM
    chassis.moveToPose(64, chassis.getPose().y - 15, 180, 1200, {}, false);
    chassis.turnToHeading(195, 500, {}, false);
    // chassis.moveToPose(64, chassis.getPose().y + 35, 180, 1200, {.forwards = false, .minSpeed = 127}, false);
    setDrive(-400, -400);
    pros::delay(1000);
    setDrive(0, 0);
    wingBL.set_value(false);
    wingBR.set_value(false);
    chassis.setPose(63, -29, 180);
    chassis.turnToHeading(0,500,{},false);
    pros::delay(1000);
    chassis.moveToPose(0, -60, 90, 2000, {.forwards=false}, false);
    // chassis.moveToPose(5, -58, -90, 5000, {}, false);
    setDrive(-150, -150);
    while(true){
        if(optical_sensor.get_proximity() > 100){
            pros::delay(100);
            break;
        }
    }

    // setDrive(1, 1);
    //one minute 15
    pros::delay(800);
    chassis.setPose(7,62,-90);
    chassis.moveToPose(33, 64, -90, 2000, {.forwards=false, .maxSpeed=80}, false);
    chassis.turnToHeading(-180, 2000);
    setDrive(-300,-300);
    pros::delay(800);
    setDrive(0, 0);
    void wallReset();



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
