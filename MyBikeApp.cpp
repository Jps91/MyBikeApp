// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <iomanip>

#include "DataStructure.h"
#include "RecordingSession.h"
#include "CSVImporter.h"


int main()
{
	std::string folder = "C:\\temp\\MyBikeApp\\DemoData\\";
	std::string filenameGPS = "Location.csv";
	std::string filenameACG = "Accelerometer.csv";
	std::string filenameGYRO = "Gyroscope.csv";

	RecordingSession track;
	track.initialize(folder, folder + "Output\\");

	GlobalPosition gps;
	gps = track.gpsSensor.gPos;

	for (size_t i = 0; i < track.gpsSensor.gHight.size; i++)
	{
		std::cout << gps.time[i] << "	" << gps.latitude[i] << "	" << gps.longitude[i] << "\n";
	}

	LocalAcceleration acg;
	acg = track.acgSensor.acg;

	for (size_t i = 0; i < acg.size; i++)
	{
		std::cout << acg.time[i] << "	" << acg.x[i] << "	" << acg.y[i] << "\n";
	}


	return 0;
}
