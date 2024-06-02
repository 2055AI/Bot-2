#include "main.h"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/rtos.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <cmath>

// void wallReset(std::string wall, std::string sensor) {
//     double Pi = 3.14159265358979323846;
//     double offset = 4;
//     if (wall == "top" && sensor == "left") {
//         double inches = DistanceL.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(chassis.getPose().x, 72 - (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().theta);
//     }
//     else if (wall == "top" && sensor == "right") {
//         double inches = DistanceR.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(chassis.getPose().x, 72 - (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().theta);
//     }
//     else if (wall == "bottom" && sensor == "left") {
//         double inches = DistanceL.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(chassis.getPose().x, -72 + (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().theta);
//     }
//     else if (wall == "bottom" && sensor == "right"){
//         double inches = DistanceR.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(chassis.getPose().x, -72 + (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().theta);
//     }
//     else if (wall == "right" && sensor == "right") {
//         double inches = DistanceR.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(72 - (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().y, chassis.getPose().theta);
//     }
//     else if (wall == "right" && sensor == "left") {
//         double inches = DistanceL.get() * 0.0393701;
//         double distance = inches * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)));
//         chassis.setPose(72 - (distance + offset * abs(cos((Pi/2)-(chassis.getPose().theta*Pi/180)))), chassis.getPose().y, chassis.getPose().theta);
//     }

// }