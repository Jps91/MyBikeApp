// MyBikeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <filesystem>
#include "GPS.h"
#include "ACG.h"
#include "GYRO.h"
#include "Speed.h"
#include "Rotation.h"
#include "Position.h"

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
	std::cout << "degrees Angle: " << y_correction << std::endl;
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

	Speed sp(gps, acg);
	//sp.filter();
	Rotation rot(gyro, acg, gps);
	Position pos(gps,acg,rot);

	for (size_t i = 1; i < acg.entries; i++)
	{
		size_t gpsI = gps.getIndexByTime(acg.time[i].x);
		size_t gyroI = gyro.findClosestElement(acg.time[i].x);

		//outputFile << acg.time[i].x<< "	" << gyro.roll[gyroI].x * 180 / pi << "	" << gyro.pitch[gyroI].x * 180 / pi << "	" << gyro.yaw[gyroI].x * 180 / pi << "	" << speed*3.6 << std::endl;
		outputFile << acg.time[i].x << "	" << pos.x[i].x << "	" << pos.y[i].x << "	" << gyro.pitch[gyroI].x << "	" << gps.altitude[gpsI].x << std::endl;
		//outputFile << avgyrox3.additionalValue(avgyrox2.additionalValue(avgyrox.additionalValue(gyro.x[gyroI].x))) << std::endl;
	}
	outputFile.close();
	std::cout << "Hello World!\n";
}
