#include "lemlib/chassis/trackingWheel.hpp"
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

int SHOOTER_PORT1 = 17;
int SHOOTER_PORT2 = 11;

int MOTOR_CATAPULT1 = 18;
int MOTOR_CATAPULT2 = 13;

int INERTIAL_SENSOR_PORT = 6;

int GPS_LEFT_SENSOR_PORT = 15;
int GPS_RIGHT_SENSOR_PORT = 16;

int TRACKING_WHEEL_VERTICAL_SENSOR_PORT = 2;
int TRACKING_WHEEL_HORIZONTAL_SENSOR_PORT = 5;

char CATA_DISTANCE_SENSOR_PORT = 7;

int OPTICAL_SENSOR_PORT = 8;


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
pros::Motor shooter1(SHOOTER_PORT1, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor shooter2(SHOOTER_PORT2, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor cata1(MOTOR_CATAPULT1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor cata2(MOTOR_CATAPULT2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// ------------------------------------------------------------------------------------------------------
// Sensors
// ------------------------------------------------------------------------------------------------------

pros::IMU inertial_sensor(INERTIAL_SENSOR_PORT);
pros::Rotation vert_encoder(TRACKING_WHEEL_VERTICAL_SENSOR_PORT);
pros::Rotation hort_encoder(TRACKING_WHEEL_HORIZONTAL_SENSOR_PORT);
pros::Distance cata_distance_sensor(CATA_DISTANCE_SENSOR_PORT);
pros::Optical optical_sensor(OPTICAL_SENSOR_PORT);

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

/**
 * @brief Define the parameters of your drivetrain here:
 */
lemlib::Drivetrain drivetrain {
    &leftMotors, // left drivetrain motors
    &rightMotors, // right drivetrain motors
    10.30, // track width
    lemlib::Omniwheel::NEW_275, // wheel diameter
    600, // wheel rpm
    8 // Chase Power
};

// forward/backward PID
lemlib::ControllerSettings linearController {
    12, // kP    //8
    0, // KI
    0, // kD May lower in the future      //25
    0, // anti windup
    1, // smallErrorRange //1
    300, // smallErrorTimeout //300
    3, // largeErrorRange //3
    600, // largeErrorTimeout //600
    20 // slew rate     //5
};
 
// turning PID
lemlib::ControllerSettings angularController {
    1, // kP
    0, // kI
    0, // kD
    0, // anti windup
    1, // smallErrorRange
    100, // smallErrorTimeout
    3, // largeErrorRange
    500, // largeErrorTimeout
    0 // slew rate
};
    
lemlib::TrackingWheel vert_tracking(&vert_encoder, lemlib::Omniwheel::NEW_275, 1.75);
lemlib::TrackingWheel hort_tracking(&hort_encoder, lemlib::Omniwheel::NEW_275, 4.25);

/**
 * @brief Define the sensors used for the robot here:
 */
lemlib::OdomSensors sensors {
    &vert_tracking, // vertical tracking wheel 1
    nullptr, // vertical tracking wheel 2
    &hort_tracking, // horizontal tracking wheel 1
    nullptr, // horizontal tracking wheel 2
    &inertial_sensor // inertial sensor
};

lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors);

// ------------------------------------------------------------------------------------------------------



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