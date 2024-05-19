#include "main.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <string>

void setIntake(int power) {
    intake1.move(power);
    intake2.move(power);
}

void setIntakeMotors() {
    int intake_power = 127 * (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2) - controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1));
    setIntake(intake_power);    
}

void setCata(int power) {
    cata1.move(power);
    cata2.move(power);
}
void cataShoot() {
    pros::Task moveCata([&] {
        while (true) {
            setCata(127);

            if (cata_distance_sensor.get() <= 100) break;

            pros::delay(20);
        }

        setCata(0);
    });
}
void cataHangPrep() {
    pros::Task moveCata([&] {
        while (true) {
            setCata(50);

            if (cata_distance_sensor.get() > 1500) break;

            pros::delay(20);
        }

        setCata(0);
    });
}
void cataHangElevate() {
    pros::Task moveCata([&] {
        while (true) {
            setCata(70);

            if (cata_distance_sensor.get() <= 60) break;

            pros::delay(20);
        }

        setCata(0);
    });
}