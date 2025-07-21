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
	/////stuff you want to initialize befor starting the clock. E.g.: To test ceartain objects.
	CSVImporter importer;
	CSVData data;


	//////////////////////
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	///////////////////////Start with your code here:


	data = importer.importDataV2("C:\\temp\\MyBikeApp\\DemoData\\Gyroscope.csv", "");




	///////////////////////End with your code here
	auto end = high_resolution_clock::now();
	duration<double, std::milli> elapsed = end - start;

	std::cout << "Time in Miliseconds: " << elapsed.count() << "\n";
	return elapsed.count();
}

void testCycling(size_t n)
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
