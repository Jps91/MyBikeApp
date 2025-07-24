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


	std::fstream outputFile(folder + "Output.csv", std::ios::trunc | std::ios::out);
	if (!outputFile)
	{
		std::cerr << "Could not open File: " << folder << "Output.csv\n";
	}
	//////////////////////
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	///////////////////////Start with your code here:

	Acceleration_3D_MeterPerSecondSquared correctedACG;
	correctedACG = acceleration_3D_toLocalCoordSystem(track.acgSensor.accelerationMeterPerSecond,);



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



	return 0;
}
