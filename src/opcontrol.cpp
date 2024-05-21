#include "main.h"
#include "pros/gps.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <ostream>
#include <fstream>

void my_opcontrol() {
    while (true) {
        setIntakeMotors();
        setDriveMotors();

        // while (true) {
        //     int power = controller.get_analog(ANALOG_LEFT_Y);
        //     int turn = controller.get_analog(ANALOG_RIGHT_X);
        //     int left = power + turn;
        //     int right = power - turn;
        //     leftMotors.move(left);
        //     rightMotors.move(right);

        //     pros::delay(2);
        // }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
            cataHangPrep();
        }
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
            cataHangElevate();
        }
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
            cataShoot();
        }

        int x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    	int y = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

        lemlib::Pose pose = chassis.getPose(); // get the current position of the robot

        pros::lcd::set_text(5, "X: "  +  std::to_string(pose.x)); // print the x position
        pros::lcd::set_text(6, "Y: " + std::to_string(pose.y)); // print the y position
        pros::lcd::set_text(7, "Angle: " + std::to_string(pose.theta)); // print the heading
        

        pros::delay(50);


    }
}