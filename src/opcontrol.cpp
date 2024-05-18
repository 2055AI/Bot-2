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

        int x = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    	int y = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        

        pros::delay(10);


    }
}