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
