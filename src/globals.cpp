#include "main.h"
#include "pros/adi.hpp"
#include "pros/gps.h"
#include "pros/gps.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"
#include "lemlib/api.hpp"
#include <cstddef>

// ------------------------------------------------------------------------------------------------------
/**
 * Globals cpp file to store all the pros variables
 * This is where you change the ports, direction of motors, and encoder values
*/
// ------------------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------
// Define the port numbers here: 
// - Use numbers if it is in a port
// - Use letters if it is a triport
// ------------------------------------------------------------------------------------------------------
int MOTOR_LB = 9;
int MOTOR_LM = 16;
int MOTOR_LF = 19;

int MOTOR_RB = 1;
int MOTOR_RM = 15;
int MOTOR_RF = 20;

int MOTOR_INTAKE1 = 17;
int MOTOR_INTAKE2 = 11;

int MOTOR_CATAPULT1 = 18;
int MOTOR_CATAPULT2 = 13;

int INERTIAL_SENSOR_PORT = 6;

int GPS_LEFT_SENSOR_PORT = 15;
int GPS_RIGHT_SENSOR_PORT = 16;

int TRACKING_WHEEL_VERTICAL_SENSOR_PORT = 2;
int TRACKING_WHEEL_HORIZONTAL_SENSOR_PORT = 5;

char CATA_DISTANCE_SENSOR_PORT = 7;


// lemlib::TrackingWheel back_tracking_wheel(&back_rot, 2.75, -5.75);

// ------------------------------------------------------------------------------------------------------
// Drivetrain 
// ------------------------------------------------------------------------------------------------------
pros::Motor driveLB(MOTOR_LB, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor driveLM(MOTOR_LM, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor driveLF(MOTOR_LF, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor driveRB(MOTOR_RB, pros::E_MOTOR_GEARSET_06, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor driveRM(MOTOR_RM, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor driveRF(MOTOR_RF, pros::E_MOTOR_GEARSET_06, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::MotorGroup leftMotors({driveLB, driveLM, driveLF});
pros::MotorGroup rightMotors({driveRB, driveRM, driveRF});


// ------------------------------------------------------------------------------------------------------
// Subsystems
// ------------------------------------------------------------------------------------------------------
pros::Motor intake1(MOTOR_INTAKE1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor intake2(MOTOR_INTAKE2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor cata1(MOTOR_CATAPULT1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata2(MOTOR_CATAPULT2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// ------------------------------------------------------------------------------------------------------
// Sensors
// ------------------------------------------------------------------------------------------------------

pros::IMU inertial_sensor(INERTIAL_SENSOR_PORT);
pros::Rotation vert_encoder(TRACKING_WHEEL_VERTICAL_SENSOR_PORT);
pros::Rotation hort_encoder(TRACKING_WHEEL_HORIZONTAL_SENSOR_PORT);
pros::Distance cata_distance_sensor(CATA_DISTANCE_SENSOR_PORT);

// ------------------------------------------------------------------------------------------------------
// Pneumatics
// ------------------------------------------------------------------------------------------------------
pros::ADIDigitalOut hang('A');
pros::ADIDigitalOut wingF('B');
pros::ADIDigitalOut wingBR('C');
pros::ADIDigitalOut wingBL('D');


// ------------------------------------------------------------------------------------------------------
// CONTROLLER
// ------------------------------------------------------------------------------------------------------
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// ------------------------------------------------------------------------------------------------------
// LEMLIB (For position tracking)
// ------------------------------------------------------------------------------------------------------
// lemlib::Drivetrain drivetrain(&leftMotors, &rightMotors, 10.3125, lemlib::NEW_275, 450, 2);

// lemlib::TrackingWheel vert_tracking(&vert_encoder, lemlib::NEW_275, 2);
// lemlib::TrackingWheel hort_tracking(&hort_encoder, lemlib::NEW_275, 3.875);
// lemlib::OdomSensors snsrs(&vert_tracking, nullptr, &hortTracking, nullptr, &imu);
    
// lemlib::ControllerSettings lateral_controller(1, 0, 0, 3, 1, 100, 3, 500, 30);
// lemlib::ConrollerSettings angular_controller(2, 0, 10, 0, 0, 0, 0, 0, 0, 0);

// lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, snsrs);

// ------------------------------------------------------------------------------------------------------