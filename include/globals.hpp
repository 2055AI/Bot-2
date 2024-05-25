#include "lemlib/chassis/chassis.hpp"
#include "pros/adi.hpp"
#include "pros/distance.hpp"
#include "pros/gps.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"
#include "pros/optical.hpp"
#include "pros/vision.hpp"


extern pros::Motor driveLB;
extern pros::Motor driveLM;
extern pros::Motor driveLF;

extern pros::Motor driveRB;
extern pros::Motor driveRM;
extern pros::Motor driveRF;

extern pros::MotorGroup leftMotors;
extern pros::MotorGroup rightMotors;

extern pros::Motor shooter1; 
extern pros::Motor shooter2;  

extern pros::Motor cata1;
extern pros::Motor cata2;

extern pros::IMU inertial_sensor;
extern pros::Rotation vert_encoder;
extern pros::Rotation hort_encoder;
extern pros::Distance cata_distance_sensor;
extern pros::Optical optical_sensor;

extern pros::ADIDigitalOut hang;
extern pros::ADIDigitalOut wingF;
extern pros::ADIDigitalOut wingBR;
extern pros::ADIDigitalOut wingBL;

extern pros::Controller controller;

extern lemlib::TrackingWheel vert_tracking;
extern lemlib::TrackingWheel hort_tracking;
extern lemlib::Chassis chassis;