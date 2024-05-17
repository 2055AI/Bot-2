#include "main.h"

void setDrive(double left, double right) {
    driveLB.move_voltage(left);
    driveLM.move_voltage(left);
    driveLF.move_voltage(left);

    driveRB.move_voltage(right);
    driveRM.move_voltage(right);
    driveRF.move_voltage(right);
}   