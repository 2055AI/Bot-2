#include "main.h"
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

void log_data() {
    // Reads config file number
    std::ifstream ConfigRead ("/usd/config.txt");

    std::string text;
    std::string filenum;

    while (std::getline(ConfigRead, text)) {
        filenum = text;
    }
    ConfigRead.close();
    std::string filepath = "/usd/Logs/" + filenum + ".txt";
    // std::string filepath1 = "/usd/Logs/" + filenum + "/inertial_sensor_data.txt";

    std::ofstream Data (filepath.c_str());
    // std::ofstream DataSensor (filepath1.c_str());

    // Update config file number
    std::ofstream ConfigWrite ("/usd/config.txt");

    ConfigWrite << std::stoi(filenum) + 1;
    ConfigWrite.close();

	double time = 0;
	Data << "X, Y, θ" << std::endl;
    // DataSensor << "Heading, Rotation, Pitch, Roll, Yaw" << std::endl;
	while (true) {
        if (time >= 5000) {
            break;  
        }
        // double x = GpsR.get_status().x;
		// double y = GpsR.get_status().y;

		// x = x * 39.370079;
		// y = y * 39.370079;
        
        // std::string line = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(inertial_sensor.get_heading());
		// Data << line << std::endl;
        // std::string sensor_line =  std::to_string(inertial_sensor.get_heading()) + ", " + std::to_string(inertial_sensor.get_rotation())+ ", " + std::to_string(inertial_sensor.get_pitch()) + ", " + std::to_string(inertial_sensor.get_roll()) + ", " + std::to_string(inertial_sensor.get_yaw());
        // DataSensor << sensor_line << std::endl;
		pros::delay(50);
        time += 50;
	}
    Data.close();
    // DataSensor.close();
}

void screen() {
    while (true) {
        // double x = GpsR.get_status().x;
        // double y = GpsR.get_status().y;

        // x = x * 39.370079;
        // y = y * 39.370079;

        // pros::lcd::set_text(6, "x, y: " + std::to_string(x) + ", "  + std::to_string(y));
        // // pros::lcd::set_text(5, "x: " ); // print the x position
        // // pros::lcd::set_text(6, "y: " ); // print the y position
        // pros::lcd::set_text(7, "angle: " + std::to_string (GpsR.get_heading())); // print the heading
        // pros::delay(20);
    }
}

