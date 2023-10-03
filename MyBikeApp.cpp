// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "GPS.h"
#include "ACG.h"
#include "GYRO.h"

int main()
{
	std::filesystem::path relativePath = "Data\\OUTPUT.csv";
	std::filesystem::path absolutePath = std::filesystem::absolute(relativePath);
	std::fstream outputFile{ absolutePath.string(), std::ios::in | std::ios::out | std::ios::trunc};
	if (!outputFile)
	{
		std::cerr << "Could not open: " << absolutePath.string() << std::endl;
	}



	GPS gps("");


	ACG acg("");
	double y_correction = acg.correctMountingresult(100);
	std::cout << "degrees Angle: "<< y_correction <<std::endl;
	acg.correctMounting(y_correction);


	GYRO gyro("", 0, 0, 0);
	//gyro.rotation();
	gyro.correctMounting(0, 0, y_correction);
	//gyro.correctMounting(0, 0, 15);
	gyro.rotationTrue();
	double speed = 0;
	FastAverageDouble avgyrox(100);
	FastAverageDouble avgyrox2(100);
	FastAverageDouble avgyrox3(100);
	double speedonly = 0;
	double sum = 0;
	for (size_t i = 1; i < acg.entries; i++)
	{
		size_t gpsI = gps.getIndexByTime(acg.time[i].x);
		size_t gyroI = gyro.findClosestElement(acg.time[i].x);	
		double gpsspeed = 0;
		if (isnan(gps.speed[gpsI].x))
		{
			gpsspeed = 0.0001;
		}
		else
		{
			gpsspeed = gps.speed[gpsI].x;
		}
		speedonly = speedonly + (acg.time[i].x - acg.time[i - 1].x) * acg.ytrue[i].x;

		speed = speed + (acg.time[i].x - acg.time[i - 1].x) * acg.ytrue[i].x;
		double gpsAndSpeed = 0.998;
		speed = speed * gpsAndSpeed+ gpsspeed * (1- gpsAndSpeed);
		sum = sum + (acg.time[i].x - acg.time[i - 1].x) * acg.y[i].x;
		//outputFile << acg.time[i].x<< "	" << gyro.roll[gyroI].x * 180 / pi << "	" << gyro.pitch[gyroI].x * 180 / pi << "	" << gyro.yaw[gyroI].x * 180 / pi << "	" << speed*3.6 << std::endl;
		outputFile << acg.time[i].x<< "	" << speed * 3.6 << "	" << gpsspeed*3.6 << "	" << speedonly *3.6<< "	" << gps.altitude[gpsI].x<< std::endl;
		//outputFile << avgyrox3.additionalValue(avgyrox2.additionalValue(avgyrox.additionalValue(gyro.x[gyroI].x))) << std::endl;
	}
	outputFile.close();
	std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
