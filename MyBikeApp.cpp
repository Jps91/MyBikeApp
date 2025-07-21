// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>

#include "DataStructure.h"
#include "RecordingSession.h"
#include "CSVImporter.h"
#include "Algorithms.h"

double testPerformance() 
{
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	///////////////////////Start with your code here:
	





	///////////////////////End with your code here
	auto end = high_resolution_clock::now();
	duration<double, std::milli> elapsed = end - start;

	return elapsed.count();  // return time in milliseconds
}

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

	for (size_t i = 0; i < gps.size / 100; i++)
	{
		//std::cout << gps.time[i] << "	" << gps.latitude[i] << "	" << gps.longitude[i] << "\n";
	}

	LocalAcceleration acg;
	acg = track.acgSensor.acg;

	for (size_t i = 0; i < acg.size / 100; i++)
	{
		//std::cout << acg.time[i] << "	" << acg.x[i] << "	" << acg.y[i] << "\n";

	}

	LocalRoationSpeed gyro;
	gyro = track.gyroSensor.gyro;

	for (size_t i = 0; i < gyro.size / 100; i++)
	{
		//std::cout << gyro.time[i] << "	" << gyro.rollPerSecond[i] << "	" << gyro.pitchPerSecond[i] << "	" << gyro.yawPerSecond[i] << "\n";
	}

	double time = testPerformance();
	std::cout << "Time in Miliseconds: " << time << "\n";



	return 0;
}
