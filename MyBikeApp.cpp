// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <fstream>


#include "DataStructure.h"
#include "RecordingSession.h"
#include "CSVImporter.h"
#include "Algorithms.h"




double testPerformance()
{
	/////stuff you want to initialize befor starting the clock. E.g.: To test ceartain objects.
	std::string folder = "C:\\temp\\MyBikeApp\\DemoData\\";
	std::string filenameGPS = "Location.csv";
	std::string filenameACG = "Accelerometer.csv";
	std::string filenameGYRO = "Gyroscope.csv";



	RecordingSession track;
	track.initialize(folder, folder);


	std::fstream outputFile(folder + "Output.csv",std::ios::trunc|std::ios::out);
	if (!outputFile)
	{
		std::cerr << "Could not open File: " << folder << "Output.csv\n";
	}
	//////////////////////
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	///////////////////////Start with your code here:

	Rotation_3D_Quaternion rotation{};
	for (size_t i = 1; i < track.gyroSensor.gyro.size; i++)
	{
		Rotation_3D_RadiansPerSecond rot_rad{ track.gyroSensor.gyro.time[i],track.gyroSensor.gyro.rollPerSecond[i],track.gyroSensor.gyro.pitchPerSecond[i],track.gyroSensor.gyro.yawPerSecond[i], };
		double deltaTime = track.gyroSensor.gyro.time[i] - track.gyroSensor.gyro.time[i - 1];
		Rotation_3D_Quaternion rot_delta = quaternion_delta_from_Rotation_3D_RadiansPerSecond(rot_rad, deltaTime);
		rotation = quaternion_Multiply(rotation, rot_delta);
		outputFile << euler_Radians_from_Quaternion(rotation).time << "	" << euler_Radians_from_Quaternion(rotation).pitch * 180 / 3.14 << "	" << euler_Radians_from_Quaternion(rotation).roll * 180 / 3.14 << "	" << euler_Radians_from_Quaternion(rotation).yaw * 180 / 3.14 << "\n";
	}
	outputFile.close();


	///////////////////////End with your code here
	auto end = high_resolution_clock::now();
	duration<double, std::milli> elapsed = end - start;

	std::cout << "Time in Miliseconds: " << elapsed.count() << "\n";


	return elapsed.count();
}

static void testCycling(const size_t n)
{
	std::vector<double>results;
	for (size_t i = 0; i < n; i++)
	{
		results.push_back(testPerformance());
	}
	double sum = 0;
	for (size_t i = 0; i < results.size(); i++)
	{
		sum += results[i];
	}
	double average = sum / results.size();

	std::cout << "result: " << average << "\n";
}

int main()
{
	testCycling(10);
	return 0;

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


	return 0;
}
